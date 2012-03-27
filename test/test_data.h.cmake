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

#ifndef _TEST_DATA_H
#define _TEST_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#define SAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/samples"

char addr_string1[] = "Foo Bar <foobar@example.com>";
char addr_string1_name_part[] = "Foo Bar";
char addr_string1_email_part[] = "foobar@example.com";

char addr_string2[] = "Example User <user@example.com>";
char addr_string2_name_part[] = "Example User";
char addr_string2_email_part[] = "user@example.com";

char test_string1[] = "Test string 1";
char test_string1_base64[] = "VGVzdCBzdHJpbmcgMQ==";
char test_string2[] = "Test string 2";
char test_string3[] = "Test string 3";
char test_string4[] = "  Test string 3   ";

char header_string1[] = "X-Foo: foobar";
char header_string1_2[] = "X-Foo: raboof";
char header_string1_key[] = "X-Foo";
char header_string1_key_lower[] = "x-foo";
char header_string1_value[] = "foobar";

char header_string2[] = "X-Test: raboof";
char header_string2_key[] = "X-Test";
char header_string2_value[] = "raboof";

char message_id_string[] = "4DF9E5EB.6080300@foo.bar";
char message_id_string_out[] = "<4DF9E5EB.6080300@foo.bar>";

char mime_type_string1[] = "text/plain; charset=us-ascii";
char mime_type_string2[] = "multipart/mixed";

char mime_disposition_string1[] = "attachment; filename=\"attachment.zip\"";
char mime_disposition_string2[] = "inline";

char mime_encoding_string1[] = "base64";
char mime_encoding_string2[] = "quoted-printable";

char test_content_string1[] = "\
Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo \
ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis \
dis parturient montes, nascetur ridiculus mus. Donec quam felis, \
ultricies nec, pellentesque eu, pretium quis, sem. Nulla consequat massa \
quis enim. Donec pede justo, fringilla vel, aliquet nec, vulputate eget, \
arcu. In enim justo, rhoncus ut, imperdiet a, venenatis vitae, justo. \
Nullam dictum felis eu pede mollis pretium.";

char test_boundary[] = "--Part_5-743799170";

char quoted_printable_string1[] = "ü";
char quoted_printable_string1_encoded[] = "ü";

#ifdef __cplusplus
}
#endif

#endif /* _TEST_DATA_H */
