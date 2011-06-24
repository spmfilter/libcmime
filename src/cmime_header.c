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

#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
	
	header->name = (char *)malloc(strlen(name) + 1);
	strcpy(header->name, name);
}

void cmime_header_set_value(CMimeHeader_T *header, const char *value) {
	char **tmp = NULL;
	assert(header);
	assert(value);

	tmp = realloc(header->value, (sizeof( *tmp) * (header->count+1)));
	tmp[header->count] = malloc(strlen(value)+1);
	strcpy(tmp[header->count],value);
	header->value = tmp;
	header->count++;
}

char *cmime_header_get_value(CMimeHeader_T *header,int pos) {
	assert(header);
	assert(pos <= header->count);
	
	return(header->value[pos]);
}
