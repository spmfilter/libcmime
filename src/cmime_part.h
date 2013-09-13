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
 * @file cmime_part.h
 * @brief Defines functions and structs for mime part handling
 */

#ifndef _CMIME_PART_H
#define _CMIME_PART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <assert.h>
#include <libgen.h>
#include <ctype.h>
#include <errno.h>

#include "cmime_list.h"

/*!
 * @struct CMimePart_T cmime_part.h
 * @brief A mime part
 */

typedef struct {
	CMimeList_T *headers; /**< Linked list with mime part headers */
	char *content; /**< content of mime part */
	char *boundary; /**< boundary for child parts */
    char *parent_boundary; /**< boundary of parent part */
	char *postface; /**< mime part postface */
    short int last; /**< last mime part */
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

/*!
 * @fn void cmime_part_set_postface(CMimePart_T *part, const char *s)
 * @brief Set mime parts postface
 * @param part a CMimePart_T object
 * @param s postface string
 */
void cmime_part_set_postface(CMimePart_T *part, const char *s);

/*!
 * @def cmime_part_get_postface(part)
 * @returns postface of mime part 
 */
#define cmime_part_get_postface(part) (part->postface);

/*!
 * @fn char *cmime_part_to_string(CMimePart_T *part, const char *nl)
 * @brief Return complete mime part as string, inclusive all mime headers
 * @param part a CMimePart_T object
 * @param nl newline character which should be used. If NULL newline
 *  character will be determined automatically.
 * @returns a newly allocated string with complete mime part
 */
char *cmime_part_to_string(CMimePart_T *part, const char *nl);

/*!
 * @fn int cmime_part_from_file(CMimePart_T **part, char *filename, const char *nl)
 * @brief Create a CMimePart_T object from file
 * @param part out param to return the new part
 * @param filename path to file
 * @param nl newline character which should be used. If NULL newline
 *  character will be determined automatically.
 * @returns 0 on success, -1 on stat error, -2 if not a regular file
 */
int cmime_part_from_file(CMimePart_T **part, char *filename, const char *nl);

/*!
 * @fn int cmime_part_to_file(CMimePart_T *part, char *filename)
 * @brief Create a file from CMimePart_T object
 * @param part CMimePart_T object
 * @param filename path to out file
 * @returns 0 on success, -1 on error
 */
int cmime_part_to_file(CMimePart_T *part, char *filename);

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
