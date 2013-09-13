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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../src/cmime_address.h"

#include "test_data.h"

int main (int argc, char const *argv[]) {
    char *s = NULL;
    CMimeAddress_T *ca = cmime_address_new();
    
    cmime_address_set_name(ca, addr_string1_name_part);
    cmime_address_set_email(ca, addr_string1_email_part);
    
    s = cmime_address_to_string(ca);
    assert(strcmp(s,addr_string1) == 0);
    free(s);
    cmime_address_free(ca);
    
    ca = cmime_address_parse_string(addr_string1);

    s = cmime_address_to_string(ca);
    assert(strcmp(s,addr_string1)==0);
    free(s);
    
    cmime_address_set_type(ca, CMIME_ADDRESS_TYPE_FROM);
    assert(cmime_address_get_type(ca) == CMIME_ADDRESS_TYPE_FROM);
    
    cmime_address_free(ca);
    return(0);
}

