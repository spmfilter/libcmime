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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../src/cmime_table.h"

#include "test_data.h"

void tdestroy(void *data) {
	assert(data);
}

void tmap(const void *key, void **count, void *args) {
	assert(key);
}

int main (int argc, char const *argv[]) {
	CMimeTable_T *t = NULL;
	char *out = NULL;
	
	if (cmime_table_new(&t,0,NULL,NULL,tdestroy)!=0) {
		printf("Failed to create new CMimeTable_T\n");			
		return(-1);
	}
	
	if (cmime_table_insert(t,header_string1_key,header_string1_value)!=0) {
		printf("Failed to insert data to CMimeTable_T\n");
		return(-1);
	}
	
	out = cmime_table_get(t,header_string1_key);
	assert(strcmp(out,header_string1_value)==0);
	
	if (cmime_table_insert(t,header_string2_key,header_string2_value)!=0) {
		printf("Failed to insert data to CMimeTable_T\n");
		return(-1);
	}
	
	cmime_table_map(t,tmap,NULL);
	
	if (cmime_table_remove(t,header_string1_key,NULL)!=0) {
		printf("Failed to remove key '%s' from CMimeTable_T\n",header_string1_key);
		return(-1);
	}
	
	if (cmime_table_length(t)!=1) {
		printf("Expected table length '1', but got '%d'\n",cmime_table_length(t));
		return(-1);
	}
	
	cmime_table_free(t);
	
	return(0);
}
