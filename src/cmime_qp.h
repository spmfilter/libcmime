/* libcmime - A C mime library
 * Copyright (C) 2012 SpaceNet AG
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
