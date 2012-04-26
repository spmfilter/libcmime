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
    asprintf(&command,"%s %s",FILE_EXECUTABLE,filename);

    /* open the pipe and try to read command output */
    fh = popen(command, "r");
    if(fh == NULL) {
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
        fclose(fh);
        return(retval);
    } else {
        fclose(fh);
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
    FILE *fp = NULL;
    int fd;
    
    assert(s);

    asprintf(&tempname,"%s/cmime_XXXXXX",P_tmpdir);
    fd = mkstemp(tempname);
    if (fd == -1) {
        perror("libcmime: error creating temporary file");
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
        mi = _split_combined_info(combined);
        free(combined);
    }
    
    
    if (remove(tempname) != 0) {
        free(tempname);
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
