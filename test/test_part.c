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

#include "../src/cmime_part.h"

#include "test_data.h"

int main (int argc, char const *argv[]) {
	CMimePart_T *part;
	char *s;
	
	part = cmime_part_new();
	assert(part);
	
	cmime_part_set_content_type(part,mime_type_string1);
	s = cmime_part_get_content_type(part);
	assert(strcmp(s,mime_type_string1)==0);

	// overwrite content type
	cmime_part_set_content_type(part,mime_type_string2);
	s = cmime_part_get_content_type(part);
	assert(strcmp(s,mime_type_string2)==0);
	
	cmime_part_set_content_disposition(part,mime_disposition_string1);
	s = cmime_part_get_content_disposition(part);
	assert(strcmp(s,mime_disposition_string1)==0);
	
	cmime_part_set_content_disposition(part,mime_disposition_string2);
	s = cmime_part_get_content_disposition(part);
	assert(strcmp(s,mime_disposition_string2)==0);

	cmime_part_set_content_transfer_encoding(part,mime_encoding_string1);
	s = cmime_part_get_content_transfer_encoding(part);
	assert(strcmp(s,mime_encoding_string1)==0);
	
	cmime_part_set_content(part,test_content_string1);
	s = cmime_part_get_content(part);
	assert(strcmp(s,test_content_string1) == 0);
	
	
	
	cmime_part_free(part);
	return 0;
}