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

#ifndef _TEST_DATA_H
#define _TEST_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

char addr_string1[] = "Foo Bar <foobar@example.com>";
char addr_string1_name_part[] = "Foo Bar";
char addr_string1_email_part[] = "foobar@example.com";

char test_string1[] = "Test string 1";
char test_string2[] = "Test string 2";
char test_string3[] = "Test string 3";
char test_string4[] = "  Test string 3   ";

char header_string1[] = "X-Foo: foobar";
char header_string1_key[] = "X-Foo";
char header_string1_value[] = "foobar";

char header_string2[] = "X-Test: raboof";
char header_string2_key[] = "X-Test";
char header_string2_value[] = "raboof";



char test_addr1[] = "Axel Steiner <ast@treibsand.com>";
char test_addr2[] = "Foo Bar <foo@foo.bar>";
char test_message_id[] = "4DF9E5EB.6080300@foo.bar";
char test_header[] = "X-Foo: foobar";
char test_header_name[] = "X-Foo";
char test_header_value[] = "foobar";
char test_header2[] = "X-Foo: raboof";
char test_header_value2[] = "raboof";

#ifdef __cplusplus
}
#endif

#endif /* _TEST_DATA_H */
