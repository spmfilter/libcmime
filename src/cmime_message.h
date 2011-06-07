/* libcmime - A C mime library
 * Copyright (C) 2010 Axel Steiner <ast@treibsand.com>
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


#ifndef _CMIME_MESSAGE_H
#define _CMIME_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

#include "cmime_list.h"
#include "cmime_address.h"

typedef enum _CMimeRecipientType {
	CMIME_RECIPIENT_TYPE_TO,
	CMIME_RECIPIENT_TYPE_CC,
	CMIME_RECIPIENT_TYPE_BCC,
} CMimeRecipientType;

/** A structure to represent an email message */
typedef struct {
	CMimeAddress_T *sender;
	CMimeList_T **recipients;
	char *subject;
	char *message_id;
	
	time_t date;
	int tz_offset;
} CMimeMessage_T;

CMimeMessage_T *cmime_message_new(void);

/** Free a CMimeMessage_T object 
 *
 * \param ca CMimeMessage_T pointer
 */
void cmime_message_free(CMimeMessage_T *message);

#ifdef __cplusplus
}
#endif

#endif