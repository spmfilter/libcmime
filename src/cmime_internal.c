/* libcmime - A C mime library
 * Copyright (C) 2012 SpaceNet AG and Axel Steiner <ast@treibsand.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#define _GNU_SOURCE

#include "cmime_internal.h"
#include "cmime_header.h"
#include "cmime_string.h"
#include "cmime_part.h"

void _cmime_internal_header_destroy(void *data) {
    assert(data);
    CMimeHeader_T *header = (CMimeHeader_T *)data;
    cmime_header_free(header);
}

char *_cmime_internal_determine_linebreak(const char *s) {
    assert(s);

    if (strstr(s,CRLF)!=NULL)
        return(CRLF);
    else if(strstr(s,CR)!=NULL)
        return(CR);
    else if(strstr(s,LF)!=NULL)
        return(LF);
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
    
    assert(l);
    assert(key);
    
    ptemp = (char *)value;
    if (value != NULL)      
        ptemp = cmime_string_strip(ptemp);

    e = cmime_list_head(l);
    while(e != NULL) {
        h = (CMimeHeader_T *)cmime_list_data(e);
        if (strcasecmp(cmime_header_get_name(h),key)==0) {
            if (value!=NULL)
                cmime_header_set_value(h,ptemp,1);
            return;
        }
        e = e->next;
    }
    
    h = cmime_header_new();
    cmime_header_set_name(h,key);
    if (value != NULL)
        cmime_header_set_value(h,ptemp,0);

    cmime_list_append(l,h);

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
    assert(data);
    CMimePart_T *p = (CMimePart_T *)data;
    cmime_part_free(p);
}

char *_cmime_internal_match_boundary(CMimeStringList_T *boundaries, char *s, char *newline) {
    int i;
    char *marker = NULL;
    char *out = NULL;
    char *t = NULL;
    size_t offset;

    if (newline != NULL) {
        t = strstr(s,newline);
        if (t!=NULL) {
            offset = strlen(s) - strlen(t);
            t = (char *)calloc(offset + sizeof(char),sizeof(char));
            strncpy(t,s,offset);
            t[strlen(t) + sizeof(char)] = '\0';
            for(i=0; i < cmime_string_list_get_count(boundaries); i++) {    
                asprintf(&marker,"--%s--",cmime_string_list_get(boundaries,i));
                if (strcmp(t,marker)==0) {
                    out = strdup(marker);
                    free(marker);
                    break;
                } else {
                    free(marker);
                    asprintf(&marker,"--%s",cmime_string_list_get(boundaries,i));
                    if (strcmp(t,marker)==0) {
                        out = strdup(marker);
                        free(marker);
                        break;
                    }
                    free(marker);
                }
            }
            free(t);
        }
    }

    return(out);
}
