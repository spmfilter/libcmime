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
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>

#include "cmime_address.h"
#include "cmime_message.h"
#include "cmime_list.h"
#include "cmime_table.h"
#include "cmime_string.h"
#include "cmime_header.h"
#include "cmime_part.h"
#include "cmime_internal.h"

void _recipients_destroy(void *data) {
	assert(data);
	CMimeAddress_T *ca = (CMimeAddress_T *)data;
	cmime_address_free(ca);
}

void _parts_destroy(void *data) {
	assert(data);
	CMimePart_T *p = (CMimePart_T *)data;
	cmime_part_free(p);
}

CMimeMessage_T *cmime_message_new(void) {
	CMimeMessage_T *message = NULL;
	
	message = (CMimeMessage_T *)calloc((size_t)1, sizeof(CMimeMessage_T));

	if (cmime_list_new(&message->headers,_cmime_internal_header_destroy)!=0)
		return(NULL);
	
	message->sender = NULL;
	if (cmime_list_new(&message->recipients,_recipients_destroy)!=0) 
			return(NULL);

	message->date = 0;
	message->tz_offset = 0;
	message->boundary = NULL;

	if (cmime_list_new(&message->parts,_parts_destroy)!=0) 
			return(NULL);

	return(message);
}

/** Free a CMimeMessage_T object  */
void cmime_message_free(CMimeMessage_T *message) {
	assert(message);

	cmime_address_free(message->sender);	
	cmime_list_free(message->recipients);

	cmime_list_free(message->headers);
	
	if (message->boundary!=NULL)
		free(message->boundary);
	
	cmime_list_free(message->parts);
	
	free(message);
}

/* Set sender of message */
void cmime_message_set_sender(CMimeMessage_T *message, const char *sender) {
	CMimeAddress_T *ca = NULL;
	assert(message);
	
	ca = cmime_address_parse_string(sender);
	if (message->sender != NULL)
		cmime_address_free(message->sender);
	message->sender = ca;
}

void cmime_message_set_message_id(CMimeMessage_T *message, const char *mid) {
	_cmime_internal_set_linked_header_value(message->headers,"Message-ID",mid);
}

char *cmime_message_get_message_id(CMimeMessage_T *message) {
	return(_cmime_internal_get_linked_header_value(message->headers,"Message-ID"));
}

int cmime_message_set_header(CMimeMessage_T *message, const char *header) {
	CMimeStringList_T *sl;
	char *k;
	char *v;
	assert(message);
	assert(header);
	
	sl = cmime_string_split(header,":",1);
	k = cmime_string_list_get(sl,0);
	k = cmime_string_strip(k);

	v = cmime_string_list_get(sl,1);
	v = cmime_string_strip(v);
	
	_cmime_internal_set_linked_header_value(message->headers,k,v);
	cmime_string_list_free(sl);

	return(0);
}

CMimeHeader_T *cmime_message_get_header(CMimeMessage_T *message, const char *header) {
	CMimeHeader_T *h = NULL;
	CMimeListElem_T *e;
	
	assert(message);
	assert(header);
	
	e = cmime_list_head(message->headers);
	while(e != NULL) {
		h = (CMimeHeader_T *)cmime_list_data(e);
		if (strcasecmp(cmime_header_get_name(h),header)==0) {
			return(h);
		}
		e = e->next;
	}
	
	return(h);
}

int cmime_message_add_recipient(CMimeMessage_T *message, const char *recipient, CMimeAddressType_T t) {
	CMimeAddress_T * ca = NULL;
	CMimeListElem_T *elem;
	CMimeAddress_T *tca = NULL;
	char *s1 = NULL;
	char *s2 = NULL;
	
	assert(message);
	assert(recipient);

	// parse recipient string and create CMimeAddress_T object
	ca = cmime_address_parse_string(recipient);
	cmime_address_set_type(ca,t);
	if (message->recipients == NULL) {
		if (cmime_list_new(&message->recipients,_recipients_destroy)!=0) 
				return(-1);
	}	
	
	// check if given recipient already exists
	elem = cmime_list_head(message->recipients);
	while(elem != NULL) {
		tca = (CMimeAddress_T *)cmime_list_data(elem);
		
		if (cmime_address_get_type(tca) == t) {
			s1 = cmime_address_to_string(tca);
			s2 = cmime_address_to_string(ca);
			if (strcmp(s1,s2)==0) {
				cmime_list_remove(message->recipients,elem,NULL);
				break;
			}
			free(s1);
			free(s2);
		}
		elem = elem->next;
	}

	if (cmime_list_append(message->recipients,ca)!=0)
		return(-1);

	return(0);
}

void cmime_message_set_content_type(CMimeMessage_T *message, const char *s) {
	_cmime_internal_set_linked_header_value(message->headers,"Content-Type",s);
}

char *cmime_message_get_content_type(CMimeMessage_T *message) {
	return(_cmime_internal_get_linked_header_value(message->headers,"Content-Type"));
}

void cmime_message_set_content_transfer_encoding(CMimeMessage_T *message, const char *s) {
	_cmime_internal_set_linked_header_value(message->headers,"Content-Transfer-Encoding",s);
}

char *cmime_message_get_content_transfer_encoding(CMimeMessage_T *message) {
	return(_cmime_internal_get_linked_header_value(message->headers,"Content-Transfer-Encoding"));
}

void cmime_message_set_mime_version(CMimeMessage_T *message, const char *s) {
	_cmime_internal_set_linked_header_value(message->headers,"Mime-Version",s);
}

/* Return the full mime-version header, with possible comments */
char *cmime_message_get_mime_version(CMimeMessage_T *message) {
	return(_cmime_internal_get_linked_header_value(message->headers,"Mime-Version"));
}

void cmime_message_set_content_id(CMimeMessage_T *message, const char *s) { 
	_cmime_internal_set_linked_header_value(message->headers,"Content-ID",s);
}

char *cmime_message_get_content_id(CMimeMessage_T *message) {
	return(_cmime_internal_get_linked_header_value(message->headers,"Content-ID"));
}

void cmime_message_set_content_description(CMimeMessage_T *message, const char *s) {
	_cmime_internal_set_linked_header_value(message->headers,"Content-Description",s);
}

char *cmime_message_get_content_description(CMimeMessage_T *message) {
	return(_cmime_internal_get_linked_header_value(message->headers,"Content-Description"));
}

void cmime_message_set_date(CMimeMessage_T *message, const char *s) {
	_cmime_internal_set_linked_header_value(message->headers,"Date",s);
}

char *cmime_message_get_date(CMimeMessage_T *message) {
	return(_cmime_internal_get_linked_header_value(message->headers,"Date"));
}

int cmime_message_set_date_now(CMimeMessage_T *message) {
	time_t currtime;                                                    
	char s[128] = {0};     
	int i;
	
	assert(message);
	
	time(&currtime);                                                     
	i = strftime(s,sizeof(s),"%a, %d %b %Y %H:%M:%S %z",localtime(&currtime));
	if (i>0) {
		_cmime_internal_set_linked_header_value(message->headers,"Date",s);
		return(0);
	} else
		return(-1);
}

void cmime_message_set_boundary(CMimeMessage_T *message, const char *boundary) {
	assert(message);
	assert(boundary);
	
	if (message->boundary != NULL)
		free(message->boundary);
		
	message->boundary = strdup(boundary);
}

char *cmime_message_generate_boundary(void) {
	char str[21];
	char *boundary = NULL;
	int i;
	static const char text[] = "abcdefghijklmnopqrstuvwxyz"
	                           "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
														 "0123456789._-=";

	srand(time(NULL));

	for ( i = 0; i < 20; ++i ) {
		str[i] = text[rand() % (sizeof text - 1)];
	}
	str[20] = '\0';
	
	asprintf(&boundary,"--=_Part_%s",str);
	return(boundary);
}

/* extract boundary from given header string */
char *_get_boundary(char *s) {
	char *boundary = NULL;
	int pos = 0;
	
	s = strstr(s,"=");
	if (*++s=='"') 
		s++;

	boundary = (char *)calloc(strlen(s) + sizeof(char),sizeof(char));
	while(*s!='\0') {
		if ((*s!='"') && (*s!='\r') && (*s!='\n'))
			boundary[pos++] = *s;
		else {
			boundary[pos] = '\0';
			break;
		}
		s++;
	}
	
	return(boundary);
}

int cmime_message_from_file(CMimeMessage_T **message, const char *filename) {
	struct stat fileinfo;
	FILE *fp = NULL;
	char *buffer = NULL;
	size_t st = 0;
	int in_header = 1;
	char *s = NULL;
	char *ptemp = NULL;
	int in_part = 0;
	CMimePart_T *part = NULL;
	char *nl = NULL;
	
	assert((*message));
	assert(filename);
	
	if (stat(filename,&fileinfo) != 0)
		return(-1); /* stat error */
	
	if(!S_ISREG(fileinfo.st_mode))
		return(-2); /* not regular file */
	
	fp = fopen(filename, "rb");
	if (fp == NULL) {
		return(-3);
	}
	
	while(getline(&buffer,&st,fp) > 0) {
		// check newline frist
		if (nl==NULL)
			nl = _cmime_internal_determine_linebreak(buffer);
		
		if(strcmp(buffer,nl)==0) {
			if (in_header==1) {
				if (s!=NULL) {
					if (cmime_message_set_header((*message), s)!=0)
						return(-4); /* failed to add header */
					free(s);
					s = (char *)calloc(1,sizeof(char));
				}
				in_header = 0;
				continue;
			}
		}
		
		if (in_header==1) {
			// search for boundary
			if ((*message)->boundary==NULL) {
				ptemp = strcasestr(buffer,"boundary=");
				if (ptemp!=NULL) {
					(*message)->boundary = _get_boundary(ptemp);
				}
			}
			
			// process header
			if (isspace(buffer[0])) {
				/* we've got a long header field line, so append the value
				 * to the previous value */
				s = (char *)realloc(s,strlen(s) + st + sizeof(char));
				strcat(s,buffer);
			} else {
				if (s!=NULL) {
					if (cmime_message_set_header((*message), s)!=0)
						return(-4); /* failed to add header */
					free(s);
				} 

				s = (char *)calloc(st + sizeof(char), sizeof(char));
				strcat(s,buffer);
			}		
		} else {
			// process body
			if ((*message)->boundary!=NULL) {
				if (strstr(buffer,(*message)->boundary)) {
					if (in_part == 1) {
						cmime_part_from_string(&part,s);
						cmime_list_append((*message)->parts,part);
					}
					part = cmime_part_new();
					in_part = 1;
					free(s);
					s = (char *)calloc(1,sizeof(char));
					continue;
				}
			} 

			s = (char *)realloc(s,strlen(s) + st + 1);
			strcat(s,buffer);
		}
	}	
	fclose(fp);

	if ((*message)->boundary==NULL) {
		part = cmime_part_new();
		cmime_part_set_content(part,s);
		free(s);
		cmime_list_append((*message)->parts,part);
	}

	return(0);
}

char *cmime_message_to_string(CMimeMessage_T *message) {
	char *out = NULL;
	CMimeHeader_T *h = NULL;
	CMimeListElem_T *e = NULL;
	char *s = NULL;
	
	assert(message);
	
	e = cmime_list_head(message->headers);
	while(e != NULL) {
		h = (CMimeHeader_T *)cmime_list_data(e);
		s = cmime_header_to_string(h);
		printf("H: [%s]\n", s);
		free(s);
		e = e->next;
	}
	
	return(out);
}
