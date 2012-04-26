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

#include "cmime_address.h"
#include "cmime_string.h"

/* Creates a new CMIMEAddress_T object */
CMimeAddress_T *cmime_address_new(void) {
    CMimeAddress_T *ca;
    ca = (CMimeAddress_T *)calloc((size_t)1,sizeof(CMimeAddress_T));
    ca->name = NULL;
    ca->email = NULL;
    ca->parsed = 0;
    return(ca);
}

/* Set the display name of CMimeAdresse_T object */
void cmime_address_set_name(CMimeAddress_T *ca, const char *name) {
    assert(ca);
    assert(name);
    
    if (ca->name != NULL)
        free(ca->name);
        
    ca->name = strdup(name);
}

/* Set the email address of CMimeAdresse_T object */
void cmime_address_set_email(CMimeAddress_T *ca, const char *email) {
    assert(ca);
    assert(email);
    
    if (ca->email != NULL)
        free(ca->email);

    ca->email = strdup(email);
}

/* Allocates a string containing the contents of the CMimeAddress_T object. */ 
char *cmime_address_to_string(CMimeAddress_T *ca) {
    char *s = NULL;
    
    assert(ca);
    if(ca->name != NULL) {
        if (ca->parsed==1) 
            asprintf(&s,"%s%s",ca->name,ca->email);
        else
            asprintf(&s,"%s %s",ca->name,ca->email);
    } else {
        asprintf(&s,"%s",ca->email);
    }
    
    return(s);
}

void cmime_address_set_type(CMimeAddress_T *ca, CMimeAddressType_T t) {
    assert(ca);

    ca->type = t;
}

/* Parse given string and create CMimeAddress_T object */
CMimeAddress_T *cmime_address_parse_string(const char *s) {
    CMimeAddress_T *ca = cmime_address_new();
    char *t1 = NULL;
    int size_in;
    int offset;

    size_in = strlen(s);
    t1 = strrchr(s,'<');
    if (t1 != NULL) {
        offset = size_in - strlen(t1);

        if (offset > 0) {
            ca->name = (char *)calloc(offset + sizeof(char),sizeof(char));
            strncpy(ca->name,s,offset);
            ca->name[strlen(ca->name) + sizeof(char)] = '\0';
        }

        ca->email = (char *)calloc(strlen(t1) + sizeof(char),sizeof(char));
        strcpy(ca->email,t1);
    }   else {
        ca->email = (char *)calloc(size_in + sizeof(char),sizeof(char));
        strncpy(ca->email,s,size_in);
        ca->email[strlen(ca->email) + sizeof(char)] = '\0';
    }

    ca->parsed = 1;
    return(ca);
}

/* Free used memory */
void cmime_address_free(CMimeAddress_T *ca) {
    if (ca!=NULL) {
        if (ca->name != NULL)
            free(ca->name);
        if (ca->email != NULL)
            free(ca->email);
        free(ca);
    }
}