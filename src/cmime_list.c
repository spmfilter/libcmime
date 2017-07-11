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

#define _GNU_SOURCE
 
#include "cmime_list.h"

/* initializes a new empty doubly linked list */
int cmime_list_new(CMimeList_T **list, void (*destroy)(void *data)) {
    (*list) = (CMimeList_T *)calloc((size_t)1,sizeof(CMimeList_T));
    
    if(*list == NULL) {
        return(-1);
    }

    (*list)->size = 0;
    (*list)->head = NULL;
    (*list)->tail = NULL;
    (*list)->destroy = destroy;

    return(0);
}

/* destroy a complete list */
int cmime_list_free(CMimeList_T *list) {
    void *data;
    assert(list);

    while(cmime_list_size(list) > 0) {
        if(cmime_list_remove(list, cmime_list_tail(list), (void **)&data) == 0 &&
            list->destroy != NULL) {
                list->destroy(data);
            }
        }
 
    /* no more operations please... */
    free(list);
    return(0);
}

/* remove an element from the list */
int cmime_list_remove(CMimeList_T *list, CMimeListElem_T *elem, void **data) {
    /* no null element and no empty list */
    if(elem == NULL || cmime_list_size(list) == 0) {
        assert(list);
        assert(elem);
        return(-1);
    }
 
    *data = elem->data;
        
    /* handle removal of first element */
    if(elem == cmime_list_head(list)) {
        list->head = elem->next;
 
        if(list->head == NULL) {
            list->tail = NULL;
        } else {
            list->head->prev = NULL;
        }
    } else {
        elem->prev->next = elem->next;
 
        if(elem->next == NULL) {
            list->tail = elem->prev;
        } else {
            elem->next->prev = elem->prev;
        }
    }
 
    free(elem);
    list->size--;

    return(0);
}

/* remove tail element and return data pointer */
void* cmime_list_pop_tail(CMimeList_T *list) {
    void *data;
    int ret;
    
    assert(list);
    
    ret = cmime_list_remove(list,cmime_list_tail(list),&data);
 
    if(ret == 0) {
        return(data);
    } else {
        return(NULL);
    }
}

void *cmime_list_pop_head(CMimeList_T *list) {
    void *data;
    int ret;
 
    assert(list);
    
    ret = cmime_list_remove(list,cmime_list_head(list),&data);
 
    if(ret == 0) {
        return(data);
    } else {
        return(NULL);
    }
}

/* insert new elem next to given element elem */
int cmime_list_insert_next(CMimeList_T *list, CMimeListElem_T *elem, void *data) {
    CMimeListElem_T *new = (CMimeListElem_T *)calloc(1,sizeof(CMimeListElem_T));
    
    assert(list);
        
    if(new == NULL) {
        return(-1);
    }
    
    /* no null element if list not empty */
    if(elem == NULL && cmime_list_size(list) != 0) {
        free(new);
        return(-1);
    }
 
    new->data = data;
 
    if(cmime_list_size(list) == 0) {
        list->head = new;
        list->tail = new;
        new->next = NULL;
        new->prev = NULL;
    } else {
        new->next = elem->next;
        new->prev = elem;

        if(elem->next == NULL) {
            list->tail = new;
        } else {
            elem->next->prev = new;
        }

        elem->next = new; 
    }
 
    list->size++;
 
    return(0);
}

/* insert new element previous to given element elem */
int cmime_list_insert_prev(CMimeList_T *list, CMimeListElem_T *elem, void *data) {
    CMimeListElem_T *new = (CMimeListElem_T *)calloc(1,sizeof(CMimeListElem_T));
    
    assert(list);
    
    if(new == NULL) {
        return(-1);
    }
 
    /* no null element if list not empty */
    if(elem == NULL && cmime_list_size(list) != 0) {
        return(-1);
    }
 
    new->data = data;
 
    if(cmime_list_size(list) == 0) {
        list->head = new;
        list->tail = new;
        new->next = NULL;
        new->prev = NULL;
    } else {
        new->next = elem;
        new->prev = elem->prev;
 
        if(elem->prev == NULL) {
            list->head = new;
        } else {
            elem->prev->next = new;
        }
 
        elem->prev = new;
    }
 
    list->size++;
 
    return(0);
}

/* append to the end of a list */
int cmime_list_append(CMimeList_T *list, void *data) {
    assert(list);
    assert(data);
    return cmime_list_insert_next(list,cmime_list_tail(list),data);
}
 
/* prepend an element to the list */
int cmime_list_prepend(CMimeList_T *list, void *data) {
    assert(list);
    assert(data);
    return cmime_list_insert_prev(list,cmime_list_head(list),data);
}

/* apply function func to every element in the list */
void cmime_list_map(CMimeList_T *list, void(*func)(CMimeListElem_T *elem,void *args), void *args) {
    CMimeListElem_T *elem;
    
    assert(list);
    
    elem = cmime_list_head(list);
    while(elem != NULL) {
        func(elem,args);
        elem = elem->next;
    }
}

int cmime_list_map_new(CMimeList_T *list, CMimeList_T **new_list, void *(*func)(CMimeListElem_T *elem,
        void *args), void *args) {
    CMimeListElem_T *elem;
    int ret;
 
    assert(list);

    ret = cmime_list_new(new_list, NULL);
    if(ret != 0) {
        return(-1);
    }
 
    elem = cmime_list_head(list);
    while(elem != NULL) {
        cmime_list_append(*new_list,func(elem,args));
        elem = elem->next;
    }
 
    return(0);
}
