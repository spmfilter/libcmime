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


#ifndef _CMIME_ADDRESS_H
#define _CMIME_ADDRESS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A structure to represent an email address
 */

typedef struct {
	char *name; /**< display name */
	char *email; /**< the email address */
} CMimeAddress_T;

/** Creates a new CMimeAddress_T object
 *
 * \returns CMimeAdress_T pointer, or NULL on failure
 */
CMimeAddress_T *cmime_address_new(void);

/** Set the display name of CMimeAdresse_T object
 *
 * \param ca CMimeAddress_T pointer
 * \param name the display name for the address
 */
void cmime_address_set_name(CMimeAddress_T *ca, char *name);

/** Set the email address of CMimeAdresse_T object
 *
 * \param ca CMimeAddress_T pointer
 * \param email email address
 */
void cmime_address_set_email(CMimeAddress_T *ca, char *email);

/** Allocates a string containing the contents of the CMimeAddress_T object.
 *
 * \param ca CMimeAddress_T pointer
 *
 * \returns the CMimeAddress_T object as an allocated string in rfc822 format.
 */ 
char *cmime_address_to_string(CMimeAddress_T *ca);

/** Parse given string and create CMimeAddress_T object
 *
 * \param s string to parse
 *
 * \returns a newlly allocated CMimeAddress_T object, or NULL on failure
 */
CMimeAddress_T *cmime_address_parse_string(char *s);

/** Free a CMimeAddress_T object 
 *
 * \param ca CMimeAddress_T pointer
 */
void cmime_address_free(CMimeAddress_T *ca);

#ifdef __cplusplus
}
#endif

#endif