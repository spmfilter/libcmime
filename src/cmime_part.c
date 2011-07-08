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
#include <libgen.h>
#include <ctype.h>

#include "cmime_message.h"
#include "cmime_part.h"
#include "cmime_util.h"
#include "cmime_internal.h"
#include "cmime_base64.h"
#include "cmime_string.h"

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
	
	part->content_type = (char *)calloc(strlen(s) * sizeof(char),sizeof(char));
	strcpy(part->content_type,s);
}

void cmime_part_set_content_disposition(CMimePart_T *part, const char *s) {
	assert(part);
	assert(s);
	
	if (part->content_disposition != NULL)
		free(part->content_disposition);
	
	part->content_disposition = (char *)calloc(strlen(s) * sizeof(char),sizeof(char));
	strcpy(part->content_disposition,s);
}

void cmime_part_set_content_transfer_encoding(CMimePart_T *part, const char *s) {
	assert(part);
	assert(s);
	
	if (part->content_transfer_encoding != NULL)
		free(part->content_transfer_encoding);
	
	part->content_transfer_encoding = (char *)calloc(strlen(s) * sizeof(char),sizeof(char));
	strcpy(part->content_transfer_encoding,s);
}

void cmime_part_set_content(CMimePart_T *part, const char *s) {
	assert(part);
	assert(s);
	
	if (part->content != NULL)
		free(part->content);
	
	part->content = (char *)calloc(strlen(s) * sizeof(char),sizeof(char));
	strcpy(part->content,s);
}

char *cmime_part_as_string(CMimePart_T *part) {
	char *out = NULL;
	char *content = NULL; 
	char *s = NULL;
	char *ptemp = NULL;
	int with_headers = 0;
	
	assert(part);
	
	content = cmime_part_get_content(part);
	if (content==NULL)
		return(NULL);
		
	out = (char *)calloc(1,sizeof(char));
			
	/* TODO: Check line endings, it should not be used CRLF as default... */	
	ptemp = cmime_part_get_content_type(part);
	if (ptemp != NULL) {
		asprintf(&s,HEADER_CONTENT_TYPE_PATTERN,ptemp,CRLF);
		out = (char *)realloc(out,strlen(out) + strlen(s) + sizeof(char));
		strcat(out,s);
		free(s);
		with_headers = 1;
	}

	ptemp = cmime_part_get_content_disposition(part);
	if (ptemp != NULL) {
		asprintf(&s,HEADER_CONTENT_DISPOSITION_PATTERN,ptemp,CRLF);
		out = (char *)realloc(out,strlen(out) + strlen(s) + sizeof(char));
		strcat(out,s);
		free(s);
		with_headers = 1;
	}

	ptemp = cmime_part_get_content_transfer_encoding(part);
	if (ptemp != NULL) {
		asprintf(&s,HEADER_CONTENT_TRANSFER_ENCODING_PATTERN,ptemp,CRLF);
		out = (char *)realloc(out,strlen(out) + strlen(s) + sizeof(char));
		strcat(out,s);
		free(s);
		with_headers = 1;
	}

	if (with_headers==1) {
		out = (char *)realloc(out,strlen(out) + strlen(CRLF) + sizeof(char));
		strcat(out,CRLF);
	} 
	
	out = (char *)realloc(out,strlen(out) + strlen(content) + sizeof(char));
	strcat(out,content);

	return(out);
}

int cmime_part_from_file(CMimePart_T **part, char *filename) {
	struct stat fileinfo;
	int retval;
	char *mimetype;
	char *ptemp1;
	char *ptemp2;
	FILE *fp = NULL;
	int encode;
	int i = 0;
	int len = 0;
	int blocklen = 0;
	int blocksout = 0;
	int pos = 0;
	unsigned char in[3], out[4];
	char *disposition;
	
	assert((*part));
	assert(filename);
	
	ptemp1 = filename;
	
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
			
			cmime_part_set_content_type((*part),mimetype);
			encode = (strncmp(mimetype,MIMETYPE_TEXT_PLAIN,strlen(MIMETYPE_TEXT_PLAIN)) == 0) ? 0 : 1;
			free(mimetype);
					
			if (encode == 1) 
				cmime_part_set_content_transfer_encoding((*part),"base64");
			else 
				cmime_part_set_content_transfer_encoding((*part),"7bit");
				
			ptemp2 = basename(ptemp1);
			asprintf(&disposition,"attachment; filename=%s",ptemp2);
			cmime_part_set_content_disposition((*part),disposition);		
			free(disposition);
			
			fp = fopen(filename, "rb");
			if (fp != NULL) {
				(*part)->content = (char *)calloc(1,sizeof(char));
				while(!feof(fp)) {
					len = 0;
					
					for(i=0; i<3; i++) {
						in[i] = (unsigned char)fgetc(fp);

						if(!feof(fp)) {
							len++;
						} else {
							in[i] = 0;
						}
					}
					
					if (len) {
						(*part)->content = (char *)realloc((*part)->content,strlen((*part)->content) + len + sizeof(char));
						if (encode == 0) 
							blocklen = 3;
						else {
							cmime_base64_encode_block(in,out,len);
							blocklen = 4;
						}
					
						for (i=0; i<blocklen;i++) {
							(*part)->content[pos++] = (encode == 0) ? in[i] : out[i];
						}
						blocksout++;
					}

					if(blocksout >= (LINE_LENGTH / 4) || feof(fp)) {
						if(blocksout && (encode == 1)) {
							/* if base64 data, we need to do a line break after LINE_LENGTH chars */
							(*part)->content = (char *)realloc((*part)->content,strlen((*part)->content) + strlen(CRLF) + sizeof(char));
							// TODO: add support for other line endings
							(*part)->content[pos++] = '\r';
							(*part)->content[pos++] = '\n';
						}
						blocksout = 0;
					} 
				}
				fclose(fp);
			} else {
				return (-3); /* failed to open file */
			}
			
		} else {
			return(-2); /* not regular file */
		} 
	} else {
		return(-1); /* stat error */
	}
	
	return(0);
}

int cmime_part_from_string(CMimePart_T **part, const char *content) {
	CMimeStringList_T *sl = NULL;
	char *s = NULL;
	char *ptemp = NULL;
	int i = 0;
	int found_header = 0;
	char *body = NULL;
	
	assert((*part));
	assert(content);
	sl = cmime_string_split(content,CRLF,0);
	
	body = (char *)calloc(1,sizeof(char));
	
	for(i=0;i<cmime_string_list_get_count(sl);i++) {
		s = cmime_string_list_get(sl,i);
		ptemp = strchr(s,':');
		if (ptemp!=NULL) {
			if (isspace(*++ptemp)!=0) {
				if (strcasestr(s,"Content-Type")!=NULL) { 
					cmime_part_set_content_type((*part),cmime_string_strip(ptemp));
					found_header = 1;
				} else if (strcasestr(s,"Content-Transfer-Encoding")!=NULL) {
					cmime_part_set_content_transfer_encoding((*part),cmime_string_strip(ptemp));
					found_header = 1;
				} else if (strcasestr(s,"Content-Disposition")!=NULL) {
					cmime_part_set_content_disposition((*part),cmime_string_strip(ptemp));
					found_header = 1;
				}
			}
		} 
		
		if (found_header==0) {
			body = (char *)realloc(body,strlen(body) + strlen(s) + sizeof(char));
			strcat(body,s);
		} else 
			found_header = 0;
	}
	cmime_part_set_content((*part),body);
	free(body);
	
	cmime_string_list_free(sl);
	return(0);
}
