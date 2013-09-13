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

#include "../src/cmime_base64.h"

#include "test_data.h"

int main (int argc, char const *argv[]) {
    char *s = NULL;
    char *s2 = NULL;
    FILE *fp = NULL;
    FILE *fp2 = NULL;
    long size = 0;
    char *in_data = NULL;
    char *out_data = NULL;
    char *in_64 = NULL;
    
    s = cmime_base64_encode_string(test_string1);
    assert(strcmp(s,test_string1_base64)==0);
    s2 = cmime_base64_decode_string(s);
    assert(strcmp(test_string1,s2)==0);
    free(s2);
    free(s);
    
    asprintf(&s,"%s/c0001.txt",SAMPLES_DIR);
    if ((fp = fopen(s, "rb")) == NULL) 
        return(-1);
        
    free(s);
    
    if (fseek(fp, 0, SEEK_END)!=0)
        return(-1);
        
    size = ftell(fp);
    rewind(fp); 
    in_data = (char*) calloc(sizeof(char), size + 20);
    fread(in_data, 1, size, fp);
    if(ferror(fp))
        return(-1);

    rewind(fp);
    if ((fp2 = fopen("test_base64.out","wb")) == NULL)
        return(-1);

    cmime_base64_encode_file(fp,fp2,0);
    in_64 = cmime_base64_encode_string(in_data);
    
    size = ftell(fp2);
    rewind(fp2);
    out_data = (char *)calloc(sizeof(char),size + 20);
    fp2 = freopen("test_base64.out","rb",fp2);
    fread(out_data,1,size,fp2);
    if (ferror(fp2))
        return(-1);

    assert(strcmp(in_64,out_data)==0);
    free(in_64);
    free(out_data);
    free(in_data);
    
    fclose(fp2);
    fclose(fp);
    
    if (remove("test_base64.out")!=0)
        return(-1);
    
    return(0);
}
