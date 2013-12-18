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

CMimeHeader_T *cmime_header_new(void) {
    CMimeHeader_T *h;
    h = (CMimeHeader_T *)calloc((size_t)1,sizeof(CMimeHeader_T));
    h->count = 0;
    h->name =  NULL;
    h->value = NULL;
    return(h);
}

void cmime_header_free(CMimeHeader_T *header) {
    size_t i;
    assert(header);


    if (header->name != NULL)
        free(header->name);
        
    for(i = 0; i < header->count; i++) {
        if (header->value[i] != NULL) 
            free(header->value[i]);
    } 
    free(header->value);
    header->value = NULL;
    free(header); 
}

void cmime_header_set_name(CMimeHeader_T *header, const char *name) {
    assert(header);
    assert(name);
    
    if (header->name != NULL) 
        free(header->name);
    
    header->name = strdup(name);
}

void cmime_header_set_value(CMimeHeader_T *header, const char *value, int overwrite) {
    char **tmp = NULL;
    size_t i;
    assert(header);

    if (overwrite==1) {
        for(i = 0; i < header->count; i++) {
            if (header->value[i] != NULL) 
                free(header->value[i]);
        }
        header->count = 0;
    }

    tmp = realloc(header->value, (sizeof( *tmp) * (header->count+1)));
    if (value != NULL)
        tmp[header->count] = strdup(value);
    else 
        tmp[header->count] = NULL;
    header->value = tmp;
    
    header->count++;
}

char *cmime_header_get_value(CMimeHeader_T *header,int pos) {
    assert(header);
    assert(pos <= header->count);
    
    if (header->value != NULL)
        return(header->value[pos]);
    else
        return(NULL);
}

char *cmime_header_to_string(CMimeHeader_T *header) {
    char *out = NULL;
    char *value = NULL;
    int i = 0;
    char *ptemp = NULL;
    
    assert(header);

    out = (char *)calloc(sizeof(char),sizeof(char));
    for(i = 0; i < cmime_header_get_count(header); i++) {
        value = cmime_header_get_value(header,i);
        if ((value !=NULL) && (strlen(value)>0)) {
            if (header->parsed==1) {
                asprintf(&ptemp,"%s:%s",cmime_header_get_name(header),value);
            } else {
                if (value[0] == (unsigned char)32)
                    asprintf(&ptemp,"%s:%s",cmime_header_get_name(header),value);
                else
                    asprintf(&ptemp,"%s: %s",cmime_header_get_name(header),value);
            }
        } else {
            asprintf(&ptemp,"%s:",cmime_header_get_name(header));
        }
        out = (char *)realloc(out,strlen(out) + strlen(ptemp) + 1);
        strcat(out,ptemp);
        free(ptemp);
    }
    
    return(out);
}
