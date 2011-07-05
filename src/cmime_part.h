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

/*!
 * @struct CMimePart_T cmime_part.h
 * @brief A mime part
 */

typedef struct {
	char *content_type; /**< content type of part */
	char *content_disposition; /**< content disposition of part */
	char *content_transfer_encoding; /**< content transfer encoding of part */
	char *content; /**< content of mime part */
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
 * @def cmime_part_get_content_type(part)
 * @returns mime parts content type
 */
#define cmime_part_get_content_type(part) (part->content_type);

/*!
 * @fn void cmime_part_set_content_disposition(CMimePart_T *part, const char *s)
 * @brief Set content disposition for mime part
 * @param part a CMimePart_T object
 * @param s content disosition string
 */
void cmime_part_set_content_disposition(CMimePart_T *part, const char *s);

/*!
 * @def cmime_part_get_content_disposition(part)
 * @returns mime parts content disposition
 */
#define cmime_part_get_content_disposition(part) (part->content_disposition);

/*!
 * @fn void cmime_part_set_content_transfer_encoding(CMimePart_T *part, const char *s)
 * @brief Set content transfer encoding for mime part
 * @param part a CMimePart_T object 
 * @param s content transfer encoding string
 */
void cmime_part_set_content_transfer_encoding(CMimePart_T *part, const char *s);

/*!
 * @def cmime_part_get_content_transfer_encoding(part)
 * @returns mime parts content transfer encoding
 */
#define cmime_part_get_content_transfer_encoding(part) (part->content_transfer_encoding)

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

/*!
 * @fn char *cmime_part_as_string(CMimePart_T *part)
 * @brief Return complete mime part as string
 * @param part a CMimePart_T object
 * @returns a newly allocated string with complete mime part
 */
char *cmime_part_as_string(CMimePart_T *part);

int cmime_part_from_file(CMimePart_T **part, char *filename);

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_PART_H */
