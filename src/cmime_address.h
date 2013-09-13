/* libcmime - A C mime library
 * Copyright (C) 2013 SpaceNet AG and Axel Steiner <ast@treibsand.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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
    int parsed; /**< flag which indicates, if the object has been parsed */
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
