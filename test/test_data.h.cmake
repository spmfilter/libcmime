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

#ifndef _TEST_DATA_H
#define _TEST_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

#define SAMPLES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test/samples"

char addr_string1[] = "Foo Bar <foobar@example.com>";
char addr_string1_name_part[] = "Foo Bar";
char addr_string1_email_part[] = "<foobar@example.com>";

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
