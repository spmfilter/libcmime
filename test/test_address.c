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

#include "../src/cmime_address.h"

int main (int argc, char const *argv[]) {
	CMimeAddress_T *ca = cmime_address_new();
	cmime_address_set_name(ca, "Axel Steiner");
	cmime_address_set_email(ca, "ast@treibsand.com");
	
	printf("%s\n",cmime_address_to_string(ca));
	
	cmime_address_free(ca);
	return 0;
}