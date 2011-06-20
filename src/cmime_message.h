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

/*!
 * @file cmime_message.h
 * @brief Defines functions and structs for message handling
 *
 * This header is intended for use by any library or application, not only libcmime.
 *
 * @example test_message.c
 */

#ifndef _CMIME_MESSAGE_H
#define _CMIME_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <time.h>

#include "cmime_address.h"
#include "cmime_list.h"
#include "cmime_table.h"

/*!
 * @struct CMimeMessage_T cmime_message.h
 * @brief Represents an email message
 */
typedef struct {
	CMimeAddress_T *sender; /**< sender of email */
	CMimeList_T *recipients; /**< double linked list with recipients */
	char *message_id; /**< message id of email */
	
	time_t date; /**< date of email */
	int tz_offset; /**< timezone offset */
} CMimeMessage_T;

/*!
 * @fn CMimeMessage_T *cmime_message_new(void)
 * @brief Creates a new CMimeMessage_T object
 * @returns CMimeMessage_T pointer, or NULL on failure
 */
CMimeMessage_T *cmime_message_new(void);

/*!
 * @fn void cmime_message_free(CMimeMessage_T *message)
 * @brief Free a CMimeMessage_T object 
 * @param ca CMimeMessage_T pointer
 */
void cmime_message_free(CMimeMessage_T *message);

/*!
 * @fn void cmime_message_set_sender(CMimeMessage_T *message, char *sender)
 * @brief Add sender to CMimeMessage_T object. If sender already exists,
 *   it will be overwritten.
 * @param message a CMimeMessate_T obect
 * @param sender sender string
 */
void cmime_message_set_sender(CMimeMessage_T *message, char *sender);


/*!
 * @def cmime_message_get_sender(message)
 * @returns sender of message as CMimeAddress_T object
 */
#define cmime_message_get_sender(message) (cmime_address_to_string(message->sender))


#ifdef __cplusplus
}
#endif

#endif /* _CMIME_MESSAGE_H */