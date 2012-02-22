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
 * @file cmime_flbi.h
 * @brief -
 *
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

typedef struct {
	CMimeMessage_T *message;
	char *sub_part_boundary;
} CMimeYYExtra_T;

//void yyrestart (FILE *input_file);
int yyparse (void *scanner, CMimeMessage_T *msg); 
extern int yylex();
//void yyerror(char *s,...);
void yyerror(YYLTYPE *yyllocp, void *scanner, CMimeMessage_T *msg, const char *s, ...);
int cmime_scanner_scan_file(CMimeMessage_T **message, FILE *fp); 

char *cmime_flbi_get_boundary(char *s);
int cmime_flbi_match_boundary(const char *haystack);
int cmime_flbi_cmp_boundary(const char *boundary, const char *haystack, const char *linebreak);
void cmime_flbi_check_part_boundary(CMimePart_T *part);
//int cmime_flbi_check_last_boundary(char *s);

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_FLBI_H */