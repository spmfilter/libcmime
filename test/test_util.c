/* libcmime - A C mime library
 * Copyright (C) 2012 SpaceNet AG and Axel Steiner <ast@treibsand.com>
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

#include "../src/cmime_util.h"

#include "test_data.h"

#define EXPECTED_MIMETYPE "text/x-c; charset=utf-8"
#define EXPECTED_MIMETYPE2 "text/plain; charset=utf-8"

int main (int argc, char const *argv[]) {
	char *out = NULL;
	
	out = cmime_util_get_mimetype("../../test/test_data.h");
	assert((strcmp(EXPECTED_MIMETYPE,out)==0)||(strcmp(EXPECTED_MIMETYPE2,out)==0));
	free(out);
	
	return(0);
}	
	