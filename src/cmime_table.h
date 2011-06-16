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

/*!
 * @file cmime_table.h
 * @brief Defines functions and structs for hash table
 *
 * This header is intended for use by libcmime only.
 *
 * @example test_table.c
 */

#ifndef _CMIME_TABLE_H
#define _CMIME_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @struct CMimeTable_T cmime_table.h
 * @brief Hash table implementation
 */

/*!
 * @struct CMimeTable_T::binding cmime_table.h
 * @brief Array with key/value pairs for hash table
 */
typedef struct {
	int size; /**< prime chosen from size hint */
	int (*cmp)(const void *x, const void *y); /**< compare function */
	unsigned (*hash)(const void *key); /**< hash function */
	int length; /**< number of hash table entries */
	unsigned timestamp; /**< table's timestamp, incremented every time the table is changed */
	struct binding {
		struct binding *link; /**< pointer to next entry */
		const void *key; /**< hash table key */
		void *value; /**< hash table value */
	} **buckets;
} CMimeTable_T;

/*!
 * @fn cmime_table_new(CMimeTable_T **table, int hint, int cmp(const void *x, const void *y), unsigned hash(const void *key))
 * @brief Creates a new CMimeTable_T table
 * @param table out param to return the new table
 * @param hint estimate number of entries
 * @param cmp Key comparsion function. Given two keys, x 
 *   and y, cmp(x,y) must return an integer less than zero, 
 *   equal to zero, or greater than zero, if, respectively, 
 *   x is less than y, x equals y, or x is greater than y. 
 *   The standard library function strcmp is an example of a 
 *   comparison function suitable for keys that are strings.
 * @param hash Hash function must return a hash number for key; 
 *   if cmp(x,y) returns zero, then hash(x) must be equal to 
 *   hash(y). Each table can have its own hash and cmp functions.
 * @returns 0 on success or -1 in case of error
 */
int cmime_table_new(CMimeTable_T **table, int hint,
	int cmp(const void *x, const void *y),
	unsigned hash(const void *key));

/*!
 * @fn cmime_table_free(CMimeTable_T *table)
 * @brief Free a CMimeTable_T table
 * @param table table to free
 */
void cmime_table_free(CMimeTable_T *table);

/*! 
 * @fn cmime_table_insert(CMimeTable_T *table, const void *key, void *value)
 * @brief Inserts new key/value pair into table. If key already exists, the
 *   new value will be stored.
 * @param table a CMimeTable_T table
 * @param key lookup key
 * @param value value to store in table
 * @returns 0 on success or -1 in case of error
 */
int cmime_table_insert(CMimeTable_T *table, const void *key, void *value);

/*!
 * @fn cmime_table_get(CMimeTable_T *table, const void *key)
 * @brief Searching the table for a key
 * @param table a CMimeTable_T table
 * @param key search key
 * @returns value for key
 */
void *cmime_table_get(CMimeTable_T *table, const void *key);

/*!
 * @fn int cmime_table_remove(CMimeTable_T *table, const void *key, void **data)
 * @brief Searches for key and removes binding from table.
 * @param table a CMimeTable_T table
 * @param key key which binding should be removed
 * @param data out param to return old value
 * @returns 0 on success or -1 in case of error
 *
 */
int cmime_table_remove(CMimeTable_T *table, const void *key, void **data);

/*!
 * @fn cmime_table_map(CMimeTable_T *table, void apply(const void *key, void **value, void *cl), void *cl)
 * @brief Calls the function pointed to by apply for every key-value pair in table in an unspecified order.
 * @param table a CMimeTable_T table
 * @param apply function to call for every element
 * @param args optional arguments for function pointer
 */
void cmime_table_map(CMimeTable_T *table,
	void apply(const void *key, void **value, void *args),
	void *args);

/*!
 * @def cmime_table_length(table)
 * @returns length of CMimeTable_T table
 */
#define cmime_table_length(table) ((table)->length)

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_TABLE_H */
