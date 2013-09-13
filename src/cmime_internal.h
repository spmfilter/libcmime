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

#ifndef _CMIME_INTERNAL_H
#define _CMIME_INTERNAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

#define FROM_HEADER "From"

/** crlf */
#define CRLF "\r\n"
#define DCRLF "\r\n\r\n"
#define LF "\n"
#define CR "\r"

typedef struct {
    CMimeBoundaryType_T type;
    char *marker;
    size_t len;
} _BoundaryInfo_T;

void _cmime_internal_header_destroy(void *data);

char *_cmime_internal_determine_linebreak_from_file(const char *s);
char *_cmime_internal_determine_linebreak(const char *s);

void _cmime_internal_set_linked_header_value(CMimeList_T *l, const char *key, const char *value);
char *_cmime_internal_get_linked_header_value(CMimeList_T *l, const char *key);
CMimeHeader_T *_cmime_internal_get_linked_header(CMimeList_T *l, const char *key);
void _cmime_internal_parts_destroy(void *data);
_BoundaryInfo_T *_cmime_internal_get_boundary_info(CMimeStringList_T *boundaries, char *s, char *newline);


#ifdef __cplusplus
}
#endif

#endif /* _CMIME_INTERNAL_H */
