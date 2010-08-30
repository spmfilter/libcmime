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

#include <stdlib.h>
#include <string.h>

#include "cmime_address.h"

/* Creates a new CMIMEAddress_T object */
CMimeAddress_T *cmime_address_new(void) {
	CMimeAddress_T *ca;
	ca = (CMimeAddress_T *)calloc((size_t)1,sizeof(CMimeAddress_T));
	return(ca);
}

/* Set the display name of CMimeAdresse_T object */
void cmime_address_set_name(CMimeEmailAddress_T *ca, char *name) {
	ca->name = (char *)malloc(strlen(name) + 1);
	strcpy(ca->name,name);
}

/* Set the email address of CMimeAdresse_T object */
void cmime_address_set_address(CMimeAddress_T *ca, char *email) {
	ca->email = (char *)malloc(strlen(email) + 1);
	strcpy(ca->email, email);
}

/* Free used memory */
void cmime_address_free(CMimeAddress_T *ca) {
	if (ca->name != NULL)
		free(ca->name);
	if (ca->email != NULL)
		free(ca->email);
	free(ca);
}