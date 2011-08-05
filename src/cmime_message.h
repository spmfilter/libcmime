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
	CMimeList_T *headers;
	char *boundary; /**< message boundary */
	time_t date; /**< date of email */
	int tz_offset; /**< timezone offset */
	char *gap;
	CMimeList_T *parts; /**< mime parts */
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
char *cmime_message_get_message_id(CMimeMessage_T *message);

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

/*!
 * @fn int cmime_message_add_recipient(CMimeMessage_T *message, const char *recipient, CMimeAddressType_T t)
 * @brief Add recipient to message
 * @param message a CMimeMessage_T object
 * @param recipient a recipient string
 * @param t CMimeAddressType_T recipient type
 * @returns 0 on success or -1 in case of error
 */
int cmime_message_add_recipient(CMimeMessage_T *message, const char *recipient, CMimeAddressType_T t);

/*! 
 * @def cmime_message_get_recipients(message)
 * @returns CMimeList_T with recipients
 */
#define cmime_message_get_recipients(message) (message->recipients)

/*!
 * @fn void cmime_message_set_content_type(CMimeMessage_T *message, const char *t)
 * @brief Set Content-Type header
 * @param message a CMimeMessage_T object
 * @param s Content-Type string
 */
void cmime_message_set_content_type(CMimeMessage_T *message, const char *s);

/*!
 * @fn char *cmime_message_get_content_type(CMimeMessage_T *message)
 * @brief Get Content-Type header value
 * @param message a CMimeMessage_T object
 * @returns Content-Type header value
 */
char *cmime_message_get_content_type(CMimeMessage_T *message);

/*! 
 * @fn void cmime_message_set_content_transfer_encoding(CMimeMessage_T *message, const char *s)
 * @brief set Content-Transfer-Encoding value
 * @param message a CMimeMessage_T object
 * @param s Content-Transfer-Encoding value
 */
void cmime_message_set_content_transfer_encoding(CMimeMessage_T *message, const char *s);

/*!
 * @fn char *cmime_message_get_content_transfer_encoding(CMimeMessage_T *message)
 * @brief Get Content-Transfer-Encoding value
 * @param message a CMimeMessage_T object
 * @returns Content-Transfer-Encoding header value
 */
char *cmime_message_get_content_transfer_encoding(CMimeMessage_T *message);

/*!
 * @fn void cmime_message_set_content_id(CMimeMessage_T *message, const char *s)
 * @brief Set Content-ID header value
 * @param message a CMimeMessage_T object
 * @param s Content-ID header value
 */
void cmime_message_set_content_id(CMimeMessage_T *message, const char *s);

/*!
 * @fn char *cmime_message_get_content_id(CMimeMessage_T *message)
 * @brief Get Content-ID header value
 * @param message a CMimeMessage_T object
 * @returns Content-ID header value
 */
char *cmime_message_get_content_id(CMimeMessage_T *message);

/*!
 * @fn cmime_message_set_content_description(CMimeMessage_T *message, const char *s)
 * @brief Set Content-Description value
 * @param message a CMimeMessage_T object
 * @param s Content-Description header value
 */
void cmime_message_set_content_description(CMimeMessage_T *message, const char *s);

/*!
 * @fn char *cmime_message_get_content_description(CMimeMessage_T *message)
 * @brief Get Content-Description header value
 * @param message a CMimeMessage_T object
 */
char *cmime_message_get_content_description(CMimeMessage_T *message);

/*!
 * @fn void cmime_message_set_mime_version(CMimeMessage_T *message, const char *v)
 * @brief Set Mime-Version header value. According to RFC 2045, Mime-Version header 
 *   is required at the top level of a message. Not required for each body part of 
 *   a multipart entity. It's required for the embedded headers of a body of type 
 *   "message/rfc822" or "message/partial" if the embedded message is itself 
 *   claimed to be MIME.
 * @param message a CMimeMessage_T object
 * @param s Mime-Version header value
 */
void cmime_message_set_mime_version(CMimeMessage_T *message, const char *s);

/*!
 * @fn char *cmime_message_get_mime_version(CMimeMessage_T *message)
 * @brief Get Mime-Version header value
 * @param message a CMimeMessage_T object
 * @returns Mime-Version header value
 */
char *cmime_message_get_mime_version(CMimeMessage_T *message);

/*!
 * @fn void cmime_message_set_date(CMimeMessage_T *message, const char *s)
 * @brief Set date string header 
 * @param message a CMimeMessage_T object
 * @param s date string
 */
void cmime_message_set_date(CMimeMessage_T *message, const char *s);

/*!
 * @fn char *cmime_message_get_date(CMimeMessage_T *message)
 * @brief Get date string from CMimeMessage_T object
 * @param message a CMimeMessage_T object
 * @returns date string
 */
char *cmime_message_get_date(CMimeMessage_T *message);

/*!
 * @fn int cmime_message_set_date_now(CMimeMessage_T *message)
 * @brief Get current time and set date header
 * @param message a CMimeMessage_T object
 * @returns 0 on success or -1 in case of error
 */
int cmime_message_set_date_now(CMimeMessage_T *message);

/*!
 * @fn void cmime_message_set_boundary(CMimeMessage_T *message, char *boundary)
 * @brief Set message boundary
 * @param message a CMimeMessage_T object
 * @param boundary the boundary to set
 */
void cmime_message_set_boundary(CMimeMessage_T *message, const char *boundary);

/*! 
 * @def cmime_message_get_boundary(message)
 * @returns message boundary
 */
#define cmime_message_get_boundary(message) (message->boundary)

/*! 
 * @fn char *cmime_message_generate_boundary(void)
 * @brief Generate a message boundary
 * @returns a newly allocated boundary
 */
char *cmime_message_generate_boundary(void);

/*!
 * @fn int cmime_message_from_file(CMimeMessage_T **message, const char *filename)
 * @brief Parse given file and create a CMimeMessage_T object
 * @param message out param to return the new message object
 * @param filename path to message file
 * @returns 0 on success, -1 on stat error, -2 if not a regular file
 */
int cmime_message_from_file(CMimeMessage_T **message, const char *filename);

/*!
 * @fn char *cmime_message_to_string(CMimeMessage_T *message) 
 * @brief Return a message object as string
 * @param message a CMimeMessage_T object 
 * @returns message as newly allocated string
 */
char *cmime_message_to_string(CMimeMessage_T *message);

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_MESSAGE_H */