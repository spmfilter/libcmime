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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "cmime_string.h"
#include "cmime_flbi.h"

char *cmime_flbi_chomp_boundary(char *s, char *linebreak) {
    char *out = NULL;
    char *p = NULL;
    int offset = 0;

    p = strstr(s,linebreak);
    if (p) {
        offset = strlen(s) - strlen(p);
        if (offset > 0) {
            out = (char *)calloc(offset + sizeof(char),sizeof(char));
            strncpy(out,s,offset);
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

    if (t != NULL)
        boundary = cmime_string_strip(t);
    else
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
