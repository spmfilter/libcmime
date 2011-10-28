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

#ifndef _CMIME_INTERNAL_H
#define _CMIME_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "cmime_list.h"
#include "cmime_message.h"
#include "cmime_header.h"

#define PART_CONTENT_TYPE_PATTERN "Content-Type:"
#define PART_CONTENT_DISPOSITION_PATTERN "Content-Disposition:"
#define PART_CONTENT_TRANSFER_ENCODING_PATTERN "Content-Transfer-Encoding:"
#define PART_CONTENT_ID_PATTERN "Content-ID:"

#define MIMETYPE_DEFAULT "application/octet-stream"
#define MIMETYPE_TEXT_PLAIN "text/plain"

#define LINE_LENGTH 72

/** crlf */
#define CRLF "\r\n"
#define DCRLF "\r\n\r\n"
#define LF "\n"
#define CR "\r"

void _cmime_internal_header_destroy(void *data);

char *_cmime_internal_determine_linebreak(const char *s);

void _cmime_internal_set_linked_header_value(CMimeList_T *l, const char *key, const char *value);
char *_cmime_internal_get_linked_header_value(CMimeList_T *l, const char *key);
CMimeHeader_T *_cmime_internal_get_linked_header(CMimeList_T *l, const char *key);

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_INTERNAL_H */
