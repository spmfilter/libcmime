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

#include "cmime_config.h"
#include "cmime_util.h"

/* get the mimetype */
char *cmime_util_get_mimetype(const char *filename) {
	char buf[128];
	FILE *fh;
	char *command = NULL;
	char *retval;

	assert(filename);
	
	/* build up the command string */
	command = (char *)calloc(strlen(FILE_EXECUTABLE) + strlen(filename) + 2, sizeof(char));
	strncpy(command, FILE_EXECUTABLE, strlen(FILE_EXECUTABLE));
	strncat(command, " ", 1);
	strncat(command, filename, strlen(filename));

	/* open the pipe and try to read command output */
	fh = popen(command, "r");
	if(fh == NULL) {
		return(NULL);
	}
	if((fgets(buf, 128, fh) == NULL)) {
		return(NULL);
	}

	if(command != NULL) {
		free(command);
	}

	/* copy command output from static buffer into string */
	retval =  (char *)calloc(strlen(buf) + 1, sizeof(char));
	strncpy(retval, buf, strlen(buf) - 1);
	return(retval);
}
