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
#include <stdlib.h>
#include <assert.h>

#include "cmime_list.h"

/* initializes a new empty doubly linked list */
int cmime_list_new(CMimeList_T **list, void (*destroy)(void *data)) {
	(*list) = (CMimeList_T *)calloc((size_t)1,sizeof(CMimeList_T));
	
	if(*list == NULL) {
		return(-1);
	}

	(*list)->size = 0;
	(*list)->head = NULL;
	(*list)->tail = NULL;
	(*list)->destroy = destroy;

	return(0);
}

/* destroy a complete list */
int cmime_list_free(CMimeList_T *list) {
	void *data;
	assert(list);

	while(cmime_list_size(list) > 0) {
		if(cmime_list_remove(list, cmime_list_tail(list), (void **)&data) == 0 &&
			list->destroy != NULL) {
				list->destroy(data);
			}
		}
 
	/* no more operations please... */
	free(list);
	return(0);
}

/* remove an element from the list */
int cmime_list_remove(CMimeList_T *list, CMimeListElem_T *elem, void **data) {
	/* no null element and no empty list */
	if(elem == NULL || cmime_list_size(list) == 0) {
		assert(list);
		assert(elem);
		return(-1);
	}
 
	*data = elem->data;
		
	/* handle removal of first element */
	if(elem == cmime_list_head(list)) {
		list->head = elem->next;
 
		if(list->head == NULL) {
			list->tail = NULL;
		} else {
			list->head->prev = NULL;
		}
	} else {
		elem->prev->next = elem->next;
 
		if(elem->next == NULL) {
			list->tail = elem->prev;
		} else {
			elem->next->prev = elem->prev;
		}
	}
 
	free(elem);
	list->size--;

	return(0);
}

/* remove tail element and return data pointer */
void* cmime_list_pop_tail(CMimeList_T *list) {
	void *data;
	int ret;
	
	assert(list);
	
	ret = cmime_list_remove(list,cmime_list_tail(list),&data);
 
	if(ret == 0) {
		return(data);
	} else {
		return(NULL);
	}
}

void *cmime_list_pop_head(CMimeList_T *list) {
	void *data;
	int ret;
 
	assert(list);
	
	ret = cmime_list_remove(list,cmime_list_head(list),&data);
 
	if(ret == 0) {
		return(data);
	} else {
		return(NULL);
	}
}

/* insert new elem next to given element elem */
int cmime_list_insert_next(CMimeList_T *list, CMimeListElem_T *elem, void *data) {
	CMimeListElem_T *new = (CMimeListElem_T *)calloc(1,sizeof(CMimeListElem_T));
	
	assert(list);
		
	if(new == NULL) {
		return(-1);
	}
 	
	/* no null element if list not empty */
	if(elem == NULL && cmime_list_size(list) != 0) {
		return(-1);
	}
 
	new->data = data;
 
	if(cmime_list_size(list) == 0) {
		list->head = new;
		list->tail = new;
		new->next = NULL;
		new->prev = NULL;
	} else {
		new->next = elem->next;
		new->prev = elem;

		if(elem->next == NULL) {
			list->tail = new;
		} else {
			elem->next->prev = new;
		}

		elem->next = new; 
	}
 
	list->size++;
 
	return(0);
}

/* insert new element previous to given element elem */
int cmime_list_insert_prev(CMimeList_T *list, CMimeListElem_T *elem, void *data) {
	CMimeListElem_T *new = (CMimeListElem_T *)calloc(1,sizeof(CMimeListElem_T));
	
	assert(list);
	assert(elem);
	
	if(new == NULL) {
		return(-1);
	}
 
	/* no null element if list not empty */
	if(elem == NULL && cmime_list_size(list) != 0) {
		return(-1);
	}
 
	new->data = data;
 
	if(cmime_list_size(list) == 0) {
		list->head = new;
		list->tail = new;
		new->next = NULL;
		new->prev = NULL;
	} else {
		new->next = elem;
		new->prev = elem->prev;
 
		if(elem->prev == NULL) {
			list->head = new;
		} else {
			elem->prev->next = new;
		}
 
		elem->prev = new;
	}
 
	list->size++;
 
	return(0);
}

/* append to the end of a list */
int cmime_list_append(CMimeList_T *list, void *data) {
	assert(list);
	assert(data);
	return cmime_list_insert_next(list,cmime_list_tail(list),data);
}
 
/* prepend an element to the list */
int cmime_list_prepend(CMimeList_T *list, void *data) {
	assert(list);
	assert(data);
	return cmime_list_insert_prev(list,cmime_list_head(list),data);
}

/* apply function func to every element in the list */
void cmime_list_map(CMimeList_T *list, void(*func)(CMimeListElem_T *elem,void *args), void *args) {
	CMimeListElem_T *elem;
	
	assert(list);
	
	elem = cmime_list_head(list);
	while(elem != NULL) {
		func(elem,args);
		elem = elem->next;
	}
}

int cmime_list_map_new(CMimeList_T *list, CMimeList_T **new, void *(*func)(CMimeListElem_T *elem,
		void *args), void *args) {
	CMimeListElem_T *elem;
	int ret;
 
	assert(list);

	ret = cmime_list_new(new, NULL);
	if(ret != 0) {
		return(-1);
	}
 
	elem = cmime_list_head(list);
	while(elem != NULL) {
		cmime_list_append(*new,func(elem,args));
		elem = elem->next;
	}
 
	return(0);
}
