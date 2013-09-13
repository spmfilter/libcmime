/* libcmime - A C mime library
 * Copyright (C) 2013 SpaceNet AG
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
 * @file cmime_qp.h
 * @brief quoted-printable functions
 */


#ifndef _CMIME_QP_H
#define _CMIME_QP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*!
 * @fn char *cmime_qp_decode(char *line_in, int mode, char esc_char)
 * @brief decode string from quoted-printable 
 * @param line_in String to decode
 * @param mode DECODE_QP_MODE_DEFAULT=0 DECODE_QP_MODE_ISO=1
 * @param esc_char = or %
 * @returns newly allocated char pointer with decoded string
 */
char *cmime_qp_decode(char *line_in, int mode, char esc_char);

/*!
 * @fn *cmime_qp_decode_text (char *line_in);
 * @brief helper function to decode from quoted-printable
 * @param line_in String to decode
 * @returns newly allocated char pointer with decoded string
 */
char *cmime_qp_decode_text (char *line_in);

/*!
 * @fn *cmime_qp_decode_iso (char *line_in);
 * @brief helper function to decode from quoted-printable with ISO
 * @param line_in String to decode
 * @returns newly allocated char pointer with decoded string
 */
char *cmime_qp_decode_iso (char *line_in);

/*!
 * @fn char *cmime_qp_decode_multipart ( char *line_in);
 * @brief helper function to decode multipart from quoted-printable
 * @param line_in String to decode
 * @returns newly allocated char pointer with decoded string
 */
char *cmime_qp_decode_multipart (char *line_in);

/*!
 * @fn char *cmime_qp_encode(char *line_in, char *lt);
 * @brief function to encode string to quoted-printable
 * @param line_in String to encode
 * @param lt line terminator (NULL triggers lt to be CRLF)
 * @returns newly allocated char pointer with encoded string
 */
char *cmime_qp_encode(char *line_in, char *lt);

/*!
 * @fn char *cmime_qp_rm_charenc(char *line_in);
 * @brief function to remove charset encoding from string
 * @param line_in String to clean
 * @returns newly allocated char pointer with clean string
 */
char *cmime_qp_rm_charenc(char *line_in);

#ifdef __cplusplus
}
#endif
#endif /* _CMIME_QP_H */
