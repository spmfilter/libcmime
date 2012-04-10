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
    