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
