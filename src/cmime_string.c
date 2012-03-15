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

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "cmime_string.h"

CMimeStringList_T *cmime_string_list_new(void) {
	CMimeStringList_T *sl;
	sl = (CMimeStringList_T *)calloc((size_t)1,sizeof(CMimeStringList_T));
	sl->count = 0;
	sl->node = NULL;
	return(sl);
}

void cmime_string_list_free(CMimeStringList_T *sl) {
	size_t i;
	assert(sl);

	for(i = 0; i < sl->count; i++) {
		free(sl->node[i]);
	} 
	free(sl->node);
	sl->node = NULL;
	free(sl); 
}

void cmime_string_list_insert(CMimeStringList_T *sl, const char *s) {
	char **tmp = NULL;
	assert(sl);
	assert(s);

	tmp = realloc(sl->node, (sizeof( *tmp) * (sl->count+1)));
	tmp[sl->count] = strdup(s);
	sl->node = tmp;
	sl->count++;
}

char *cmime_string_list_get(CMimeStringList_T *sl,int pos) {
	assert(sl);
	assert(pos <= sl->count);
	return(sl->node[pos]);
}

CMimeStringList_T *cmime_string_split(const char *s, const char *sep, int maxsplit) {
	CMimeStringList_T *sl;
	char *cp = NULL;
	char *tf = NULL;
	char *tmp = NULL;
	int size = 0;
	int count = 0;
	
	size = strlen(s);
	
	assert(s);
	assert(size>0);
	assert(sep);

	sl = cmime_string_list_new();
	
	tf = cp = strdup(s);
	
	while ((tmp = strsep(&cp, sep)) != NULL) {
		cmime_string_list_insert(sl,tmp);
		if ((maxsplit != 0) && (count == maxsplit))
			break;
		count++;
	}

	free(tf);
	
	return(sl);
}

char *cmime_string_strip(char *s) {
	assert(s);

	char * e = s + strlen(s) - 1;
	while(*s && isspace(*s)) s++;
	while(e > s && isspace(*e)) *e-- = '\0';

	return(s);
}

char *cmime_string_chomp(char *s) {
	char *p = NULL;

	switch(s[strlen(s)-1]) {
		case '\n':
			if((p = strrchr(s,'\r')) != NULL ) *p = '\0';
			else *(p = strrchr(s,'\n')) = '\0';
			break;
		case '\r':
			*(p = strrchr(s,'\r')) = '\0';
			break;
		case '\x0c':
			*(p = strrchr(s,'\x0c')) = '\0';
			break;
	}

	return(s);
}
