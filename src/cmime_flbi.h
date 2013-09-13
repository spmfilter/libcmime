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

#ifndef _CMIME_FLBI_H
#define _CMIME_FLBI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "cmime_message.h"
#include "cmime_part.h"
#include "cmime_parser.tab.h"

int yyparse (void *scanner, CMimeMessage_T *msg); 
extern int yylex();
void yyerror(YYLTYPE *yyllocp, void *scanner, CMimeMessage_T *msg, const char *s, ...);
int cmime_scanner_scan_buffer(CMimeMessage_T **message, char *buffer);

char *cmime_flbi_boundary_linebreak(char *s, char *linebreak);
char *cmime_flbi_get_boundary(char *s);
void cmime_flbi_check_part_boundary(CMimePart_T *part);
char *cmime_flbi_chomp_boundary(char *s, char *linebreak);
char *cmime_flbi_scan_postface(char *s, CMimeMessage_T *msg);

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_FLBI_H */

