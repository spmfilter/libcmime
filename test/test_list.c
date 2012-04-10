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
