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
#include <assert.h>
#include <ctype.h>

#include "cmime_address.h"
#include "cmime_message.h"
#include "cmime_list.h"
#include "cmime_table.h"
#include "cmime_string.h"
#include "cmime_header.h"

static int _case_key_cmp(const void *x, const void *y) {
	return(strcasecmp((char *)x,(char *)y));
}

static unsigned _lower_str_hash(const void *x) {
	const char *str = x;
	unsigned h = 0;

	while (*str)
		h = (h<<1) + tolower(*str++);
	return h;
}

void _set_core_header_value(CMimeMessage_T *msg, const char *key, const char *value) {
	CMimeHeader_T *h = NULL;
	assert(msg);
	assert(key);
	assert(value);
	
	h = cmime_table_get(msg->headers,key);
	if (h != NULL) {
		cmime_header_set_value(h,value,1);
	} else {
		h = cmime_header_new();
		cmime_header_set_name(h,key);
		cmime_header_set_value(h,value,1);
		cmime_table_insert(msg->headers,cmime_header_get_name(h),h);
	}
}

char *_get_core_header_value(CMimeMessage_T *msg, const char *key) {
	CMimeHeader_T *h;
	char *v = NULL;
	h = cmime_table_get(msg->headers,key);
	if (h != NULL) {
		v = cmime_header_get_value(h,0);
		return(v);
	} else
		return(NULL);
}

void _header_destroy(void *data) {
	CMimeHeader_T *header = (CMimeHeader_T *)data;
	cmime_header_free(header);
}

void _recipients_destroy(void *data) {
	assert(data);
	CMimeAddress_T *ca = (CMimeAddress_T *)data;
	cmime_address_free(ca);
}

CMimeMessage_T *cmime_message_new(void) {
	CMimeMessage_T *message = NULL;
	
	message = (CMimeMessage_T *)calloc((size_t)1, sizeof(CMimeMessage_T));
	
	if (cmime_table_new(&message->headers,0,_case_key_cmp,_lower_str_hash,_header_destroy)!=0) 
		return(NULL);
	
	message->sender = NULL;
	if (cmime_list_new(&message->recipients,_recipients_destroy)!=0) 
			return(NULL);

	message->date = 0;
	message->tz_offset = 0;

	return(message);
}

/** Free a CMimeMessage_T object  */
void cmime_message_free(CMimeMessage_T *message) {
	assert(message);

	cmime_address_free(message->sender);	
	cmime_list_free(message->recipients);
	
	cmime_table_free(message->headers);
	
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
	_set_core_header_value(message,"Message-ID",mid);
}

char *cmime_message_get_message_id(CMimeMessage_T *message) {
	return(_get_core_header_value(message,"Message-ID"));
}

int cmime_message_set_header(CMimeMessage_T *message, const char *header) {
	CMimeStringList_T *sl;
	CMimeHeader_T *h;
	CMimeHeader_T *oh;
	char *k;
	char *v;
	assert(message);
	assert(header);
	
	sl = cmime_string_split(header,":");
	k = cmime_string_list_get(sl,0);
	k = cmime_string_strip(k);
	
	v = cmime_string_list_get(sl,1);
	v = cmime_string_strip(v);
	
	h = cmime_header_new();
	cmime_header_set_name(h,k);
	cmime_header_set_value(h,v,0);
	
	// check if header already exists
	if (cmime_table_get(message->headers,k) != NULL) {
		// header already exists, we'll overwrite it...
		if (cmime_table_remove(message->headers,k,(void **)&oh) != 0)
			return(-1);
		else 
			cmime_header_free(oh); 
	} 
	
	// insert new header object in hash table
	if (cmime_table_insert(message->headers, cmime_header_get_name(h), h)!=0)
		return(-1);

	cmime_string_list_free(sl);

	return(0);
}

CMimeHeader_T *cmime_message_get_header(CMimeMessage_T *message, const char *header) {
	CMimeHeader_T *h = NULL;
	
	assert(message);
	assert(header);
	
	h = (CMimeHeader_T *)cmime_table_get(message->headers,header);
	
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
	_set_core_header_value(message,"Content-Type",s);
}

char *cmime_message_get_content_type(CMimeMessage_T *message) {
	return(_get_core_header_value(message,"Content-Type"));
}

void cmime_message_set_content_transfer_encoding(CMimeMessage_T *message, const char *s) {
	_set_core_header_value(message,"Content-Transfer-Encoding",s);
}

char *cmime_message_get_content_transfer_encoding(CMimeMessage_T *message) {
	return(_get_core_header_value(message,"Content-Transfer-Encoding"));
}

void cmime_message_set_mime_version(CMimeMessage_T *message, const char *s) {
	_set_core_header_value(message,"Mime-Version",s);
}

/* Return the full mime-version header, with possible comments */
char *cmime_message_get_mime_version(CMimeMessage_T *message) {
	return(_get_core_header_value(message,"Mime-Version"));
}

void cmime_message_set_content_id(CMimeMessage_T *message, const char *s) { 
	_set_core_header_value(message,"Content-ID",s);
}

char *cmime_message_get_content_id(CMimeMessage_T *message) {
	return(_get_core_header_value(message,"Content-ID"));
}

void cmime_message_set_content_description(CMimeMessage_T *message, const char *s) {
	_set_core_header_value(message,"Content-Description",s);
}

char *cmime_message_get_content_description(CMimeMessage_T *message) {
	return(_get_core_header_value(message,"Content-Description"));
}
