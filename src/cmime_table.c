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

#include <stdlib.h>
#include <limits.h>

#include "cmime_assert.h"
#include "cmime_table.h"

static int cmpatom(const void *x, const void *y) {
	return x != y;
}

static unsigned hashatom(const void *key) {
	return (unsigned long)key>>2;
}

int cmime_table_new(CMimeTable_T **table, int hint, 
		int cmp(const void *x, const void *y), 
		unsigned hash(const void *key)) {

	int i;
	static int primes[] = { 509, 509, 1021, 2053, 4093,
		8191, 16381, 32771, 65521, INT_MAX };
	
	assert(table);
	assert(hint >= 0);
	for (i = 1; primes[i] < hint; i++)
		;
	
	*table = (CMimeTable_T *)malloc(sizeof(CMimeTable_T) + primes[i-1]*sizeof ((*table)->buckets[0]));
	if (*table == NULL)
		return(-1);
		
	(*table)->size = primes[i-1];
	(*table)->cmp  = cmp ? cmp : cmpatom;
	(*table)->hash = hash ? hash : hashatom;
	(*table)->buckets = (struct binding **)((*table) + 1);
	for (i = 0; i < (*table)->size; i++)
		(*table)->buckets[i] = NULL;
	(*table)->length = 0;
	(*table)->timestamp = 0;
	
	return(0);
}


void *cmime_table_get(CMimeTable_T *table, const void *key) {
	int i;
	struct binding *p;
	
	assert(table);
	assert(key);
	
	i = (*table->hash)(key)%table->size;
	for (p = table->buckets[i]; p; p = p->link) {
		if ((*table->cmp)(key, p->key) == 0)
			break;
	}
	return(p ? p->value : NULL);
}

void *cmime_table_insert(CMimeTable_T *table, const void *key, void *value) {
	int i;
	struct binding *p;
	void *prev;
	
	assert(table);
	assert(key);
	
	i = (*table->hash)(key)%table->size;
	for (p = table->buckets[i]; p; p = p->link) {
		if ((*table->cmp)(key, p->key) == 0)
			break;
	}
	
	if (p == NULL) {
		p = malloc((long)sizeof *(p));
		p->key = key;
		p->link = table->buckets[i];
		table->buckets[i] = p;
		table->length++;
		prev = NULL;
	} else {
		prev = p->value;
	}
	p->value = value;
	table->timestamp++;

	return(prev);
}

void cmime_table_map(CMimeTable_T *table,
		void apply(const void *key, void **value, void *cl),
		void *cl) {
	int i;
	unsigned stamp;
	struct binding *p;
	
	assert(table);
	assert(apply);
	
	stamp = table->timestamp;
	for (i = 0; i < table->size; i++) {
		for (p = table->buckets[i]; p; p = p->link) {
			apply(p->key, &p->value, cl);
			assert(table->timestamp == stamp);
		}
	}
}

void *cmime_table_remove(CMimeTable_T *table, const void *key) {
	int i;
	struct binding **pp;
	
	assert(table);
	assert(key);
	
	table->timestamp++;
	i = (*table->hash)(key)%table->size;
	for (pp = &table->buckets[i]; *pp; pp = &(*pp)->link) {
		if ((*table->cmp)(key, (*pp)->key) == 0) {
			struct binding *p = *pp;
			void *value = p->value;
			*pp = p->link;
			if (p != NULL)
				free(p);
			table->length--;
			return value;
		}
	}
	return(NULL);
}

void cmime_table_free(CMimeTable_T *table) {
	assert(table);
	
	if (table->length > 0) {
		int i;
		struct binding *p, *q;
		for (i = 0; i < table->size; i++) {
			for (p = table->buckets[i]; p; p = q) {
				q = p->link;
				if (p != NULL)
					free(p);
			}
		}
	}
	free(table);
}
