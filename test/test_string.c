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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../src/cmime_string.h"

#include "test_data.h"

int main (int argc, char const *argv[]) {
	CMimeStringList_T *sl;
    CMimeStringList_T *sl2;
    
    char *s = NULL;
    
    sl = cmime_string_list_new();
    
    cmime_string_list_insert(sl,test_string1);
    cmime_string_list_insert(sl,test_string2);
    cmime_string_list_insert(sl,test_string3);
    assert(cmime_string_list_get_count(sl)==3);
        
    s = cmime_string_list_get(sl,0);
    assert(strcmp(s,test_string1)==0);
    
    s = cmime_string_list_get(sl,1);
    assert(strcmp(s,test_string2)==0);
    
    s = cmime_string_list_get(sl,2);
    assert(strcmp(s,test_string3)==0);
    
    cmime_string_list_free(sl);
    
    // check string split
    sl2 = cmime_string_split(test_string1," ",3);
    assert(cmime_string_list_get_count(sl2)==3);
    
    s = cmime_string_list_get(sl2,0);
    assert(strcmp(s,"Test")==0);
    
    s = cmime_string_list_get(sl2,1);
    assert(strcmp(s,"string")==0);
    
    s = cmime_string_list_get(sl2,2);
    assert(strcmp(s,"1")==0);
    
    cmime_string_list_free(sl2);
    
    // check string strip
    s = cmime_string_strip(test_string4);
    assert(strcmp(s,test_string3)==0);
	
	return(0);
}
