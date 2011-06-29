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
 * @brief tbd
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

#define HEADER_CONTENT_TYPE "Content-Type"
#define HEADER_CONTENT_DISPOSITION "Content-Disposition"
#define HEADER_CONTENT_TRANSFER_ENCODING "Content-Transfer-Encoding"

#define DEFAULT_CONTENT_TYPE "text/plain"
#define DEFAULT_CONTENT_DISPOSITION "inline"

typedef struct {
	char *content_type;
	char *content_disposition;
	char *content_transfer_encoding;
	char *content;
	
} CMimePart_T;

CMimePart_T *cmime_part_new(void);
void cmime_part_free(CMimePart_T *part);

void cmime_part_set_content_type(CMimePart_T *part, const char *s);
#define cmime_part_get_content_type(part) (part->content_type);

void cmime_part_set_content_disposition(CMimePart_T *part, const char *s);
#define cmime_part_get_content_disposition(part) (part->content_disposition);

void cmime_part_set_content_transfer_encoding(CMimePart_T *part, const char *s);
#define cmime_part_get_content_transfer_encoding(part) (part->content_transfer_encoding)

void cmime_part_set_content(CMimePart_T *part, const char *s);
#define cmime_part_get_content(part) (part->content);

char *cmime_part_as_string(CMimePart_T *part);

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_PART_H */
