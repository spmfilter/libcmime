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
 * @file cmime_string.h
 * @brief Defines helper functions and structs for string handling
 *
 * This header is intended for use by libcmime only.
 *
 * @example test_string.c
 */

#ifndef _CMIME_STRING_H
#define _CMIME_STRING_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @fn char *cmime_string_strip(char *s)
 * @brief Strip leading/trailing spaces from string. 
 *   This function doesn't allocate or reallocate any memory; 
 *   it modifies string in place. 
 * @param s string with spaces
 * @returns string with removed spaces
 */
char *cmime_string_strip(char *s);

/*!
 * @fn char *cmime_string_chomp(char *s)
 * @brief remove trailing newline from string
 *   This function doesn't allocate or reallocate any memory;
 *   it modifies string in place.
 * @param s a string
 * @returns string with removed newline
 */
char *cmime_string_chomp(char *s);

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_STRING_H */
