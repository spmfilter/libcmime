/* libcmime - A C mime library
 * Copyright (C) 2013 SpaceNet AG and Axel Steiner <ast@treibsand.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
 * @brief Iterates over list and calls function func with every element, return value of func will be saved in new list **new_list
 * @param list a CMimeList_T list
 * @param new_list out param to return the new list
 * @param func function to call for each element
 * @param args optional arguments for function pointer
 */
int cmime_list_map_new(CMimeList_T *list, CMimeList_T **new_list, void *(*func)(CMimeListElem_T *elem, void *args), void *args);

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
