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

#include "cmime_part.h"

CMimePart_T *cmime_part_new(void) {
	CMimePart_T *part = NULL;
	
	part = (CMimePart_T *)calloc((size_t)1, sizeof(CMimePart_T));
	part->content_type = NULL;
	part->content_disposition = NULL;
	part->content_transfer_encoding = NULL;
	part->content = NULL;
	
	return(part);
}

void cmime_part_free(CMimePart_T *part) {
	assert(part);
	
	if (part->content_type != NULL)
		free(part->content_type);
	
	if (part->content_disposition != NULL)
		free(part->content_disposition);
		
	if (part->content_transfer_encoding != NULL)
		free(part->content_transfer_encoding);
		
	if (part->content != NULL)
		free(part->content);
		
	free(part);
}

void cmime_part_set_content_type(CMimePart_T *part, const char *s) {
	assert(part);
	assert(s);
	
	if (part->content_type != NULL)
		free(part->content_type);
	
	part->content_type = (char *)malloc(strlen(s) + 1);
	strcpy(part->content_type,s);
}

void cmime_part_set_content_disposition(CMimePart_T *part, const char *s) {
	assert(part);
	assert(s);
	
	if (part->content_disposition != NULL)
		free(part->content_disposition);
	
	part->content_disposition = (char *)malloc(strlen(s) + 1);
	strcpy(part->content_disposition,s);
}

void cmime_part_set_content_transfer_encoding(CMimePart_T *part, const char *s) {
	assert(part);
	assert(s);
	
	if (part->content_transfer_encoding != NULL)
		free(part->content_transfer_encoding);
	
	part->content_transfer_encoding = (char *)malloc(strlen(s) + 1);
	strcpy(part->content_transfer_encoding,s);
}

void cmime_part_set_content(CMimePart_T *part, const char *s) {
	assert(part);
	assert(s);
	
	if (part->content != NULL)
		free(part->content);
	
	part->content = (char *)malloc(strlen(s) + 1);
	strcpy(part->content,s);
}

char *cmime_part_as_string(CMimePart_T *part) {
	char *out = NULL;
	char *type = NULL;
	char *disposition = NULL;
	char *encoding = NULL;
	char *content = NULL;
	char *s = NULL;
	size_t i = 0;
	size_t size = 0;
	assert(part);
	
	content = cmime_part_get_content(part);
	if (content==NULL)
		return(NULL);
	
	type = cmime_part_get_content_type(part);
	if (type!=NULL)
		i = strlen(HEADER_CONTENT_TYPE) + strlen(type) + 4;
	else {
		i = strlen(HEADER_CONTENT_TYPE) + strlen(DEFAULT_CONTENT_TYPE) + 4;
	}
	
	disposition = cmime_part_get_content_disposition(part);
	encoding = cmime_part_get_content_transfer_encoding(part);
	
	
	
	
}

