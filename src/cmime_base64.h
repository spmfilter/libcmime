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
 * @file cmime_base64.h
 * @brief Base64 functions
 *
 * This header is intended for use by any library or application, not only libcmime.
 *
 * @example test_base64.c
 */

#ifndef _CMIME_BASE64_H
#define _CMIME_bASE64_H

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
