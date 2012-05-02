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

/*!
 * @file cmime_address.h
 * @brief Defines functions and structs for email address handling
 */

#ifndef _CMIME_ADDRESS_H
#define _CMIME_ADDRESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/*!
 * @enum CMimeAddressType_T 
 * @brief Possible types of email addresses
 */
typedef enum _CMimeAddressType {
	CMIME_ADDRESS_TYPE_TO, /**< message recipient */
	CMIME_ADDRESS_TYPE_CC, /**< message cc recipient */
	CMIME_ADDRESS_TYPE_BCC, /**< nessage bcc recipient */
	CMIME_ADDRESS_TYPE_FROM, /**< message sender */
} CMimeAddressType_T;

/*!
 * @struct CMimeAddress_T cmime_address.h
 * @brief Represents an email address
 */
typedef struct {
	char *name; /**< display name */
	char *email; /**< the email address */
	CMimeAddressType_T type; /**< type of address */
    int parsed;
} CMimeAddress_T;

/*!
 * @fn CMimeAddress_T *cmime_address_new(void)
 * @brief Creates a new CMimeAddress_T object
 * @returns CMimeAdress_T pointer, or NULL on failure
 */
CMimeAddress_T *cmime_address_new(void);

/*! 
 * @fn void cmime_address_set_name(CMimeAddress_T *ca, const char *name)
 * @brief Set the display name of CMimeAdresse_T object
 * @param ca CMimeAddress_T pointer
 * @param name the display name for the address
 */
void cmime_address_set_name(CMimeAddress_T *ca, const char *name);

/*! 
 * @fn void cmime_address_set_email(CMimeAddress_T *ca, const char *email)
 * @brief Set the email address of CMimeAdress_T object
 * @param ca CMimeAddress_T pointer
 * @param email email address
 */
void cmime_address_set_email(CMimeAddress_T *ca, const char *email);

/*!
 * @fn void cmime_address_set_type(CMimeAddress_T *ca, CMimeAddressType_T t)
 * @brief Set the address type of CMimeAdress_T object
 * @param ca CMimeAddress_T object
 * @param t CMimeAddressType_T type
 */
void cmime_address_set_type(CMimeAddress_T *ca, CMimeAddressType_T t);

/*! 
 * @fn char *cmime_address_to_string(CMimeAddress_T *ca)
 * @brief Allocates a string containing the contents of the CMimeAddress_T object.
 * @param ca CMimeAddress_T pointer
 * @returns the CMimeAddress_T object as an newly allocated string in rfc822 format.
 */ 
char *cmime_address_to_string(CMimeAddress_T *ca);

/*! 
 * @fn CMimeAddress_T *cmime_address_parse_string(const char *s)
 * @brief Parse given string and create CMimeAddress_T object
 * @param s string to parse
 * @returns a newly allocated CMimeAddress_T object, or NULL on failure
 */
CMimeAddress_T *cmime_address_parse_string(const char *s);

/*! 
 * @fn void cmime_address_free(CMimeAddress_T *ca)
 * @brief Free a CMimeAddress_T object 
 * @param ca CMimeAddress_T pointer
 */
void cmime_address_free(CMimeAddress_T *ca);

/*!
 * @def cmime_address_get_name(ca)
 * @returns name of CMimeAddress_T object
 */
#define cmime_address_get_name(ca) (ca->name)

/*!
 * @def cmime_address_get_email(ca)
 * @returns email address of CMimeAddress_T object
 */
#define cmime_address_get_email(ca) (ca->email)

/*!
 * @def cmime_address_get_type(ca)
 * @returns address type
 */
#define cmime_address_get_type(ca) (ca->type)

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_ADDRESS_H */
