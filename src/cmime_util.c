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

#include "cmime_string.h"
#include "cmime_config.h"
#include "cmime_util.h"

CMimeInfo_T *_split_combined_info(char *combined) {
    CMimeInfo_T *mi = NULL;
    char *t1 = NULL;
    char *t2 = NULL;
    int len1, len2;

    mi = cmime_util_info_new();
    mi->combined = strdup(combined);
    len1 = strlen(combined);
    t1 = strchr(combined,';');
    len2 = strlen(t1);
    mi->mime_type = (char *)calloc((len1 - len2) + sizeof(char),sizeof(char));
    strncpy(mi->mime_type,combined,len1 - len2);
    mi->mime_type[strlen(mi->mime_type)] = '\0';

    t2 = strchr(t1,'=');
    t2++;
    mi->mime_encoding = (char *)calloc(strlen(t2) + sizeof(char), sizeof(char));
    strcpy(mi->mime_encoding,t2);
    
    return(mi);
}

/* get the mimetype */
char *cmime_util_get_mimetype(const char *filename) {
    char *buf = NULL;
    size_t st = 0;
    FILE *fh = NULL;
    char *command = NULL;
    char *retval = NULL;
    
    assert(filename);
    
    /* build up the command string */
    asprintf(&command,"%s '%s'",FILE_EXECUTABLE,filename);

    /* open the pipe and try to read command output */
    fh = popen(command, "r");
    if(fh == NULL) {
        free(command);
        return(NULL);
    }
    free(command);
    if(getline(&buf,&st,fh) > 0) {
        /* copy command output from static buffer into string */
        retval =  (char *)calloc(strlen(buf) + sizeof(char), sizeof(char));
        buf = cmime_string_chomp(buf);
        strncpy(retval, buf, strlen(buf));
        retval[strlen(retval)] = '\0';
        free(buf);
        pclose(fh);
        return(retval);
    } else {
        pclose(fh);
        return(NULL);
    }
}

CMimeInfo_T *cmime_util_info_new(void) {
    CMimeInfo_T *mi = NULL;
    mi = (CMimeInfo_T *)calloc((size_t)1,sizeof(CMimeInfo_T));
    mi->mime_type =  NULL;
    mi->mime_encoding = NULL;
    mi->combined = NULL;
    return(mi);
}

void cmime_util_info_free(CMimeInfo_T *mi) {
    assert(mi);

    if (mi->mime_type != NULL)
        free(mi->mime_type);
    
    if (mi->mime_encoding != NULL)
        free(mi->mime_encoding);

    if (mi->combined != NULL)
        free(mi->combined);

    free(mi); 
}

CMimeInfo_T *cmime_util_info_get_from_string(const char *s) {
    CMimeInfo_T *mi = NULL;
    char *tempname = NULL;
    char *combined = NULL;
    char wanted = ';';
    FILE *fp = NULL;
    int fd;
    
    assert(s);

    asprintf(&tempname,"%s/cmime_XXXXXX",P_tmpdir);
    fd = mkstemp(tempname);
    if (fd == -1) {
        perror("libcmime: error creating temporary file");
        free(tempname);
        return(NULL);
    }

    fp = fdopen(fd,"wb");
    if (fp == NULL) {
        perror("libcmime: error opening temporary file");
        free(tempname);
        return(NULL);
    }
    if (fwrite(s,strlen(s),1,fp) <=0 ) {
        fclose(fp);
        free(tempname);
        return(NULL);
    }
    fclose(fp);

    combined = cmime_util_get_mimetype(tempname);
    if (combined != NULL) {
        if (strchr(combined,wanted) != NULL) {
            mi = _split_combined_info(combined);
        } else {
            mi = cmime_util_info_new();
            mi->combined = strdup(combined);            
        }
        free(combined);
    }
    
    
    if (remove(tempname) != 0) {
        free(tempname);
        cmime_util_info_free(mi);
        perror("libcmime: failed to remove temporary file");
        return(NULL);
    }
    free(tempname);
    return(mi);
}

CMimeInfo_T *cmime_util_info_get_from_file(const char *filename) {
    CMimeInfo_T *mi = NULL;
    char *combined = NULL;

    assert(filename);

    combined = cmime_util_get_mimetype(filename);
    if (combined != NULL) {
        mi = _split_combined_info(combined);
        free(combined);
    }

    return(mi);   
}

int cmime_util_rand()
{
    static unsigned seed = 0;
    int result;
    
    if(seed == 0)
        seed = time((time_t*)0) ^ getpid();

    result = rand_r(&seed);
    return result;
}

