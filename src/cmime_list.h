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


/** structure for creating a simple linked list implementation */
struct _CMimeList {
	void *data;
	struct _CMimeList *next;
};

/* typedef for linked list */
typedef struct _CMimeList CMimeList_T;

/** Create a new list
  *
  * \returns CMimeList_T Pointer to beginning of new list on success, NULL on failure
  */
CMimeList_T *cmime_list_new(void);

/** Append a new element to the list
  *
  * \param list pointer to list head
  * \param data pointer to data segment to add to the list
  *
  * \returns 0 on success, -1 on error
  */
int cmime_list_append(CMimeList_T *list, void *data);

/** Prepend a new element to the list
  *
  * \param list pointer to pointer of list head, required due new memory
  *             allocation
  * \param data pointer to data segment to add to the list
  *
  * \returns 0 on success, -1 on error
  */
int cmime_list_prepend(CMimeList_T **list, void *data);

/** Dump the contents of a list (debugging purposes)
  *
  * \param list pointer to list head
  * \param printme function pointer for function to use when printing data
  *
  * \returns Nothing
  */
void cmime_list_dump(CMimeList_T *list, void (*printme)(void *data));

/** Cleanup all resources allocated with a list
  *
  * \param list pointer to list head
  * \param deleteme function pointer pointing to function responsible for
  *                 cleaning resources
  *
  * \returns Nothing
  */
void cmime_list_free(CMimeList_T *list, void (*deleteme)(void *data));

#ifdef __cplusplus
}
#endif

#endif