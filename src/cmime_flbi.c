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

int cmime_flbi_cmp_boundaries(CMimeYYExtra_T *yydata, const char *haystack) {
	char *p = NULL;
	char **it = NULL;
	
	asprintf(&p,"--%s%s",yydata->message->boundary,yydata->message->linebreak);
	if (strncmp(haystack,p,strlen(p)) == 0) {
		free(p);
		return(1);
	}	
	free(p);

	it = yydata->sub_part_boundaries;
	while(*it!=NULL) {
		asprintf(&p,"--%s%s",*it,yydata->message->linebreak);
		if (strncmp(haystack,p,strlen(p)) == 0) {
			free(p);
			return(1);
		}
		free(p);
		it++;
	}

	return(0);
}

int cmime_flbi_cmp_closing_boundary(const char *boundary, const char *haystack) {
	char *p = NULL;

	asprintf(&p,"--%s--",boundary);
	if (strncmp(haystack,p,strlen(p)) == 0) {
		free(p);
		return(1);
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

int cmime_flbi_strstr_boundary(CMimeYYExtra_T *yydata, const char *haystack) {
	char **it = NULL;

	if (strstr(haystack,yydata->message->boundary)) {
		return(1);
	} 

	it = yydata->sub_part_boundaries;
	while(*it != NULL) {
		if (strstr(haystack,*it))
			return(1);
		it++;
	}

	return(0);
}

char *cmime_flbi_get_parent_boundary(CMimeMessage_T *msg, const char *search_parent) {
	char *s = NULL;
	CMimeListElem_T *e = NULL;
	CMimePart_T *p = NULL;

	s = msg->boundary;

	e = cmime_list_head(msg->parts);
	while(e != NULL) {
		p = (CMimePart_T *)cmime_list_data(e);
		if (p->boundary) {
			if (search_parent != NULL) {
				if (strcmp(search_parent,p->boundary)==0) {
					s = p->parent_boundary;
					break;	
				} 
			}
		}
		e = e->prev;
	}

	return(s);
}
