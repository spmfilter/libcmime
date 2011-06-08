/* libcmime - A C mime library
 * Copyright (C) 2010 Axel Steiner <ast@treibsand.com>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/cmime_list.h"

#define TEST_STRING1 "This is test 1"
#define TEST_STRING2 "This is test 2"
#define TEST_STRING3 "This is test 3"
#define TEST_STRING4 "This is test 4"

void list_char_printer(CMimeListElem_T *elem,void *args) {
	printf("%s\n", (char *)elem->data);
}

void list_destroy(void *data) {
	char *s = (char *)data;
	if (s!=NULL) free(s);
}

int main (int argc, char const *argv[]) {
	CMimeList_T *l;
	char *s = NULL;
	char *s2 = NULL;
	char *s3 = NULL;
	char *s4 = NULL;
	char *out;
	char *data;
	char *pop;
	CMimeListElem_T *e;
	
	if (cmime_list_new(&l,list_destroy)!=0) {
		printf("Failed to create new CMimeList_T\n");			
		return(-1);
	}

	s = malloc(strlen(TEST_STRING1) + 1);
	strcpy(s,TEST_STRING1);
	if (cmime_list_append(l,s)!=0) {
		printf("Failed to append data to CMimeList_T\n");
		return(-1);
	}
	
	out = (char *)cmime_list_data(cmime_list_head(l));
	if (strcmp(s,out)!=0) {
		printf("Expected '%s', but got '%s'\n",s,out);	
		return(-1);
	}
	
	if (cmime_list_size(l)!=1) {
		printf("Expected list size 1, but got %d",cmime_list_size(l));
		return(-1);
	}
	
	s2 = malloc(strlen(TEST_STRING2) + 1);
	strcpy(s2,TEST_STRING2);
	e = cmime_list_head(l);

	if (cmime_list_is_head(e)!=1) {
		printf("List element is not head\n");
		return(-1);
	}

	if (cmime_list_insert_next(l,e,s2)!=0) {
		printf("Failed to insert data to CMimeList_T\n");
		return(-1);
	}
	
	if (cmime_list_size(l)!=2) {
		printf("Expected list size 2, but got %d",cmime_list_size(l));
		return(-1);
	}
	
	e = cmime_list_tail(l);
	out = (char *)cmime_list_data(e);
	if (strcmp(s2,out)!=0) {
		printf("Expected '%s', but got '%s'\n",s2,out);
		return(-1);
	}
	
	if (cmime_list_is_tail(e)!=1) {
		printf("List element is not tail\n");
		return(-1);
	}
	
	s3 = malloc(strlen(TEST_STRING3) + 1);
	strcpy(s3,TEST_STRING3);
	
	if (cmime_list_insert_prev(l,e,s3)!=0) {
		printf("Failed to insert data to CMimeList_T\n");
		return(-1);
	}
	
	out = (char *)cmime_list_data(cmime_list_prev(e));
	if (strcmp(s3,out)!=0) {
		printf("Expected '%s', but got '%s'\n",s3,out);
		return(-1);
	}
	
	s4 = malloc(strlen(TEST_STRING4) + 1);
	strcpy(s4,TEST_STRING4);
	if (cmime_list_prepend(l,s4)!=0) {
		printf("Failed to prepend data to CMimeList_T\n");
		return(-1);
	}
	cmime_list_map(l,list_char_printer,NULL);

	e = cmime_list_head(l);
	if (cmime_list_remove(l,e,&data)!=0) {
		printf("Failed to remove element from CMimeList_T\n");
		return(-1);
	}
	free(data);

	pop = cmime_list_pop_head(l);
	if (pop==NULL) {
		printf("Failed to pop head from CMimeList_T\n");
		return(-1);
	} else {
		if (strcmp(pop,TEST_STRING1)!=0) {
			printf("Expected '%s', but got '%s'\n",TEST_STRING1,pop);
			return(-1);
		}
		free(pop);
	}
	
	pop = cmime_list_pop_tail(l);
	if(pop==NULL) {
		printf("Failed to pop tail from CMimeList_T\n");
		return(-1);
	} else {
		if (strcmp(pop,TEST_STRING2)!=0) {
			printf("Expected '%s', but got '%s'\n",TEST_STRING2,pop);
			return(-1);
		}
		free(pop);
	}
	
	if (cmime_list_free(l)!=0) {
		printf("Failed to free CMimeList_T\n");
		return(-1);
	}

	return(0);
}
