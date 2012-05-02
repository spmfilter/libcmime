/* libcmime - A C mime library
 * Copyright (C) 2012 SpaceNet AG and Axel Steiner <ast@treibsand.com>
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
 * @file cmime_list.h
 * @brief Defines functions and structs for double linked list
 */

#ifndef _CMIME_LIST_H
#define _CMIME_LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

/*!
 * @struct CMimeListElem_T cmime_list.h
 * @brief An element of a CMimeList_T list
 */
typedef struct _CMimeListElem {
	void *data;  /**< pointer to element data */
	struct _CMimeListElem *prev;  /**< pointer to previous element */
	struct _CMimeListElem *next;  /**< pointer to next element */
} CMimeListElem_T;

/*!
 * @struct CMimeList_T cmime_list.h
 * @brief Double linked list implementation
 */
typedef struct _CMimeList {
	int size;  /**< list size */
	void (*destroy)(void *data);  /**< function pointer for destroy function */
	CMimeListElem_T *head;  /**< pointer to list head */
	CMimeListElem_T *tail;  /**< pointer to list tail */
} CMimeList_T;

/*!
 * @fn cmime_list_new(CMimeList_T **list, void (*destroy)(void *data))
 * @brief Creates a new CMimeList_T list
 * @param list out param to return the new list
 * @param func list destroy function
 * @returns 0 on success or -1 in case of error
 */
int cmime_list_new(CMimeList_T **list, void (*destroy)(void *data));

/*!
 * @fn cmime_list_free(CMimeList_T *list)
 * @brief Free a CMimeList_T list
 * @param list list to free
 * @returns 0 on success or -1 in case of error
 */
int cmime_list_free(CMimeList_T *list);

/*! 
 * @fn cmime_list_remove(CMimeList_T *list, CMimeListElem_T *elem, void **data)
 * @brief Remove an element from list
 * @param list a CMimeList_T list
 * @param elem the CMimeListElem_T element which should be removed
 * @param data out param to return element data
 * @returns 0 on success or -1 in case of error  
 */
int cmime_list_remove(CMimeList_T *list, CMimeListElem_T *elem, void **data);

/*! 
 * @fn cmime_list_append(CMimeList_T *list, void *data)
 * @brief Append data to the end of a list
 * @param list CMimeList_T list to which new data should be appended
 * @param data new data which should be appended
 * @returns 0 on success or -1 in case of error 
 */
int cmime_list_append(CMimeList_T *list, void *data);

/*! 
 * @fn cmime_list_prepend(CMimeList_T *list, void *data)
 * @brief Prepend data to a list
 * @param list a CMimeList_T list to which new data should be prepended
 * @param data new data which should be appended
 * @returns 0 on success or -1 in case of error  
 */
int cmime_list_prepend(CMimeList_T *list, void *data);

/*! 
 * @fn cmime_list_pop_tail(CMimeList_T *list)
 * @brief Remove tail element from list and return data pointer
 * @param list a CMimeList_T list
 * @returns data pointer of removed list element
 */
void *cmime_list_pop_tail(CMimeList_T *list);

/*! 
 * @fn cmime_list_pop_head(CMimeList_T *list)
 * @brief Remove head element from list an return data pointer
 * @param list a CMimeList_T list
 * @returns data pointer of removed list element   
 */
void *cmime_list_pop_head(CMimeList_T *list);

/*! 
 * @fn cmime_list_insert_next(CMimeList_T *list, CMimeListElem_T *elem, void *data)
 * @brief Insert new element next to elem
 * @param list a CMimeList_T list
 * @param elem a CMimeListElem_T element
 * @param data data to insert next to element
 * @returns 0 on sucess or -1 in case of error
 */
int cmime_list_insert_next(CMimeList_T *list, CMimeListElem_T *elem, void *data);

/*! 
 * @fn cmime_list_insert_prev(CMimeList_T *list, CMimeListElem_T *elem, void *data)
 * @brief Insert new element previous to elem 
 * @param list a CMimeList_T list
 * @param elem a CMimeListElem_T element
 * @param data data to insert previous to element
 * @returns 0 on sucess or -1 in case of error
 */
int cmime_list_insert_prev(CMimeList_T *list, CMimeListElem_T *elem, void *data);

/*! 
 * @fn cmime_list_map(CMimeList_T *list, void(*func)(CMimeListElem_T *elem,void *args), void *args)
 * @brief Iterates over list and calls function for every element with the current element 
 * @param list a CMimeList_T list
 * @param func function to call for each element
 * @param args optional arguments for function pointer
 */
void cmime_list_map(CMimeList_T *list, void(*func)(CMimeListElem_T *elem,void *args), void *args);

/*! 
 * @fn cmime_list_map_new(CMimeList_T *list, CMimeList_T **new, void *(*func)(CMimeListElem_T *elem, void *args), void *args)
 * @brief Iterates over list and calls function func with every element, return value of func will be saved in new list **new
 * @param list a CMimeList_T list
 * @param new out param to return the new list
 * @param func function to call for each element
 * @param args optional arguments for function pointer
 */
int cmime_list_map_new(CMimeList_T *list, CMimeList_T **new, void *(*func)(CMimeListElem_T *elem, void *args), void *args);

/*!
 * @def cmime_list_size(list)
 * @returns size of CMimeList_T list
 */
#define cmime_list_size(list) ((list)->size)

/*! 
 * @def cmime_list_head(list)
 * @returns head element of CMimeList_T list  
 */
#define cmime_list_head(list) ((list)->head)

/*!
 * @def cmime_list_tail(list)
 * @returns tail element of CMimeList_T list
 */
#define cmime_list_tail(list) ((list)->tail)

/*!
 * @def cmime_list_is_head(elem)
 * @returns 1 if element is list head, 0 if not
 */
#define cmime_list_is_head(elem) ((elem)->prev == NULL ? 1 : 0)

/*! 
 * @def cmime_list_is_tail(elem)
 * @returns 1 if element is list tail, 0 if not
 */
#define cmime_list_is_tail(elem) ((elem)->next == NULL ? 1 : 0)

/*!
 * @def cmime_list_data(elem)
 * @returns data pointer of element
 */
#define cmime_list_data(elem) ((elem)->data)

/*!
 * @def cmime_list_next(elem)
 * @returns next element
 */
#define cmime_list_next(elem) ((elem)->next)

/*!
 * @def cmime_list_prev(elem)
 * @returns previous element
 */
#define cmime_list_prev(elem) ((elem)->prev)

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_LIST_H */
