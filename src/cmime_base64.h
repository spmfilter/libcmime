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
 * @file cmime_base64.h
 * @brief Base64 functions
 *
 * This header is intended for use by any library or application, not only libcmime.
 */

#ifndef _CMIME_BASE64_H
#define _CMIME_BASE64_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*! 
 * @fn void cmime_base64_encode_block(unsigned char in[3], unsigned char out[4], int len)
 * @brief Encode 3byte block to base64
 * @param in 3byte input
 * @param out 4byte output
 * @param len number of charactes in input
 */
void cmime_base64_encode_block(unsigned char in[3], unsigned char out[4], int len);

/*!
 * @fn void cmime_base64_decode_block(unsigned char in[4], unsigned char out[3])
 * @brief Decode 4 byte base64 block
 * @param in 4byte input
 * @param out 3byte output
 */
void cmime_base64_decode_block(unsigned char in[4], unsigned char out[3]);

/*!
 * @fn void cmime_base64_encode_file(FILE *infile, FILE *outfile, int linelen)
 * @brief Encode complete file to base64 and write to file pointer
 * @param infile input file pointer
 * @param outfile output file pointer
 * @param linelen maximum line length used when writing base64 to file
 */
void cmime_base64_encode_file(FILE *infile, FILE *outfile, int linelen);

/*!
 * @fn void cmime_base64_decode_file(FILE *infile, FILE *outfile)
 * @brief Decode complete file from base64 and write to file pointer
 * @param infile input file pointer
 * @param outfile output file pointer
 */
void cmime_base64_decode_file(FILE *infile, FILE *outfile);

/*!
 * @fn char *cmime_base64_encode_string(const char *source)
 * @brief Encode string to base64
 * @param source String to encode
 * @returns newly allocated char pointer with encoded string
 */
char *cmime_base64_encode_string(const char *source);

/*!
 * @fn char *cmime_base64_decode_string(const char *source)
 * @brief Decode base64 string
 * @param source base64 string to decode
 * @returns newly allocated chart pointer with decoded string
 */
char *cmime_base64_decode_string(const char *source);

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_BASE64_H */
