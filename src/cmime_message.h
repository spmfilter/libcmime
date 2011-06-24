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

/*!
 * @file cmime_message.h
 * @brief Defines functions and structs for message handling
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
#include "cmime_header.h"

/*!
 * @struct CMimeMessage_T cmime_message.h
 * @brief Represents an email message
 */
typedef struct {
	CMimeAddress_T *sender; /**< sender of email */
	CMimeList_T *recipients; /**< double linked list with recipients */
	CMimeTable_T *headers; /**< message header hash table */
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
void cmime_message_set_sender(CMimeMessage_T *message, const char *sender);

/*!
 * @def cmime_message_get_sender(message)
 * @returns sender of message as CMimeAddress_T object
 */
#define cmime_message_get_sender(message) (cmime_address_to_string(message->sender))

/*!
 *
 * @fn void cmime_message_set_message_id(CMimeMessage_T *message, const char *mid)
 * @brief Set message id to CMimeMessage_T object, if alreay exists, value will 
 *   be overwritten.
 * @param message a CMimeMessage_T object
 * @param mid message id string
 */
void cmime_message_set_message_id(CMimeMessage_T *message, const char *mid);

/*!
 * @def cmime_message_get_message_id(message)
 * @returns message id of CMimeMessage_T object
 */
#define cmime_message_get_message_id(message) (message->message_id);

/*!
 * @fn int cmime_message_set_header(CMimeMessage_T *message, const char *header)
 * @brief Set a header to message object. If header already exists, it will be overwritten
 * @param message a CMimeMessage_T object
 * @param header full header string
 * @returns 0 on success or -1 in case of error
 */
int cmime_message_set_header(CMimeMessage_T *message, const char *header);

/*!
 * @fn CMimeHeader_T *cmime_message_get_header(CMimeMessage_T *message, const char *header)
 * @brief Get header for given key
 * @param message a CMimeMessage_T object
 * @param key key to search for
 * @returns a CMImeHeader_T object, or NULL in case of error
 */
CMimeHeader_T *cmime_message_get_header(CMimeMessage_T *message, const char *header);

int cmime_message_add_recipient(CMimeMessage_T *message, const char *recipient, CMimeAddressType_T t);

#define cmime_message_get_recipients(message) (message->recipients)

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_MESSAGE_H */