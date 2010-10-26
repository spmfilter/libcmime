/* libcmime - A C mime library
 * Copyright (C) 2010 Axel Steiner
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

#include <stdlib.h>

#include "cmime_list.h"

/* create  a new list */
CMimeList_T *cmime_list_new(void) {
	CMimeList_T *list;
	list = (CMimeList_T *)calloc((size_t)1, sizeof(CMimeList_T));
	list->next = NULL;
	list->data = NULL;
	return(list);
}

/* append an element */
int cmime_list_append(CMimeList_T *list, void *data) {
	CMimeList_T *elem;
	CMimeList_T *it;

	/* its the first element */
	if(list->data == NULL) {
		list->data = data;
		list->next = NULL;
		return(0);
	}

	/* create new element */
	elem = (CMimeList_T *)malloc(sizeof(CMimeList_T));
	if(elem == NULL) {
		/* malloc failed */
		return(-1);
	}

	elem->data = data;
	elem->next = NULL;

	/* walk list up to the end */
	it = list;	
	
	while(1) {
		if(it->next == NULL) {
			break;
		}

		it = it->next;
	}
	
	it->next = elem;

	return(0);
}

/* prepend an item */
int cmime_list_prepend(CMimeList_T **list, void *data) {
	CMimeList_T *elem;

	/* its the first element */
	if((*list)->data == NULL) {
		(*list)->data = data;
		(*list)->next = NULL;
		return(0);
	}

	/* its a new element */
	elem = (CMimeList_T *)malloc(sizeof(CMimeList_T));
	elem->data = data;
	elem->next = *list;
	*list = elem;

	return(0);
}

/* dump a list */
void cmime_list_dump(CMimeList_T *list, void (*printme)(void *data)) {
	CMimeList_T *it;

	it = list;

	if(it == NULL || it->data == NULL) {
		return;
	}

	while(1) {
		printme(it->data);
		if(it->next == NULL) {
			break;
		}

		it = it->next;
	}
}

/* free used memory */
void cmime_list_free(CMimeList_T *list, void (*deleteme)(void *data)) {
	CMimeList_T *it;
	CMimeList_T *prev;

	it = list;
	
	/* the list itself is not allocated, do nothing */
	if(it == NULL) {
		return;
	}

	/* the list is allocated but contains no items so free up
	 * only the head item
	 */
	if(it != NULL && it->data == NULL && it->next == NULL) {
		free(it);
		return;
	}

	/* walk the list and call given funtion deleteme onto the data item,
	 * after freeing the data item free the list item itself and go on
	 * to the next one
	 **/
	while(1) {
		deleteme(it->data);
		
		if(it->next == NULL) {
			free(it);
			break;
		}

		prev = it; /* save pointer for freeing afterwards */
		it = it->next;
		free(prev);
	}
}
