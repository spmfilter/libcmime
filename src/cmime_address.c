/* libcmime - A C mime library
 * Copyright (C) 2011 Axel Steiner <ast@treibsand.com>
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

#include "cmime_address.h"

/* Creates a new CMIMEAddress_T object */
CMimeAddress_T *cmime_address_new(void) {
	CMimeAddress_T *ca;
	ca = (CMimeAddress_T *)calloc((size_t)1,sizeof(CMimeAddress_T));
	ca->name = NULL;
	ca->email = NULL;
	return(ca);
}

/* Set the display name of CMimeAdresse_T object */
void cmime_address_set_name(CMimeAddress_T *ca, const char *name) {
	assert(ca);
	assert(name);
	
	if (ca->name != NULL)
		free(ca->name);
		
	ca->name = (char *)malloc(strlen(name) + sizeof(char));
	strcpy(ca->name,name);
}

/* Set the email address of CMimeAdresse_T object */
void cmime_address_set_email(CMimeAddress_T *ca, const char *email) {
	assert(ca);
	assert(email);
	
	if (ca->email != NULL)
		free(ca->email);
		
	ca->email = (char *)malloc(strlen(email) + sizeof(char));
	strcpy(ca->email, email);
}

/* Allocates a string containing the contents of the CMimeAddress_T object. */ 
char *cmime_address_to_string(CMimeAddress_T *ca) {
	char *s = NULL;
	
	assert(ca);
	
	asprintf(&s,"%s <%s>",ca->name,ca->email);
	
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
	char *t2 = NULL;
	int i,size_in;
	
	size_in = strlen(s);
	t1 = strrchr(s,'<');
	if (t1 != NULL) {
		i = size_in - strlen(t1);
		if (*(s + i - 1) == ' ')
			i--;
		ca->name = (char *)calloc(i + sizeof(char),sizeof(char));
		memcpy(ca->name,s,i);
		
		t2 = strrchr(t1,'>');
		if (t2 != NULL) {
			i = strlen(++t1) - strlen(t2);
		} else {
			i = strlen(++t1);
		}
		ca->email = (char *)calloc(i + sizeof(char),sizeof(char));
		memcpy(ca->email,t1,i);
	} 	else {
		ca->email = (char *)calloc(size_in + sizeof(char),sizeof(char));
		memcpy(ca->email,s,size_in);
	}
	
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