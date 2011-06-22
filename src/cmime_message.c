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

#include "cmime_address.h"
#include "cmime_message.h"
#include "cmime_list.h"
#include "cmime_table.h"

/*
static char *rfc822_headers[] = {
	"Return-Path",
	"Received",
	"Date",
	"From",
	"Reply-To",
	"Subject",
	"Sender",
	"To",
	"Cc",
};
*/
//#define N_RECIPIENT_TYPES 3

void recipients_destroy(void *data) {
	assert(data);
	CMimeAddress_T *ca = (CMimeAddress_T *)data;
	cmime_address_free(ca);
}

CMimeMessage_T *cmime_message_new(void) {
	CMimeMessage_T *message = NULL;
	
	message = (CMimeMessage_T *)calloc((size_t)1, sizeof(CMimeMessage_T));
	
	if (cmime_table_new(&message->headers,0,NULL,NULL)!=0) 
		return(NULL);
	
	message->sender = NULL;
	if (cmime_list_new(&message->recipients,recipients_destroy)!=0) 
			return(NULL);

	message->date = 0;
	message->tz_offset = 0;
	message->message_id = NULL;

	return(message);
}

/** Free a CMimeMessage_T object  */
void cmime_message_free(CMimeMessage_T *message) {
	assert(message);
	if (message->message_id != NULL)
		free(message->message_id);

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
	assert(message);
	assert(mid);
	
	if (message->message_id != NULL) 
		free(message->message_id);
		
	message->message_id = (char *)malloc(strlen(mid) + 1);
	strcpy(message->message_id,mid);
}

int cmime_message_set_header(CMimeMessage_T *message, const char *header) {
	assert(message);
	assert(header);
	
	
	
	return(0);
}