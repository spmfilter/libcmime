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
#include <string.h>

#include "../src/cmime_address.h"

#define ADDR_STRING "Axel Steiner <ast@treibsand.com>"
#define EXPTECTED_NAME_STRING "Axel Steiner"
#define EXPTECTED_EMAIL_STRING "ast@treibsand.com"

int main (int argc, char const *argv[]) {
	char *s = NULL;
	CMimeAddress_T *ca = cmime_address_new();
	int passed = 0;
	
	cmime_address_set_name(ca, "Axel Steiner");
	cmime_address_set_email(ca, "ast@treibsand.com");
	
	s = cmime_address_to_string(ca);
	if (strcmp(s,ADDR_STRING) != 0) {
		printf("Expected '%s', but got '%s'",ADDR_STRING,s);
		passed = 1;
	}
	
	free(s);
	cmime_address_free(ca);
	
	ca = cmime_address_parse_string(ADDR_STRING);
	if (strcmp(ca->name,EXPTECTED_NAME_STRING) != 0) {
		printf("Exptected '%s', but got '%s'\n",EXPTECTED_NAME_STRING,ca->name);			
		return -1;
	}
	
	if (strcmp(ca->email,EXPTECTED_EMAIL_STRING) != 0) {
		printf("Exptected '%s', but got '%s'\n",EXPTECTED_EMAIL_STRING,ca->email);			
		return -1;
	}
	cmime_address_free(ca);
	return (passed);
}