/* libcmime - A C mime library
 * Copyright (C) 2013 SpaceNet AG and Axel Steiner <ast@treibsand.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*!
 * @file cmime_message.h
 * @brief Defines functions and structs for message handling
 *
 * @example append_and_prepend_subject.c
 * @example create_mime_message.c
 * @example create_mime_message_part_from_file.c
 * @example create_mime_message_skeleton.c
 * @example create_mime_message_skeleton_add_attachment.c
 * @example parse_mime_message.c
 * @example save_attachment_to_file.c
 *
 */

#ifndef _CMIME_MESSAGE_H
#define _CMIME_MESSAGE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/param.h>
#include <errno.h>

#include "cmime_address.h"
#include "cmime_list.h"
#include "cmime_header.h"
#include "cmime_part.h"
#include "cmime_util.h"
#include "cmime_string.h"

/*!
 * @enum CMimeBoundaryType_T 
 * @brief Possible types of boundaries
 */
typedef enum _BoundaryType {
    CMIME_BOUNDARY_OPEN, /**< indicates an opening boundary */
    CMIME_BOUNDARY_CLOSE, /**< indicates an closing boundary */
} CMimeBoundaryType_T;

/*!
 * @struct CMimeMessage_T cmime_message.h
 * @brief Represents an email message
 */
typedef struct {
    CMimeAddress_T *sender; /**< sender of email */
    CMimeList_T *recipients; /**< double linked list with recipients */
    CMimeList_T *headers; /**< message header */
    char *boundary; /**< message boundary */
    char *gap; /**< gap between headers an mime parts */
    CMimeList_T *parts; /**< mime parts */
    char *linebreak; /**< linebreak used by message */
    CMimeStringList_T *boundaries; /**< list with all appearing boundaries */ 
} CMimeMessage_T;

/*!
 * @enum CMimeMultipartType_T 
 * @brief Possible multipart mime subtypes
 */
typedef enum _CMimeMultipartType {
    CMIME_MULTIPART_MIXED, /**< multipart/mixed */
    CMIME_MULTIPART_DIGEST, /**< multipart/digest */
    CMIME_MULTIPART_MESSAGE, /**< message/rfc822 */
    CMIME_MULTIPART_ALTERNATIVE, /**< multipart/alternative */
    CMIME_MULTIPART_RELATED /**< multipart/related */
} CMimeMultipartType_T;

/*!
 * @fn CMimeMessage_T *cmime_message_new(void)
 * @brief Creates a new CMimeMessage_T object
 * @returns CMimeMessage_T pointer, or NULL on failure
 */
CMimeMessage_T *cmime_message_new(void);

/*!
 * @fn void cmime_message_free(CMimeMessage_T *message)
 * @brief Free a CMimeMessage_T object 
 * @param message CMimeMessage_T pointer
 */
void cmime_message_free(CMimeMessage_T *message);

/*!
 * @fn void cmime_message_set_sender(CMimeMessage_T *message, const char *sender)
 * @brief Add sender to CMimeMessage_T object. If sender already exists,
 *   it will be overwritten.
 * @param message a CMimeMessage_T obect
 * @param sender sender string
 */
void cmime_message_set_sender(CMimeMessage_T *message, const char *sender);

/*!
 * @fn void cmime_message_set_sender_encode(CMimeMessage_T *message, const char *sender)
 * @brief Add sender to CMimeMessage_T object and convert to 7 bit mime encoded string 
 *   based on RFC 2047. If sender already exists, it will be overwritten.
 * @param message a CMimeMessage_T obect
 * @param sender sender string
 */
void cmime_message_set_sender_encode(CMimeMessage_T *message, const char *sender);

/*!
 * @fn char *cmime_message_get_sender_string(CMimeMessage_T *message)
 * @brief Get sender of a CMimeMessage_T object as string
 * @param message a CMimeMessage_T object
 * @returns sender of message as CMimeAddress_T object
 */
//#define cmime_message_get_sender(message) (cmime_address_to_string(message->sender))
char *cmime_message_get_sender_string(CMimeMessage_T *message);

/*!
 * @def cmime_message_get_sender(message)
 * @returns sender of CMimeMessage_T object
 */
#define cmime_message_get_sender(message) (message->sender)

/*!
 * @fn void cmime_message_set_message_id(CMimeMessage_T *message, const char *mid)
 * @brief Set message id to CMimeMessage_T object, if alreay exists, value will 
 *   be overwritten.
 * @param message a CMimeMessage_T object
 * @param mid message id string
 */
void cmime_message_set_message_id(CMimeMessage_T *message, const char *mid);

/*!
 * @fn char *cmime_message_get_message_id(CMimeMessage_T *message)
 * @brief Get MessageID of message object
 * @param message a CMimeMessage_T object
 * @returns message id of CMimeMessage_T object
 */
char *cmime_message_get_message_id(CMimeMessage_T *message);

/*!
 * @fn char *cmime_message_generate_message_id(void)
 * @brief Generate a message id
 * @returns a newly allocated message id string
 */
char *cmime_message_generate_message_id(void);

/*!
 * @fn void cmime_message_add_generated_message_id(CMimeMessage_T *message)
 * @brief Add a newly generated message id to a CMimeMessage_T object
 * @param message a CMimeMessage_T object
 */
void cmime_message_add_generated_message_id(CMimeMessage_T *message);

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
 * @param header name of header to search for
 * @returns a CMImeHeader_T object, or NULL in case of error
 */
CMimeHeader_T *cmime_message_get_header(CMimeMessage_T *message, const char *header);

/*! 
 * @def cmime_message_get_headers(message)
 * @returns CMimeList_T with headers
 */
#define cmime_message_get_headers(message) (message->headers)

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
 * @fn int cmime_message_add_recipient_encode(CMimeMessage_T *message, const char *recipient, CMimeAddressType_T t)
 * @brief Add recipient to message and convert to 7 bit mime encoded string based on RFC 2047.
 * @param message a CMimeMessage_T object
 * @param recipient a recipient string
 * @param t CMimeAddressType_T recipient type
 * @returns 0 on success or -1 in case of error
 */
int cmime_message_add_recipient_encode(CMimeMessage_T *message, const char *recipient, CMimeAddressType_T t);

/*!
 * @fn int cmime_message_add_recipient_to(CMimeMessage_T *message, const char *recipient)
 * @brief Add To recipient to message
 * @param message a CMimeMessage_T object
 * @param recipient a recipient string
 * @returns 0 on success or -1 in case of error
 */
int cmime_message_add_recipient_to(CMimeMessage_T *message, const char *recipient);

/*!
 * @fn int cmime_message_add_recipient_cc(CMimeMessage_T *message, const char *recipient)
 * @brief Add Cc recipient to message
 * @param message a CMimeMessage_T object
 * @param recipient a recipient string
 * @returns 0 on success or -1 in case of error
 */
int cmime_message_add_recipient_cc(CMimeMessage_T *message, const char *recipient);

/*!
 * @fn int cmime_message_add_recipient_bcc(CMimeMessage_T *message, const char *recipient)
 * @brief Add Bcc recipient to message
 * @param message a CMimeMessage_T object
 * @param recipient a recipient string
 * @returns 0 on success or -1 in case of error
 */
int cmime_message_add_recipient_bcc(CMimeMessage_T *message, const char *recipient);

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
 * @fn void cmime_message_set_boundary(CMimeMessage_T *message, const char *boundary)
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
 * @fn void cmime_message_add_generated_boundary(CMimeMessage_T *message)
 * @brief Add a newly generated boundary to a CMimeMessage_T object
 * @param message a CMimeMessage_T object
 */
void cmime_message_add_generated_boundary(CMimeMessage_T *message);

/*!
 * @fn int cmime_message_from_file(CMimeMessage_T **message, const char *filename, int header_only)
 * @brief Parse given file and create a CMimeMessage_T object
 * @param message out param to return the new message object
 * @param filename path to message file
 * @param header_only parse only message headers, 1 = tue, 0 = false
 * @returns 0 on success, -1 on stat error, -2 if not a regular file, -3 if reading fails, 
 *   1 if parsing failed because of invalid input, 2 if parsing failed due to memory exhaustion
 */
int cmime_message_from_file(CMimeMessage_T **message, const char *filename, int header_only);

/*!
 * @fn int cmime_message_to_file(CMimeMessage_T *message, const char *filename)
 * @brief Write CMimeMessage_T object to file
 * @param message a CMimeMessage_T object
 * @param filename path to file, which should be written
 * @returns the number of items successfully written on success, -1 in case of error
 */
int cmime_message_to_file(CMimeMessage_T *message, const char *filename);

/*!
 * @fn char *cmime_message_to_string(CMimeMessage_T *message) 
 * @brief Return a message object as string
 * @param message a CMimeMessage_T object 
 * @returns message as newly allocated string
 */
char *cmime_message_to_string(CMimeMessage_T *message);

/*!
 * @fn int cmime_message_from_string(CMimeMessage_T **message, const char *content, int header_only)
 * @brief Parse given string and create a new CMimeMessage_T object
 * @param message out param to return the new message object
 * @param content message string to parse
 * @param header_only parse only message headers, 1 = tue, 0 = false
 * @returns 0 on success or -1 in case of error, 1 if parsing failed because of invalid input, 
 *   2 if parsing failed due to memory exhaustion
 */
int cmime_message_from_string(CMimeMessage_T **message, const char *content, int header_only);

/*!
 * @fn cmime_message_part_count(message)
 * @brief Return number of mime parts
 * @param message a CMimeMessage_T obejct
 * @returns number of mime parts
 */
#define cmime_message_part_count(message) (message->parts->size)

/*!
 * @fn void cmime_message_set_subject(CMimeMessage_T *message, const char *subject)
 * @brief Set message subject
 * @param message a CMimeMessage_T object
 * @param s the subject string
 */
void cmime_message_set_subject(CMimeMessage_T *message, const char *s);

/*!
 * @fn void cmime_message_set_subject_encode(CMimeMessage_T *message, const char *subject)
 * @brief Set message subject and convert to 7 bit mime encoded string based on RFC 2047.
 * @param message a CMimeMessage_T object
 * @param s the subject string
 */
void cmime_message_set_subject_encode(CMimeMessage_T *message, const char *s);

/*!
 * @fn char *cmime_message_get_subject(CMimeMessage_T *message)
 * @brief Get the subject string from CMimeMessage_T object
 * @param message a CMimeMessage_T object
 * @returns subject string to append
 */
char *cmime_message_get_subject(CMimeMessage_T *message);

/*!
 * @fn void cmime_message_prepend_subject(CMimeMessage_T *message, const char *s)
 * @brief prepend string to subject
 * @param message a CMimeMessage_T object
 * @param s string to prepend
 */
void cmime_message_prepend_subject(CMimeMessage_T *message, const char *s);

/*!
 * @fn void cmime_message_append_subject(CMimeMessage_T *message, const char *s)
 * @brief append string to subject
 * @param message a CMimeMessage_T object
 * @param s a append string for subject
 */
void cmime_message_append_subject(CMimeMessage_T *message, const char *s);

/*!
 * @fn int cmime_message_set_body(CMimeMessage_T *message, const char *body)
 * @brief Set plain body to non multipart message.
 * @param message a CMimeMessage_T object
 * @param content body content
 * @returns 0 on sucess, -1 if message is multipart
 */
int cmime_message_set_body(CMimeMessage_T *message, const char *content);

/*!
 * @fn int cmime_message_append_part(CMimeMessage_T *message, CMimePart_T *part)
 * @brief Append mime part to message object and generate boundary if necessary
 * @param message a CMimeMessage_T object
 * @param part a CMimePart_T part
 * @returns 0 on success, -1 in case of error
 */
int cmime_message_append_part(CMimeMessage_T *message, CMimePart_T *part);

/*!
 * @fn void cmime_message_add_attachment (CMimeMessage_T *message, char *filename)
 * @brief add attachment to message 
 * @param message a CMimeMessage_T object
 * @param attachment a file pointer
 */
void cmime_message_add_attachment(CMimeMessage_T *message, char *attachment);

/*!
 * @fn CMimeMessage_T *cmime_message_create_skeleton( const char *sender, const char *recipient, const char *subject)
 * @brief create message skeleton with basic header information
 * @param sender a from sender
 * @param recipient a to recipient
 * @param subject a subject string
 * @returns CMimeMessage_T pointer
 */
CMimeMessage_T *cmime_message_create_skeleton(const char *sender, const char *recipient, const char *subject);

/*!
 * @fn int cmime_message_part_remove(CMimeMessage_T *message, CMimePart_T *part)
 * @brief Remove a mime part from message
 * @param message a CMimeMessage_T object
 * @param part the mime part which should be removed
 * @returns 0 on success, -1 in case of error
 */ 
int cmime_message_part_remove(CMimeMessage_T *message, CMimePart_T *part);

/*! 
 * @fn int cmime_message_add_child_part(CMimeMessage_T *message, CMimePart_T *part, CMimePart_T *child, CMimeMultipartType_T subtype)
 * @brief Add a child part to given mimepart, set content type and generate a boundary if necessary.
 * @param message a CMimeMessage_T object
 * @param part the parent mime part
 * @param child the child mime part, which should be added
 * @param subtype the multipart subtype
 * @returns 0 on success or -1 in case of error
 */
int cmime_message_add_child_part(CMimeMessage_T *message, CMimePart_T *part, CMimePart_T *child, CMimeMultipartType_T subtype);

/*!
 * @def cmime_message_part_first(message)
 * @returns returns the first mime part of message
 */
#define cmime_message_part_first(message) ((CMimePart_T *)cmime_list_head(message->parts)->data)

/*!
 * @def cmime_message_part_last(message)
 * @returns returns the last mime part of message
 */
#define cmime_message_part_last(message) ((CMimePart_T *)cmime_list_tail(message->parts)->data)

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_MESSAGE_H */

