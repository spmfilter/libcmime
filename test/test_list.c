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

#include "../src/cmime_list.h"

#include "test_data.h"

void list_char_printer(CMimeListElem_T *elem,void *args) {
    assert(elem);
}

void list_destroy(void *data) {
    assert(data);
}

int main (int argc, char const *argv[]) {
    CMimeList_T *l;
    char *out;
    char *data;
    char *pop;
    CMimeListElem_T *e;

    if (cmime_list_new(&l,list_destroy)!=0) {
        printf("Failed to create new CMimeList_T\n");           
        return(-1);
    }

    if (cmime_list_append(l,test_string1)!=0) {
        printf("Failed to append data to CMimeList_T\n");
        return(-1);
    }
    
    out = (char *)cmime_list_data(cmime_list_head(l));
    assert(strcmp(test_string1,out)==0);
    
    if (cmime_list_size(l)!=1) {
        printf("Expected list size 1, but got %d",cmime_list_size(l));
        return(-1);
    }
    
    e = cmime_list_head(l);

    if (cmime_list_is_head(e)!=1) {
        printf("List element is not head\n");
        return(-1);
    }

    if (cmime_list_insert_next(l,e,test_string2)!=0) {
        printf("Failed to insert data to CMimeList_T\n");
        return(-1);
    }
    
    if (cmime_list_size(l)!=2) {
        printf("Expected list size 2, but got %d",cmime_list_size(l));
        return(-1);
    }
    
    e = cmime_list_tail(l);
    out = (char *)cmime_list_data(e);
    assert(strcmp(test_string2,out)==0);
    
    if (cmime_list_is_tail(e)!=1) {
        printf("List element is not tail\n");
        return(-1);
    }
        
    if (cmime_list_insert_prev(l,e,test_string3)!=0) {
        printf("Failed to insert data to CMimeList_T\n");
        return(-1);
    }
    
    out = (char *)cmime_list_data(cmime_list_prev(e));
    assert(strcmp(test_string3,out)==0);
    
    if (cmime_list_prepend(l,test_string4)!=0) {
        printf("Failed to prepend data to CMimeList_T\n");
        return(-1);
    }
    cmime_list_map(l,list_char_printer,NULL);

    e = cmime_list_head(l);
    if (cmime_list_remove(l,e,(void *)&data)!=0) {
        printf("Failed to remove element from CMimeList_T\n");
        return(-1);
    }

    pop = cmime_list_pop_head(l);
    assert(pop);
    assert(strcmp(pop,test_string1)==0);
    
    pop = cmime_list_pop_tail(l);
    assert(pop);
    assert(strcmp(pop,test_string2)==0);
    
    if (cmime_list_free(l)!=0) {
        printf("Failed to free CMimeList_T\n");
        return(-1);
    }

    return(0);
}
