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
 * @file cmime_header.h
 * @brief Defines functions and structs for header management
 *
 * @example test_header.c
 */

#ifndef _CMIME_HEADER_H
#define _CMIME_HEADER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

/*!
 * @struct CMimeHeader_T cmime_header.h
 * @brief A email header item
 */
typedef struct {
	char *name; /**< name of the header */
	char **value; /**< list with header values */
	size_t count; /**< count of header appearance */
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
 * @fn int cmime_header_set_name(CMimeHeader_T *header, char *name)
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
 * @fn int cmime_header_set_value(CMimeHeader_T *header, char *value)
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
