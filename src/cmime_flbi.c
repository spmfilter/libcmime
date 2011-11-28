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

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "cmime_flbi.h"

void cmime_flbi_check_boundary(CMimeMessage_T *msg, char *s) {
	int pos = 0;
	if (msg->boundary==NULL) {
		if (strcasestr(s,"boundary=")) {
			s = strstr(s,"=");
			if (*++s=='"') 
				s++;
				
			msg->boundary = (char *)calloc(strlen(s) + sizeof(char),sizeof(char));
			while(*s!='\0') {
				if ((*s!=';') && (*s!='"'))
					msg->boundary[pos++] = *s;
				else {
					msg->boundary[pos] = '\0';
					break;
				}
				s++;
			}
		}
	}
}