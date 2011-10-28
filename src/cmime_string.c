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

char *cmime_string_strstr_last(const char *s1, const char *s2) {
	char *strp;
	int len1, len2;

	assert(s1);
	assert(s2);
 
	len2 = strlen(s2);
	if(len2==0)
		return((char*)s1);
 
	len1 = strlen(s1);
	if(len1 - len2 <= 0)
		return(NULL);
    
	strp = (char*)(s1 + len1 - len2);
	while(strp != s1) {
		if(*strp == *s2) {
			if(strncmp(strp,s2,len2)==0)
				return(strp);
		}
		strp--;
	}
	
	return(NULL);
}

char *cmime_string_strsep_last(const char *s, const char *sep) {
	int count = 0;
	int seen = 0;
	int pos = 0;
	int len = strlen(sep);
	char *out = NULL;
	const char *it = s;
	
	while(*it != '\0') { 
		if (strncmp(it,sep,len)==0) count++;
		*it++;
	}
	
	it = s;
	out = (char *)malloc(strlen(s) + sizeof(char));
	while(*it != '\0') {
		if (strncmp(it,sep,len)==0) {
			seen++;
			if (seen == count) break;
		} 		
		out[pos++] = *it++;
	}
	out[pos] = '\0';
	return(out);
}
