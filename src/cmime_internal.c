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

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "cmime_internal.h"
#include "cmime_header.h"

void _cmime_internal_header_destroy(void *data) {
	assert(data);
	CMimeHeader_T *header = (CMimeHeader_T *)data;
	cmime_header_free(header);
}

char *_cmime_internal_determine_linebreak(const char *s) {
	assert(s);
	
	if (strstr(s,CRLF)!=NULL)
		return(CRLF);
	else if(strstr(s,LF)!=NULL)
		return(LF);
	else if(strstr(s,CR)!=NULL)
		return(CR);
	else
		return(NULL);
}
