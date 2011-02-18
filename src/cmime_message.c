/* libcmime - A C mime library
 * Copyright (C) 2010 Axel Steiner
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

#include "cmime_message.h"
#include "cmime_list.h"

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

#define N_RECIPIENT_TYPES 3

CMimeMessage_T *cmime_message_new(void) {
	CMimeMessage_T *message;
	int i;
	
	message = (CMimeMessage_T *)calloc((size_t)1, sizeof(CMimeMessage_T));
	
	message->sender = NULL;
	message->recipients = calloc((size_t)N_RECIPIENT_TYPES, sizeof(CMimeList_T));
	message->subject = NULL;
	message->date = 0;
	message->tz_offset = 0;
	message->message_id = NULL;

	/* initialize recipient lists */
	for (i = 0; i < N_RECIPIENT_TYPES; i++) {
//		message->recipients[i] = cmime_list_new();
	}

	return(message);
}

/* cleanup a cmime_list whith CMimeAddress_T pointers */
void cmime_message_clear_recipients(void *data) {
	CMimeAddress_T *ca = (CMimeAddress_T *)data;

	if(ca != NULL)
		cmime_address_free(ca);
}

/** Free a CMimeMessage_T object  */
void cmime_message_free(CMimeMessage_T *message) {
	int i;
	for (i = 0; i < N_RECIPIENT_TYPES; i++) {
	//	cmime_list_free(message->recipients[i], cmime_message_clear_recipients);
	}
	
	free(message);
}
