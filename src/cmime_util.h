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
#include <time.h>
#include <unistd.h>

/*!
 * @struct CMimeInfo_T cmime_util.h
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

/*!
 * @fn int cmime_util_rand()
 * @brief Generate a random number from 0 to RAND_MAX
 * @returns a random integer from 0 to RAND_MAX
 */
int cmime_util_rand();

#ifdef __cplusplus
}
#endif

#endif	/* _CMIME_UTIL_H */
