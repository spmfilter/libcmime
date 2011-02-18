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

#ifndef _CMIME_LIST_H
#define _CMIME_LIST_H

#ifdef __cplusplus
extern "C" {
#endif


struct _CMimeListElem {
	void *data;
 
	struct _CMimeListElem *prev;
	struct _CMimeListElem *next;
};

typedef struct _CMimeListElem CMimeListElem_T;

struct _CMimeList {
	int size;
	void (*destroy)(void *data);
	CMimeListElem_T *head;
	CMimeListElem_T *tail;
};

typedef struct _CMimeList CMimeList_T;

int cmime_list_new(CMimeList_T **list, void (*destroy)(void *data));

int cmime_list_free(CMimeList_T *list);

/** remove an element  */
int cmime_list_remove(CMimeList_T *list, CMimeListElem_T *elem, void **data);

/** append to list */
int cmime_list_append(CMimeList_T *list, void *data);

/** prepend to list */
int cmime_list_prepend(CMimeList_T *list, void *data);

/** remove from tail */
void *cmime_list_remove_tail(CMimeList_T *list);

/** remove from head   */
void *cmime_list_remove_head(CMimeList_T *list);

/** insert new element next to elem */
int cmime_list_insert_next(CMimeList_T *list, CMimeListElem_T *elem, void *data);

/** insert new element previous to elem */
int cmime_list_insert_prev(CMimeList_T *list, CMimeListElem_T *elem, void *data);

/** iterates over list and calls function for every element with the current element */
void cmime_list_map(CMimeList_T *list, void(*func)(CMimeListElem_T *elem,void *args), void *args);

/** iterates over list and calls function func with every element, return value
 *  of func will be saved in new list **new
 */
int cmime_list_map_new(CMimeList_T *list, CMimeList_T **new, void *(*func)(CMimeListElem_T *elem, void *args), void *args);

/* * * MACROS * * */
#define cmime_list_size(list) ((list)->size)
#define cmime_list_head(list) ((list)->head)
#define cmime_list_tail(list) ((list)->tail)
#define cmime_list_is_head(elem) ((elem)->prev == NULL ? 1 : 0)
#define cmime_list_is_tail(elem) ((elem)->next == NULL ? 1 : 0)
#define cmime_list_data(elem) ((elem)->data)
#define cmime_list_next(elem) ((elem)->next)
#define cmime_list_prev(elem) ((elem)->prev)

#ifdef __cplusplus
}
#endif

#endif