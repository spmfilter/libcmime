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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>

#include "cmime_message.h"
#include "cmime_part.h"
#include "cmime_util.h"

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
	
	assert(part);
	
	content = cmime_part_get_content(part);
	if (content==NULL)
		return(NULL);
	
	out = (char *)malloc(sizeof(char));
	/* TODO: Check line endings, it should not be used CRLF as default... */
	type = cmime_part_get_content_type(part);
	if (type != NULL) {
		asprintf(&s,HEADER_CONTENT_TYPE_PATTERN,type,CRLF);
		out = (char *)realloc(out,strlen(out) + strlen(s));
		strcat(out,s);
		free(s);
	}

	encoding = cmime_part_get_content_transfer_encoding(part);
	if (encoding != NULL) {
		asprintf(&s,HEADER_CONTENT_TRANSFER_ENCODING_PATTERN,encoding,CRLF);
		out = (char *)realloc(out,strlen(out) + strlen(s));
		strcat(out,s);
		free(s);
	}
	
	disposition = cmime_part_get_content_disposition(part);
	if (disposition != NULL) {
		asprintf(&s,HEADER_CONTENT_DISPOSITION_PATTERN,disposition,CRLF);
		out = (char *)realloc(out,strlen(out) + strlen(s));
		strcat(out,s);
		free(s);
	}


	if ((type!=NULL) || (encoding!=NULL) || (disposition!=NULL)) {
		out = (char *)realloc(out,strlen(out) + strlen(CRLF));
		strcat(out,CRLF);
	} 
	
	out = (char *)realloc(out,strlen(out) + strlen(content));
	strcat(out,content);

	return(out);
}

int cmime_part_from_file(CMimePart_T **part, const char *filename) {
	CMimePart_T *part;
	struct stat fileinfo;
	char *ptemp1;
	int retval;
	char *mimetype;
	
	part = cmime_part_new();
	ptemp1 = filename; /* Important! basename() fucks up pointer */
	
	/* only regular files please */
	retval = stat(filename,&fileinfo);
	if (retval == 0) {
		if(S_ISREG(fileinfo.st_mode)) {
			mimetype = cmime_util_get_mimetype(filename);
			
			/* set default mime type if no one found */
			if(mimetype == NULL) {
				mimetype = (char *)calloc(strlen(MIMETYPE_DEFAULT) + 1, sizeof(char));
				strncpy(mimetype, MIMETYPE_DEFAULT, strlen(MIMETYPE_DEFAULT));
			}
			
			cmime_part_set_content_type(part,mimetype);
			free(mimetype);
			
		} else {
			return(-2); /* not regular file */
		} 
	} else {
		return(-1); /* stat error */
	}
	
	return(0);
}

