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
#include <assert.h>

#include "../src/cmime_table.h"

void tmap(const void *key, void **count, void *args) {
	assert(key);
}

int main (int argc, char const *argv[]) {
	CMimeTable_T *t = NULL;
	char test_key1[] = "subject";
	char test_value1[] = "This is a test subject";
	char test_key2[] = "X-Foo";
	char test_value2[] = "bar";
	char *out = NULL;
	
	if (cmime_table_new(&t,0,NULL,NULL)!=0) {
		printf("Failed to create new CMimeTable_T\n");			
		return(-1);
	}
	
	if (cmime_table_insert(t,test_key1,test_value1)!=0) {
		printf("Failed to insert data to CMimeTable_T\n");
		return(-1);
	}
	
	out = cmime_table_get(t,test_key1);
	assert(strcmp(out,test_value1)==0);
	
	if (cmime_table_insert(t,test_key2,test_value2)!=0) {
		printf("Failed to insert data to CMimeTable_T\n");
		return(-1);
	}
	
	cmime_table_map(t,tmap,NULL);
	
	if (cmime_table_remove(t,test_key1,NULL)!=0) {
		printf("Failed to remove key '%s' from CMimeTable_T\n",test_key1);
		return(-1);
	}
	
	if (cmime_table_length(t)!=1) {
		printf("Expected table length '1', but got '%d'\n",cmime_table_length(t));
		return(-1);
	}
	
	cmime_table_free(t);
	
	return(0);
}
