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
 * @file cmime_base64.h
 * @brief Base64 functions
 *
 * ...
 *
 * @example test_base64.c
 */

#ifndef _CMIME_BASE64_H
#define _CMIME_bASE64_H

#ifdef __cplusplus
extern "C" {
#endif

void cmime_base64_encode_block(unsigned char in[3], unsigned char out[4], int len);

void cmime_base64_decode_block(unsigned char in[4], unsigned char out[3]);

void cmime_base64_encode_file(FILE *infile, FILE *outfile, int linelen);

void cmime_base64_decode_file(FILE *infile, FILE *outfile);

char *cmime_base64_encode_string(const char *source);

char *cmime_base64_decode_string(const char *source);

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_BASE64_H */
