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
            ca->name[strlen(ca->name)] = '\0';
        }

        ca->email = (char *)calloc(strlen(t1) + sizeof(char),sizeof(char));
        strcpy(ca->email,t1);
    }   else {
        ca->email = (char *)calloc(size_in + sizeof(char),sizeof(char));
        strncpy(ca->email,s,size_in);
        ca->email[strlen(ca->email)] = '\0';
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

