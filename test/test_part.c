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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

#include "../src/cmime_part.h"
#include "../src/cmime_util.h"
#include "../src/cmime_internal.h"

#include "test_data.h"

int main (int argc, char const *argv[]) {
    CMimePart_T *part = NULL;
    char *s = NULL;
    char *out = NULL;
    char *out2 = NULL;
    FILE *fp = NULL;
    size_t size;

    part = cmime_part_new();
    assert(part);
    
    cmime_part_set_content_type(part,mime_type_string2);
    s = cmime_part_get_content_type(part);
    assert(strcmp(s,mime_type_string2)==0);

    // overwrite content type
    cmime_part_set_content_type(part,mime_type_string1);
    s = cmime_part_get_content_type(part);
    assert(strcmp(s,mime_type_string1)==0);
    
    cmime_part_set_content_disposition(part,mime_disposition_string1);
    s = cmime_part_get_content_disposition(part);
    assert(strcmp(s,mime_disposition_string1)==0);
    
    cmime_part_set_content_disposition(part,mime_disposition_string2);
    s = cmime_part_get_content_disposition(part);
    assert(strcmp(s,mime_disposition_string2)==0);

    cmime_part_set_content_transfer_encoding(part,mime_encoding_string1);
    s = cmime_part_get_content_transfer_encoding(part);
    assert(strcmp(s,mime_encoding_string1)==0);
    
    cmime_part_set_content_transfer_encoding(part,mime_encoding_string2);
    s = cmime_part_get_content_transfer_encoding(part);
    assert(strcmp(s,mime_encoding_string2)==0);
    
    cmime_part_set_content(part,test_content_string1);
    s = cmime_part_get_content(part);   
    assert(strcmp(s,test_content_string1)==0);
    
    out = cmime_part_to_string(part,NULL);
    cmime_part_free(part);
    
    /* now create a new mime part object from 
     * the old part string and compare */
    part = cmime_part_new();
    cmime_part_from_string(&part,out);

    out2 = cmime_part_to_string(part,NULL);
    assert(strcmp(out,out2)==0);
    free(out);
    free(out2);
    cmime_part_free(part); 
    
    /* create a part from file and compare with expected result */    
    part = cmime_part_new();
    asprintf(&s,"%s/c0001.txt",SAMPLES_DIR);
    cmime_part_from_file(&part,s,NULL);
    free(s);

    asprintf(&s,"%s/c0001_part.txt",SAMPLES_DIR);
    fp = fopen(s, "rb");
    if (fp == NULL) {
        perror("libcmime: error opening file");
        return(-1);
    }
    free(s);
    if (fseek(fp, 0, SEEK_END)!=0) {
        perror("libcmime: seek failed");
        return(-1);
    }
    
    size = ftell(fp);
    rewind(fp); 
    s = (char*) calloc(sizeof(char), size + sizeof(char));
    fread(s, size, 1, fp);
    if(ferror(fp)) {
        perror("libcmime: failed reading file");
        return(-1);
    }

    if (fclose(fp)!=0) {
        perror("libcmime: error closing file");
        return(-1);
    }

    out = cmime_part_to_string(part,NULL);

    assert(strcmp(out,s)==0);
    free(out);
    free(s);
        
    cmime_part_free(part);

    /* base 64 part */
    part = cmime_part_new();
    asprintf(&s,"%s/redball.png",SAMPLES_DIR);
    cmime_part_from_file(&part,s,LF);
    free(s);

    asprintf(&s,"%s/redball_part.txt",SAMPLES_DIR);
    fp = fopen(s, "rb");
    if (fp == NULL) {
        perror("libcmime: error opening file");
        return(-1);
    }
    free(s);
    if (fseek(fp, 0, SEEK_END)!=0) {
        perror("libcmime: seek failed");
        return(-1);
    }
    
    size = ftell(fp);
    rewind(fp); 
    s = (char*) calloc(sizeof(char), size + sizeof(char));
    fread(s, size, 1, fp);
    if(ferror(fp)) {
        perror("libcmime: failed reading file");
        return(-1);
    }

    if (fclose(fp)!=0) {
        perror("libcmime: error closing file");
        return(-1);
    }
    out = cmime_part_to_string(part,NULL);
    assert(strcmp(out,s)==0);

    free(out);
    free(s);

    cmime_part_free(part);

    return(0);
}
