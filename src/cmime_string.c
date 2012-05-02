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

#include "cmime_string.h"

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
        if (((*it) > (unsigned char) 127)||(*it < 0))
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
        if ((*it) > (unsigned char) 127)
            return(0);
    }
        
    return(-1);
}

