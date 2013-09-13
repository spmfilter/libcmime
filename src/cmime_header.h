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
 * @file cmime_header.h
 * @brief Defines functions and structs for header management
 */

#ifndef _CMIME_HEADER_H
#define _CMIME_HEADER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*!
 * @struct CMimeHeader_T cmime_header.h
 * @brief A email header item
 */
typedef struct {
	char *name; /**< name of the header */
	char **value; /**< list with header values */
	size_t count; /**< count of header appearance */
    int parsed; /**< flag which indicates, if the object has been parsed */
} CMimeHeader_T;


/*!
 * @fn CMimeHeader_T *cmime_header_new(void)
 * @brief Creates a new CMimeHeader_T object
 * @returns CMimeHeader_T pointer, or NULL on failure
 */
CMimeHeader_T *cmime_header_new(void);

/*!
 * @fn void cmime_header_free(CMimeHeader_T *header)
 * @brief Free a CMimeHeader_T object 
 * @param header CMimeHeader_T pointer
 */
void cmime_header_free(CMimeHeader_T *header);

/*!
 * @fn int cmime_header_set_name(CMimeHeader_T *header, const char *name)
 * @brief Set a header name, if aleready exists, name will be overwritten
 * @param header CMimeHeader_T pointer
 * @param name name of header
 */
void cmime_header_set_name(CMimeHeader_T *header, const char *name);

/*!
 * @def cmime_header_get_name(header)
 * @returns header name
 */
#define cmime_header_get_name(header) (header->name)


/*!
 * @fn void cmime_header_set_value(CMimeHeader_T *header, const char *value, int overwrite)
 * @brief Set/append a header value
 * @param header CMimeHeader_T pointer
 * @param value header value
 * @param overwrite 1 will overwrite value, 0 append
 */
void cmime_header_set_value(CMimeHeader_T *header, const char *value, int overwrite);

/*!
 * @fn char *cmime_header_get_value(CMimeHeader_T *header, int pos)
 * @brief Return header value (at given position, if header appears more than once)
 * @param header CMimeHeader_T pointer
 * @param pos Position of appearance in email
 * @returns header value
 */
char *cmime_header_get_value(CMimeHeader_T *header,int pos);

/*! 
 * @def cmime_header_get_count(header)
 * @returns header value count
 */
#define cmime_header_get_count(header) ((int)header->count)

/*!
 * @fn char *cmime_header_to_string(CMimeHeader_T *header)
 * @brief Return full header as newly allocated string
 * @param header a CMimeHeader_T object
 * @returns full header as newly allocated string
 */
char *cmime_header_to_string(CMimeHeader_T *header);

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_HEADER_H */
