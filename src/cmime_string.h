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
 * @file cmime_string.h
 * @brief Defines helper functions and structs for string handling
 */

#ifndef _CMIME_STRING_H
#define _CMIME_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

/*!
 * @struct CMimeStringList_T cmime_string.h
 * @brief A simple list with strings
 */
typedef struct {
    char **node; /**< list with string values */
    size_t count; /**< count of strings in list */
} CMimeStringList_T;

/*!
 * @enum CMimeStringEncodingType_T
 * @brief Possible string encoding types
 */
typedef enum _CMimeStringEncodingType_T {
  CMIME_STRING_ENCODING_B64, /**< Base64 encoding */
  CMIME_STRING_ENCODING_QP, /**< Quoted-Printable encoding */
} CMimeStringEncodingType_T;


/*!
 * @fn CMimeStringList_T *cmime_string_list_new(void)
 * @brief Creates a new CMimeStringList_T object
 * @returns CMimeStringList_T pointer, or NULL on failure
 */
CMimeStringList_T *cmime_string_list_new(void);

/*!
 * @fn void cmime_string_list_free(CMimeStringList_T *sl)
 * @brief Free a CMimeStringList_t object 
 * @param sl CMimeStringList_T pointer
 */
void cmime_string_list_free(CMimeStringList_T *sl);

/*!
 * @fn void cmime_string_list_insert(CMimeStringList_T *sl, const char *s)
 * @brief Insert new string into list
 * @param sl CMimeStringList_T pointer
 * @param s a string
 */
void cmime_string_list_insert(CMimeStringList_T *sl, const char *s);

/*!
 * @fn char *cmime_string_list_get(CMimeStringList_T *sl, int pos)
 * @brief Return string at given position
 * @param sl CMimeStringList_T pointer
 * @param pos String position in list
 * @returns string value
 */
char *cmime_string_list_get(CMimeStringList_T *sl, int pos);

/*! 
 * @def cmime_string_list_get_count(sl)
 * @returns string list count
 */
#define cmime_string_list_get_count(sl) ((int)sl->count)

/*!
 * @fn char *cmime_string_strip(char *s)
 * @brief Strip leading/trailing spaces from string. 
 *   This function doesn't allocate or reallocate any memory; 
 *   it modifies string in place. 
 * @param s string with spaces
 * @returns string with removed spaces
 */
char *cmime_string_strip(char *s);

/*!
 * @fn char *cmime_string_chomp(char *s)
 * @brief remove trailing newline from string
 *   This function doesn't allocate or reallocate any memory;
 *   it modifies string in place.
 * @param s a string
 * @returns string with removed newline
 */
char *cmime_string_chomp(char *s);

/*!
 * int cmime_string_is_7bit(const char *s)
 * @brief Determine if given string is 7bit only or not
 * @param s string to check
 * @returns 0 if  string is 7bit, otherwise -1
 */
int cmime_string_is_7bit(const char *s);

/*!
 * int cmime_string_is_8bit(const char *s)
 * @brief Determine if given string contains 8bit characters
 * @param s string to check
 * @returns 0 if string contains 8bit characters, otherwise -1
 */
int cmime_string_is_8bit(const char *s);

/*!
 * char 
 * @brief Convert given string to 7 bit mime encoded string based on RFC 2047
 * @param s string to encode
 * @param t CMimeStringEncodingType_T encoding
 * @returns a newly allocated encoded string
 */ 
char *cmime_string_encode_to_7bit(const char *s, CMimeStringEncodingType_T t);

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_STRING_H */
