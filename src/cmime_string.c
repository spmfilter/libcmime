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

#include "cmime_string.h"
#include "cmime_util.h"
#include "cmime_base64.h"
#include "cmime_qp.h"

CMimeStringList_T *cmime_string_list_new(void) {
    CMimeStringList_T *sl;
    sl = (CMimeStringList_T *)calloc((size_t)1,sizeof(CMimeStringList_T));
    sl->count = 0;
    sl->node = NULL;
    return(sl);
}

void cmime_string_list_free(CMimeStringList_T *sl) {
    size_t i;
    assert(sl);

    for(i = 0; i < sl->count; i++) {
        if (sl->node[i]!=NULL)
            free(sl->node[i]);
    } 
    free(sl->node);
    sl->node = NULL;
    free(sl); 
}

void cmime_string_list_insert(CMimeStringList_T *sl, const char *s) {
    char **tmp = NULL;
    assert(sl);
    assert(s);

    tmp = realloc(sl->node, (sizeof( *tmp) * (sl->count+1)));
    tmp[sl->count] = strdup(s);
    sl->node = tmp;
    sl->count++;
}

char *cmime_string_list_get(CMimeStringList_T *sl,int pos) {
    assert(sl);
    assert(pos <= sl->count);
    return(sl->node[pos]);
}

char *cmime_string_strip(char *s) {
    assert(s);

    char * e = s + strlen(s) - 1;
    while(*s && isspace(*s)) s++;
    while(e > s && isspace(*e)) *e-- = '\0';

    return(s);
}

char *cmime_string_chomp(char *s) {
    char *p = NULL;
    
    switch(s[strlen(s)-1]) {
        case '\n':
            if((p = strrchr(s,'\r')) != NULL ) *p = '\0';
            else *(p = strrchr(s,'\n')) = '\0';
            break;
        case '\r':
            *(p = strrchr(s,'\r')) = '\0';
            break;
        case '\x0c':
            *(p = strrchr(s,'\x0c')) = '\0';
            break;
    }

    return(s);
}

int cmime_string_is_7bit(const char *s) {
    const char *it = NULL;
    assert(s);

    it = s;
    while(*it != '\0') {
        if (((unsigned char)(*it) > (unsigned char) 127)||(*it < 0))
            return(-1);
        it++;
    }

    return(0);
}

int cmime_string_is_8bit(const char *s) {
    const char *it = NULL;

    assert(s);
    it = s;
    while(*it != '\0') {
        if ((unsigned char)(*it) > (unsigned char) 127)
            return(0);
    }
        
    return(-1);
}

char *cmime_string_encode_to_7bit(const char *s, CMimeStringEncodingType_T t) {
    CMimeInfo_T *mi = NULL;
    char *out = NULL;
    char *ptemp = NULL;
    char *enc = NULL;

    assert(s);

    // =?charset?encoding?text?=
    mi = cmime_util_info_get_from_string(s);

    if (strcmp(mi->mime_encoding,"us-ascii")!=0) {
        if (t == CMIME_STRING_ENCODING_B64) {
            ptemp = cmime_base64_encode_string(s); 
            enc = strdup("b");
        } else if (t == CMIME_STRING_ENCODING_QP) {
            ptemp = cmime_qp_encode((char *)s,NULL); 
            ptemp = cmime_string_chomp(ptemp);
            enc = strdup("q");
        } else {
            ptemp = strdup(s);
        }

        asprintf(&out,"=?%s?%s?%s?=",mi->mime_encoding,enc,ptemp);
        free(ptemp);
        free(enc);
    } else {
        out = strdup(s);
    }

    cmime_util_info_free(mi);
    return out;
}
