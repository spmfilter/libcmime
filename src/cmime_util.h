/* libcmime - A C mime library
 * Copyright (C) 2012 SpaceNet AG and Axel Steiner <ast@treibsand.com>
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
 * @file cmime_util.h
 * @brief Defines utility functions
 *
 */

#ifndef _CMIME_UTIL_H
#define	_CMIME_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

/*!
 * @struct CMimeInfo_T cmime_info.h
 * @brief Struct with mime informations
 */
typedef struct {
    char *mime_type; /**< a mime type */
    char *mime_encoding; /**< a mime encoding */
    char *combined; /**< combined mime type/encoding */
} CMimeInfo_T;

/*!
 * @fn char *cmime_util_get_mimetype(const char *filename)
 * @brief Get mimetype of file use the 'file' tool
 * @param filename path to file
 * @returns mimetype of file
 */
char *cmime_util_get_mimetype(const char *filename);

/*!
 * @fn CMimeInfo_T *cmime_util_info_new(void)
 * @brief Creates a new CMimeInfo_T object
 * @returns CMimeInfo_T pointer, or NULL on failure
 */
CMimeInfo_T *cmime_util_info_new(void);

/*!
 * @fn void cmime_util_info_free(CMimeInfo_T *mi)
 * @brief free a CMimeInfo_T object
 * @param mi a CMimeInfo_T object
 */
void cmime_util_info_free(CMimeInfo_T *mi);

/*!
 * @fn CMimeInfo_T *cmime_util_info_get_from_string(const char *s)
 * @brief Get mime type and encoding from given string
 * @param s a string to check
 * @returns a newly allocated CMimeInfo_T object, or NULL on failure
 */
CMimeInfo_T *cmime_util_info_get_from_string(const char *s);

/*!
 * @fn CMimeInfo_T *cmime_util_info_get_from_file(const char *filename)
 * @brief Get mime type and encoding from file
 * @param filename file to check
 * @returns a newly allocated CMimeInfo_T object, or NULL on failure
 */
CMimeInfo_T *cmime_util_info_get_from_file(const char *filename);

#ifdef __cplusplus
}
#endif

#endif	/* _CMIME_UTIL_H */
