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

char *cmime_flbi_get_boundary(char *s) {
	char *boundary = NULL;	
	int pos = 0;

	if (strcasestr(s,"boundary=")) {
		s = strstr(s,"=");
		if (*++s=='"') 
			s++;
				
		boundary = (char *)calloc(strlen(s) + sizeof(char),sizeof(char));
		while(*s!='\0') {
			if ((*s!=';') && (*s!='"'))
				boundary[pos++] = *s;
			else {
				boundary[pos] = '\0';
				break;
			}
			s++;
		}
	}
	return boundary;
}

int cmime_flbi_match_boundary(const char *haystack) {
	if (strcasestr(haystack, "boundary="))
		return(1);
	
	return(0);
}

int cmime_flbi_cmp_boundary(const char *boundary, const char *haystack, const char *linebreak) {
	char *p = NULL;
	asprintf(&p,"--%s%s",boundary,linebreak);
	if (strncmp(haystack,p,strlen(p)) == 0) {
		free(p);
		return(1);
	}
	
	free(p);

	asprintf(&p,"--%s--",boundary);
	if (strncmp(haystack,p,strlen(p)) == 0) {
		free(p);
		return(2);
	}
	
	free(p);

	return(0);
}

void cmime_flbi_check_part_boundary(CMimePart_T *part) {
	CMimeListElem_T *e = NULL;
	CMimeHeader_T *h = NULL;
	char *bound = NULL;
	
	e = cmime_list_head(part->headers);
	while(e != NULL) {
		h = (CMimeHeader_T *)cmime_list_data(e);

		bound = cmime_flbi_get_boundary(cmime_header_get_value(h,0));
		if (bound != NULL) {
			part->boundary = bound;
			break;
		}
		e = e->next;
	}
}

/*
int cmime_flbi_check_last_boundary(char *s) {
	int in_boundary = 0;
	
	if (s!=NULL) {
		while(*s!='\0') {
			if ((*s)!='-') {
				in_boundary = 1;
				s++;
			} else if ((in_boundary==1) && ((*s)=='-') && ((*s++)=='-')) {
				return(1);
			} else
				s++;
		}
	}
	
	return(0);
}

*/