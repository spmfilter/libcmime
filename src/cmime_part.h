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
 * @file cmime_part.h
 * @brief Defines functions and structs for mime part handling
 *
 * ...
 *
 * @example test_part.c
 */

#ifndef _CMIME_PART_H
#define _CMIME_PART_H

#ifdef __cplusplus
extern "C" {
#endif

#include "cmime_list.h"

/*!
 * @struct CMimePart_T cmime_part.h
 * @brief A mime part
 */

typedef struct {
	CMimeList_T *headers;
	char *content; /**< content of mime part */
	char *boundary;
    char *parent_boundary;
//	CMimeList_T *parts; /**< mime parts */
	char *postface;
    short int last;
//    CMimeList_T *prev;
} CMimePart_T;

/*!
 * @fn CMimePart_T *cmime_part_new(void)
 * @brief Creates a new CMimePart_T object
 * @returns CMimePart_T pointer, or NULL on failure
 */
CMimePart_T *cmime_part_new(void);

/*!
 * @fn void cmime_part_free(CMimePart_T *part)
 * @brief Frees a CMimePart_T object
 * @param part a CMimePart_T object
 */
void cmime_part_free(CMimePart_T *part);

/*!
 * @fn void cmime_part_set_content_type(CMimePart_T *part, const char *s)
 * @brief Set the content type for a mime part
 * @param part a CMimePart_T object
 * @param s content type string
 */
void cmime_part_set_content_type(CMimePart_T *part, const char *s);

/*!
 * @fn char *cmime_part_get_content_type(CMimePart_T *part)
 * @brief Return content type of mime part
 * @param part a CMimePart_T object
 * @returns mime parts content type
 */
char *cmime_part_get_content_type(CMimePart_T *part);

/*!
 * @fn void cmime_part_set_content_disposition(CMimePart_T *part, const char *s)
 * @brief Set content disposition for mime part
 * @param part a CMimePart_T object
 * @param s content disosition string
 */
void cmime_part_set_content_disposition(CMimePart_T *part, const char *s);

/*!
 * @fn char *cmime_part_get_content_disposition(CMimePart_T *part)
 * @brief Return content disposition of mime part
 * @param part a CMimePart_T object
 * @returns mime parts content disposition
 */
char *cmime_part_get_content_disposition(CMimePart_T *part);

/*!
 * @fn void cmime_part_set_content_transfer_encoding(CMimePart_T *part, const char *s)
 * @brief Set content transfer encoding for mime part
 * @param part a CMimePart_T object 
 * @param s content transfer encoding string
 */
void cmime_part_set_content_transfer_encoding(CMimePart_T *part, const char *s);

/*!
 * @fn char *cmime_part_get_content_transfer_encoding(CMimePart_T *part)
 * @brief Return content transfer encoding of mime part
 * @param part a CMimePart_T object
 * @returns mime parts content transfer encoding
 */
char *cmime_part_get_content_transfer_encoding(CMimePart_T *part);

/*!
 * @fn void cmime_part_set_content_id(CMimePart_T *part, const char *s)
 * @brief Set content id for mime part
 * @param part a CMimePart_T object
 * @param s content id string
 */
void cmime_part_set_content_id(CMimePart_T *part, const char *s);

/*!
 * @fn char *cmime_part_get_content_id(CMimePart_T *part)
 * @brief Return content id of mime part
 * @param part a CMimePart_T object
 * @returns newly allocated char pointer with content id of mime part 
 */
char *cmime_part_get_content_id(CMimePart_T *part);

/*!
 * @fn void cmime_part_set_content(CMimePart_T *part, const char *s)
 * @brief Set content  for mime part
 * @param part a CMimePart_T object 
 * @param s mime part content
 */
void cmime_part_set_content(CMimePart_T *part, const char *s);

/*!
 * @def cmime_part_get_content(part)
 * @returns content of mime part
 */
#define cmime_part_get_content(part) (part->content);


void cmime_part_set_postface(CMimePart_T *part, const char *s);
#define cmime_part_get_postface(part) (part->postface);

/*!
 * @fn char *cmime_part_to_string(CMimePart_T *part)
 * @brief Return complete mime part as string
 * @param part a CMimePart_T object
 * @returns a newly allocated string with complete mime part
 */
char *cmime_part_to_string(CMimePart_T *part);

/*!
 * @fn int cmime_part_from_file(CMimePart_T **part, char *filename)
 * @brief Create a CMimePart_T object from file
 * @param pat out param to return the new part
 * @param filename path to file
 * @returns 0 on success, -1 on stat error, -2 if not a regular file
 */
int cmime_part_from_file(CMimePart_T **part, char *filename);

/*!
 * @fn int cmime_part_from_string(CMimePart_T **part, const char *content)
 * @brief Parse given string and create a new CMimePart_T object
 * @param part out param to return the new mime part object
 * @param content mime part string to parse
 * @returns 0 on success or -1 in case of error
 */
int cmime_part_from_string(CMimePart_T **part, const char *content);

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_PART_H */
