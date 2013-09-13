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

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../src/cmime_header.h"

#include "test_data.h"

int main (int argc, char const *argv[]) {
    CMimeHeader_T *h;
    char *s = NULL;
    int count;
    
    h = cmime_header_new();
    assert(h);
    
    cmime_header_set_name(h,header_string1_key);
    s = cmime_header_get_name(h);
    assert(strcmp(s,header_string1_key)==0);
    
    cmime_header_set_value(h,header_string1_value,0);
    count = cmime_header_get_count(h);
    assert(count == 1);

    // fetch first value
    s = cmime_header_get_value(h,0);
    assert(strcmp(s,header_string1_value)==0);
        
    s = cmime_header_to_string(h);
    assert(strcmp(s,header_string1)==0);
    free(s);
        
    // add another header value
    cmime_header_set_value(h,header_string2_value,0);
    count = cmime_header_get_count(h);
    assert(count == 2);
        
    // check first value again
    s = cmime_header_get_value(h,0);
    assert(strcmp(s,header_string1_value)==0);
        
    // check second value
    s = cmime_header_get_value(h,1);
    assert(strcmp(s,header_string2_value)==0);
    
    // now overwrite header values
    cmime_header_set_value(h,test_string1,1);
    s = cmime_header_get_value(h,0);
    assert(strcmp(s,test_string1)==0);
    
    count = cmime_header_get_count(h);
    assert(count == 1);
    
    cmime_header_free(h);
    return(0);
}   

