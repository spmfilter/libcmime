#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

#include "../src/cmime_flbi.h"
#include "../src/cmime_message.h"
#include "../src/cmime_internal.h"
#include "../src/cmime_string.h"

/*
typedef struct {
    char **boundaries;
    int count;
} CMimeBoundaries_T;
*/

/* extra header from given string */
char *_extract_headers(char *s) {
    char *it = NULL;
    char *nxt = NULL;
    char *headers = NULL;
    int pos = 0;

    headers = (char *)calloc(sizeof(char),sizeof(char));
    it = s;
    while(*it != '\0') {
        nxt = it;
        nxt++;
        if (((*it==(unsigned char)10)||(*it==(unsigned char)13)) && 
                ((*nxt==(unsigned char)10)||(*nxt==(unsigned char)13))) 
            break;
        
        headers = (char *)realloc(headers,pos + 1 + sizeof(char));
        headers[pos++] = *it;

        it++;
    }
    headers[pos] = '\0';

    return(headers);
}

/* find all boundaries in string */
CMimeStringList_T *_get_boundaries(char *s) {
    CMimeStringList_T *boundaries = NULL;   
    char *t = NULL;
    char *header = NULL;
    char *p = NULL;
    int pos = 0;
    char *it = NULL;
    char *nxt = NULL;

    boundaries = cmime_string_list_new();

    /* search for content-type headers, where new boundaries are defined */    
    while ((it = strcasestr(s,"content-type:"))!=NULL) {
        /* get all content-type header line(s) */
        header = (char *)calloc(sizeof(char),sizeof(char));
        while(*it != '\0') {
            nxt = it;
            nxt++;

            /* break if char is a newline is not followed by a whitespace or tabulator */
            if (((*it==(unsigned char)10)||(*it==(unsigned char)13)) &&
                    ((*nxt!=(unsigned char)9)&&(*nxt!=(unsigned char)32)))
                break;

            header = (char *)realloc(header,pos + 1 + sizeof(char));
            header[pos++] = *it;
            it++;
        }
        header[pos] = '\0';
        pos = 0;

        s = it;
        
        /* now search for boundary= in content-type header */
        if ((it = strcasestr(header,"boundary="))!=NULL) { 
            p = strstr(it,"=");
            if (*++p=='"') 
                p++;
                    
            t = (char *)calloc(sizeof(char),sizeof(char));
            /* if found, extract boundary and append to list */
            while(*p != '\0') {
                if ((*p=='"')||(*p==';')||(*p==(unsigned char)10)||(*p==(unsigned char)13))
                    break;

                t = (char *)realloc(t,pos + (2 * sizeof(char)));
                t[pos++] = *p;
                p++;
            }
            t[pos] = '\0';
            pos = 0;
            cmime_string_list_insert(boundaries,t);
            free(t);
        }

        free(header);
    }
    
    return(boundaries);
}

char *_match_boundary(CMimeStringList_T *boundaries, char *s) {
    int i;
    char *marker = NULL;

    for(i=0; i < cmime_string_list_get_count(boundaries); i++) {
        asprintf(&marker,"--%s--",cmime_string_list_get(boundaries,i));
        if (strncmp(s,marker,strlen(marker))==0) {
            return(marker);
        } else {
            free(marker);
            asprintf(&marker,"--%s",cmime_string_list_get(boundaries,i));
            if (strncmp(marker,s,strlen(marker))==0) {
                return(marker);
            }
            free(marker);
        }
    }
    return(NULL);
}

void parse_input(char *buffer) {
    CMimeMessage_T *msg = cmime_message_new();
    CMimeStringList_T *boundaries = NULL;
    CMimeStringList_T *mime_bodies = NULL;
    char *newline_char = NULL;
    char *empty_line = NULL;
    int len_empty_line;
    char *it = NULL;
    int offset;
    int count = 0;
    char *marker = NULL;
    int len_marker;
    char *headers = NULL;
    char *nxt = NULL;
    char *mime_body_start = NULL;
    char *mime_body = NULL;
    char *stripped = NULL;
    int ret;
    char *msg_string = NULL;
    CMimeListElem_T *elem = NULL;
    CMimePart_T *part = NULL;


    /* search newline and build header-body seperator */
    newline_char = _cmime_internal_determine_linebreak(buffer);
    asprintf(&empty_line,"%s%s",newline_char,newline_char);
    len_empty_line = strlen(empty_line);
    free(empty_line);

    boundaries = _get_boundaries(buffer);
    mime_bodies = cmime_string_list_new();

    if (cmime_string_list_get_count(boundaries) > 0) {
        stripped = (char *)calloc(sizeof(char),sizeof(char));
        it = buffer;
        while((it = strstr(it,"--"))!=NULL) {
            marker = _match_boundary(boundaries,it);
            if (marker != NULL) {
                /* check existing mime body marker */
                if (mime_body_start != NULL) {
                    /* there is a start marker.... */
                    offset = strlen(mime_body_start) - strlen(it);
                    if (offset > 0) {
                        /* calculate offset to next boundary and copy body */
                        mime_body = (char *)calloc(offset + sizeof(char),sizeof(char));
                        strncpy(mime_body,mime_body_start,offset);
                    } else {
                        /* this part seems to be empty */
                        mime_body = (char *)calloc(sizeof(char),sizeof(char));
                        mime_body[0] = '\0';
                    }

                    cmime_string_list_insert(mime_bodies, mime_body);
                    free(mime_body);

                    offset = -1;
                    mime_body_start = NULL;
                }

                len_marker = strlen(marker);
                // check if it's a closing boundary
                if ((marker[len_marker-2] == '-') && (marker[len_marker-1] == '-')) {
                    free(marker);
                    nxt = it + len_marker;
                    if ((nxt = strstr(nxt,"--"))!=NULL) {
                        if ((marker = _match_boundary(boundaries,nxt))!=NULL) {  
                            /* we've found another boundary, so it's not the last part. 
                             * Offset is the difference between previous found boundary and new one */
                            offset = strlen(it) - strlen(nxt); 
                            free(marker);
                        }
                    }

                    if (offset == -1)
                        /* since _match_boundary has not found another part behind,
                         * this seems to be the end...my best friend.
                         * Offset is the difference between previous found boundary and file end */
                        offset = strlen(it);                             
                } else {
                    free(marker);
                    if (count == 0) {
                        /* this is the first run, so we have to copy the message
                         * headers first */
                        offset = strlen(buffer) - strlen(it);    
                        stripped = (char *)realloc(stripped,strlen(stripped) + offset + sizeof(char));
                        strncat(stripped,buffer,offset);
                        offset = -1;
                    }

                    /* calculate offset for mime part headers */
                    headers = _extract_headers(strstr(it,newline_char));
                    
                    offset = len_marker + strlen(headers) + len_empty_line;
                    free(headers);

                    /* Now it's time to grap the mime part body.
                     * Set a marker where the mime part content begins*/
                    mime_body_start = it + offset;
                }
                
                count++;
                if (offset != -1) {
                    stripped = (char *)realloc(stripped,strlen(stripped) + offset + sizeof(char));
                    strncat(stripped,it,offset);
                    offset = -1;
                }
            }  
            it++;
        }
    } else {
        stripped = strdup(buffer);
    }
    
    /* parse the stripped message */
    ret = cmime_scanner_scan_buffer(&msg, stripped);

    /* now the wedding between CMimeMessage_T and stripped content */
    count = 0;
    elem = cmime_list_head(msg->parts);
    while(elem != NULL) {
        part = (CMimePart_T *)cmime_list_data(elem);
        mime_body = cmime_string_list_get(mime_bodies,count);
        if (mime_body!=NULL)
            part->content = mime_body;
        count++;
        elem = elem->next;
    }

    msg_string = cmime_message_to_string(msg);
    printf("%s",msg_string);
    free(msg_string);

    cmime_string_list_free(boundaries);
    free(stripped);
    cmime_message_free(msg); 
}


int main(int argc, char const *argv[]) {
    int fd;
    FILE *fp;
    struct stat sb;
    char *p;

    if (argc != 2)
        return(-1);

    /* open file for reading */
    fp = fopen(argv[1],"r");
    fd = fileno(fp);

    /* get size of file */
    if (fstat (fd, &sb) == -1) {
        perror ("fstat");
        return(-1);
    }
    if (!S_ISREG (sb.st_mode)) {
        fprintf (stderr, "%s is not a file\n", argv[1]);
        return(-1);
    }

    
    if (fd == -1) {
        perror("Error opening file for reading");
        exit(EXIT_FAILURE);
    }

    /* map file to memory */
    p = mmap (0, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
    if (p == MAP_FAILED) {
        close(fd);
        perror("Error mmapping the file");
        exit(EXIT_FAILURE);
    }
    
    fclose(fp);
   
    parse_input(p);
    
    /* delete mapping */
    if (munmap (p, sb.st_size) == -1) {
        perror ("munmap");
        return 1; 
    }

    
    return(0);
}