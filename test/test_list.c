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

#include <stdio.h>
#include <stdlib.h>

#include "../src/cmime_list.h"

void list_char_printer(void *data) {
	printf("elem is %s\n", (char *)data);
}

int main (int argc, char const *argv[]) {
	CMimeList_T *head;

	head = cmime_list_new();

	cmime_list_append(head, "this is test 1");
	cmime_list_append(head, "this is test 2");
	cmime_list_append(head, "this is test 3");
	cmime_list_append(head, "this is test 4");
	cmime_list_prepend(&head, "this is test 5");
	cmime_list_prepend(&head, "this is test 6");
	cmime_list_append(head, "this is test 7");

	cmime_list_dump(head, list_char_printer);
	
	return(0);
}
