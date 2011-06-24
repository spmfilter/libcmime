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

#include "../src/cmime_address.h"

#include "test_data.h"

int main (int argc, char const *argv[]) {
	char *s = NULL;
	CMimeAddress_T *ca = cmime_address_new();
	
	cmime_address_set_name(ca, addr_string1_name_part);
	cmime_address_set_email(ca, addr_string1_email_part);
	
	
	s = cmime_address_to_string(ca);
	assert(strcmp(s,addr_string1) == 0);
	free(s);
	cmime_address_free(ca);
	
	ca = cmime_address_parse_string(addr_string1);
	assert(strcmp(ca->name,addr_string1_name_part) == 0);
	assert(strcmp(ca->email,addr_string1_email_part) == 0);

	s = cmime_address_to_string(ca);
	assert(strcmp(s,addr_string1)==0);
	free(s);
	
	cmime_address_set_type(ca, CMIME_ADDRESS_TYPE_FROM);
	assert(cmime_address_get_type(ca) == CMIME_ADDRESS_TYPE_FROM);
	
	cmime_address_free(ca);
	return(0);
}