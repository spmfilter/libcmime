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

#include "cmime_internal.h"
#include "cmime_header.h"
#include "cmime_string.h"
#include "cmime_part.h"

void _cmime_internal_header_destroy(void *data) {
    CMimeHeader_T *header = (CMimeHeader_T *)data;
    assert(data);
    cmime_header_free(header);
}

char *_cmime_internal_determine_linebreak(const char *s) {
    assert(s);

    if (strstr(s,CRLF)!=NULL)
        return(CRLF);
    else if(strstr(s,LF)!=NULL)
        return(LF);
    else if(strstr(s,CR)!=NULL) 
        return(CR);
    else
        return(NULL);
}


char *_cmime_internal_determine_linebreak_from_file(const char *s) {
    FILE *fp = NULL;
    char line_in[512];
    char *nl = NULL;

    assert(s);

    fp = fopen(s, "rb");
    if (fp != NULL) {
        while (fgets(line_in, 512, fp)!=NULL){
            nl = _cmime_internal_determine_linebreak(line_in);
            if (nl != NULL)
                break;
        }
        if (fclose(fp)!=0)
            perror("libcmime: error closing file");
    } else {
        perror("libcmime: error opening file");
        return(NULL);
    }

    if (nl == NULL)
        nl = CRLF;
    
    return(nl);
}

/* replace or add a new linked header */
void _cmime_internal_set_linked_header_value(CMimeList_T *l, const char *key, const char *value) {
    CMimeListElem_T *e = NULL;
    CMimeHeader_T *h = NULL;
    char *ptemp = NULL;
    char *cp = NULL;
    
    assert(l);
    assert(key);
    
    ptemp = (char *)value;
    if (value != NULL)  {
        cp = strdup(value);
        ptemp = cmime_string_strip(cp);
    }

    e = cmime_list_head(l);
    while(e != NULL) {
        h = (CMimeHeader_T *)cmime_list_data(e);
        if (strcasecmp(cmime_header_get_name(h),key)==0) {
            if (value!=NULL)
                cmime_header_set_value(h,ptemp,1);
            free(cp);
            return;
        }
        e = e->next;
    }
    
    h = cmime_header_new();
    cmime_header_set_name(h,key);
    if (value != NULL)
        cmime_header_set_value(h,ptemp,0);

    cmime_list_append(l,h);
    free(cp);

    return;
}

char *_cmime_internal_get_linked_header_value(CMimeList_T *l, const char *key) {
    CMimeListElem_T *e;
    CMimeHeader_T *h;
    
    assert(l);
    assert(key);
    
    e = cmime_list_head(l);
    while(e != NULL) {
        h = (CMimeHeader_T *)cmime_list_data(e);
        if (strcasecmp(cmime_header_get_name(h),key)==0) {
            return(cmime_header_get_value(h,0));
        }
        e = e->next;
    }
    
    return(NULL);
}

CMimeHeader_T *_cmime_internal_get_linked_header(CMimeList_T *l, const char *key) {
    CMimeListElem_T *e;
    CMimeHeader_T *h;
    
    assert(l);
    assert(key);
    
    e = cmime_list_head(l);
    while(e != NULL) {
        h = (CMimeHeader_T *)cmime_list_data(e);
        if (strcasecmp(cmime_header_get_name(h),key)==0) {
            return(h);
        }   
        
        e = e->next;
    }
    
    return(NULL);
}

void _cmime_internal_parts_destroy(void *data) {
    CMimePart_T *p = (CMimePart_T *)data;
    assert(data);
    cmime_part_free(p);
}

_BoundaryInfo_T *_cmime_internal_get_boundary_info(CMimeStringList_T *boundaries, char *s, char *newline) {
    _BoundaryInfo_T *info = NULL;
    int i;
    char *marker = NULL;
    char *t = NULL;
    char *bound;
    size_t offset;

    if (newline != NULL) {
        t = strstr(s,newline);
        if (t!=NULL) {
            offset = strlen(s) - strlen(t);
            t = (char *)calloc(offset + sizeof(char),sizeof(char));
            strncpy(t,s,offset);
            t[strlen(t)] = '\0';
            for(i=0; i < cmime_string_list_get_count(boundaries); i++) {    
                bound = cmime_string_list_get(boundaries,i);
                asprintf(&marker,"--%s--",bound);
                if (strcmp(t,marker)==0) {
                    info = (_BoundaryInfo_T *)calloc((size_t)1,sizeof(_BoundaryInfo_T));
                    info->marker = strdup(marker);
                    info->type = CMIME_BOUNDARY_CLOSE;
                    info->len = strlen(marker);
                    free(marker);
                    break;
                } else {
                    free(marker);
                    asprintf(&marker,"--%s",bound);
                    if (strcmp(t,marker)==0) {
                        info = (_BoundaryInfo_T *)calloc((size_t)1,sizeof(_BoundaryInfo_T));
                        info->marker = strdup(marker);
                        info->type = CMIME_BOUNDARY_OPEN;
                        info->len = strlen(marker);
                        free(marker);
                        break;
                    }
                    free(marker);
                }
            }
            free(t);
        }
    }

    return(info);
}
