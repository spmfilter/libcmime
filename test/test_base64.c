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

#include "../src/cmime_base64.h"

#include "test_data.h"

int main (int argc, char const *argv[])	{
	char *out = NULL;
	char *out2 = NULL;
	
	out = cmime_base64_encode_string(test_string1);
	free(out);
	
	out = cmime_base64_encode_string(test_string5);
	assert(strcmp(out,test_string5_base64)==0);
	printf("IN:   [%s] (%d)\n", test_string5, (int)strlen(test_string5));
	printf("CO    [%s]\n", out);
	out2 = cmime_base64_decode_string(out);
	printf("OUT2: [%s]\n", out2);
	free(out2);
	free(out);
	
	
	
	
	
	return 0;
}
