/* libcmime - A C mime library
 * Copyright (C) 2013 SpaceNet AG and Axel Steiner <ast@treibsand.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "cmime_part.h"
#include "cmime_internal.h"
#include "cmime_string.h"
#include "cmime_flbi.h"

char *cmime_flbi_boundary_linebreak(char *s, char *linebreak) {
    char *out = NULL;
    char *p = NULL;
    int len;

    p = strstr(s,linebreak);
    if (p!=NULL) {
        len = strlen(linebreak);
        out = (char *)calloc(len + sizeof(char), sizeof(char));
        strncpy(out,p,len);
        out[strlen(out)] = '\0';
    }

    return(out);   
}

char *cmime_flbi_chomp_boundary(char *s, char *linebreak) {
    char *out = NULL;
    char *p = NULL;
    int offset = 0;

    p = strstr(s,linebreak);
    if (p!=NULL) {
        offset = strlen(s) - strlen(p);
        if (offset > 0) {
            out = (char *)calloc(offset + sizeof(char),sizeof(char));
            strncpy(out,s,offset);
            out[strlen(out)] = '\0';
        }
    } else 
        out = strdup(s);

    return(out);
}


char *cmime_flbi_get_boundary(char *s) {
    char *t = NULL;
    char *boundary = NULL;  
    char *p = NULL;
    int pos = 0;

    p = strcasestr(s,"boundary=");
    if (p != NULL) {
        s = strstr(p,"=");
        if (*++s=='"') 
            s++;
                
        t = (char *)calloc(strlen(s) + sizeof(char),sizeof(char));
        while(*s!='\0') {
            if ((*s!=';') && (*s!='"'))
                t[pos++] = *s;
            else {
                t[pos] = '\0';
                break;
            }
            s++;
        }
    }

    if (t != NULL) {
        char *stripped = cmime_string_strip(t);
        if (stripped != t) {
            boundary = strdup(stripped);
            free(t);
        } else
            boundary = stripped;
    } else
        boundary = t;

    return boundary;
}

void cmime_flbi_check_part_boundary(CMimePart_T *part) {
    CMimeListElem_T *e = NULL;
    CMimeHeader_T *h = NULL;
    char *bound = NULL;

    e = cmime_list_head(part->headers);
    while(e != NULL) {
        h = (CMimeHeader_T *)cmime_list_data(e);

        bound = cmime_flbi_get_boundary(cmime_header_get_value(h,0));
        if (bound != NULL) {
            part->boundary = bound;
            break;
        }
        e = e->next;
    }
}

char *cmime_flbi_scan_postface(char *s, CMimeMessage_T *msg) {
    char *it = s;
    _BoundaryInfo_T *info = NULL;
    char *boundary;
    char *postface = NULL;
    char *nxt = NULL;
    _BoundaryInfo_T *nxt_info = NULL;
    char *t = NULL;
    int count;
    int offset = 0;
    CMimeListElem_T *elem = NULL;
    CMimePart_T *part = NULL;
    int match = 0;

    count = 0;
    while((it = strstr(it,"--"))!=NULL) {
        info = _cmime_internal_get_boundary_info(msg->boundaries,it,msg->linebreak);
        if (info!=NULL) {
            //len = strlen(marker);
            //if ((marker[len-2]=='-')&&(marker[len-1]=='-')) {
            if (info->type == CMIME_BOUNDARY_CLOSE) {
                if (count == 0) {
                    offset = strlen(s) - strlen(it);
                    postface = (char *)calloc(offset + sizeof(char),sizeof(char));
                    strncpy(postface,s,offset);
                    postface[strlen(postface)] = '\0';
                    count++;
                } 
                nxt = it + info->len;
                if ((nxt = strstr(nxt,"--"))!=NULL) { 
                    if ((nxt_info = _cmime_internal_get_boundary_info(msg->boundaries,nxt,msg->linebreak))!=NULL) {  
                        it = it + nxt_info->len;
                        offset = strlen(it) - strlen(nxt); 
                        free(nxt_info->marker);
                        free(nxt_info);
                        nxt_info = NULL;
                    } else
                        offset = strlen(s) - strlen(it);
                } else {
                    it = it + info->len;
                    offset = strlen(s) - strlen(it);
                }

                /* jump over 2 starting - chars for boundary comparsion */
                boundary = info->marker;
                boundary += 2 * sizeof(char);
                /* find mime part which belongs to this boundary */
                elem = cmime_list_tail(msg->parts);
                while(elem!=NULL) {
                    part = (CMimePart_T *)cmime_list_data(elem);
                    match = 0;
                    if (part->parent_boundary!=NULL) {
                        if (strncmp(part->parent_boundary,boundary,strlen(part->parent_boundary))==0) 
                            match = 1;
                    }

                    if (part->boundary!=NULL) {
                        if (strncmp(part->boundary,boundary,strlen(part->boundary))==0) 
                            match = 1;
                    }
                    
                    if (match == 1) {
                        if (part->postface!=NULL)
                            free(part->postface);

                        t = (char *)calloc(offset + sizeof(char),sizeof(char));
                        strncpy(t,it,offset);
                        t[strlen(t)] = '\0';
                        part->postface = t;
                        part->last = 1;
                        break;
                   } 
                    
                    elem = elem->prev;
                }
            }
            count++;
            free(info->marker);
            free(info);
            info = NULL;
        }
        
        it++;
    }
    
    if (postface==NULL)
        postface = strdup(s);
    return(postface);
}

