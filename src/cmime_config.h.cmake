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
 * @file cmime_config.h
 * @brief CMime's config constants. 
 *
 */

#ifndef _CMIME_CONFIG_H
#define _CMIME_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * @def CMIME_VERSION
 * @returns CMime version
 */
#define CMIME_VERSION ${CMIME_VERSION_NUMBER}

/*!
 * @def CMIME_MAJOR_VERSION
 * @returns CMime's major version
 */
#define CMIME_MAJOR_VERSION ${CMIME_MAJOR_VERSION}

/*!
 * @def CMIME_MINOR_VERSION
 * @returns CMime's minor version
 */
#define CMIME_MINOR_VERSION ${CMIME_MINOR_VERSION}

/*!
 * @def CMIME_MICRO_VERSION
 * @returns CMime's micro version
 */
#define CMIME_MICRO_VERSION ${CMIME_MICRO_VERSION}

/*!
 * @def FILE_EXECUTABLE
 * @returns path to the file binary
 */
#define FILE_EXECUTABLE "${FILE_EXECUTABLE} -b --mime"

#ifdef __cplusplus
}
#endif

#endif /* _CMIME_CONFIG_H */
