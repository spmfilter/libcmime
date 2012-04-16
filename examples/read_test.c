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
                if ((*p=='"')||(*p==(unsigned char)10)||(*p==(unsigned char)13))
                    break;

                t = (char *)realloc(t,pos + (2 * sizeof(char)));
                t[pos++] = *p;
                p++;
            }
            t[pos] = '\0';
            pos = 0;
            cmime_string_list_insert(boundaries,t);
        }

        free(header);
    }
    
    return(boundaries);
}

void parse_input(char *buffer) {
    CMimeMessage_T *msg = cmime_message_new();
    CMimeStringList_T *boundaries = NULL;
    char *newline_char = NULL;
    char *empty_line = NULL;
    int i;
    int offset;
    int count = 0;
    char *marker = NULL;
    char *p = NULL;
    char *it = NULL;

    char *stripped = NULL;

    char *headers = NULL;
    char *begin_body = NULL;

    int len_boundary;

    
    char *msg_string = NULL;
    

    /* search newline and build header-body seperator */
    newline_char = _cmime_internal_determine_linebreak(buffer);
    asprintf(&empty_line,"%s%s",newline_char,newline_char);

    boundaries = _get_boundaries(buffer);
    printf("NUMBER: [%d]\n",cmime_string_list_get_count(boundaries));
    for(i=0; i < boundaries->count; i++) {
        printf("BOUNDARY [%s]\n",cmime_string_list_get(boundaries,i));
    }

    if (cmime_string_list_get_count(boundaries) > 0) {
        stripped = (char *)calloc(sizeof(char),sizeof(char));
        //asprintf(&marker,"%s--",newline_char);
        //printf("MARKER [%s]\n",marker);
        it = buffer;
        while((it = strstr(it,"--"))!=NULL) {
            for(i=0; i < cmime_string_list_get_count(boundaries); i++) {
                asprintf(&marker,"--%s",cmime_string_list_get(boundaries,i));
                if (strncmp(it,marker,strlen(marker))==0) {
                    
                    if (count == 0) {
                        offset = strlen(buffer) - strlen(it);    
                        stripped = (char *)realloc(stripped,strlen(stripped) + offset + sizeof(char));
                        strncat(stripped,buffer,offset);
                    } 

                    headers = _extract_headers(strstr(it,newline_char));
                    offset = strlen(marker) + strlen(headers);
                    p = (char *)calloc(offset + sizeof(char),sizeof(char));
                    //strncat(stripped,it,offset);
                    strncpy(p,it,offset);
                    //printf("P:\n[%s]\n",p);
                    free(p);     
                    
                    count++;
                    //p = (char *)calloc(len + sizeof(char),sizeof(char));
                    
                    //strncpy(p,buffer,len);
                    //printf("BUFFER:\n%s\n",p);
                    //free(p);
                    //printf("MATCH [%s]\n",bounds->boundaries[i]);

                    /* jump over matching boundary */
                    //headers = _extract_headers(strstr(it,newline_char));
                    //printf("HEADER:\n%s\n",headers);
                    break;
                }
                free(marker);
            }
            
            it++;
        }

        printf("STRIPPED:\n=======================================\n[%s]\n",stripped);


        /*
        for(i=0; i < bounds->count; i++) {
            asprintf(&marker,"--%s%s",bounds->boundaries[i],newline_char);
            printf("MARKER [%s]\n",marker);
            p = strstr(buffer,marker);
            len = strlen(buffer) - strlen(p);
            stripped = (char *)realloc(stripped,strlen(stripped) + len + sizeof(char));
            strncpy(stripped,buffer,len);
            printf("STRIPPEND\n[%s]\n",stripped);
        }*/
    } else {
        stripped = strdup(buffer);
    }

    /* copy headers of message */
    /*
    begin_body = strstr(buffer,dnl);
    free(dnl);

    len_headers = strlen(buffer) - strlen(begin_body);
    headers = malloc(len_headers + sizeof(char));
    strncpy(headers,buffer,len_headers);
  
    */

    cmime_string_list_free(boundaries);
  /*

    ret = cmime_scanner_scan_buffer(&msg, buffer);

    if (msg->boundary != NULL) {
        asprintf(&search_key,"%s--",nl);

        t = strstr(buffer,search_key);
        printf("%s\n",t);
        free(search_key);
    }
*/
//    printf("BOUNDARY [%s]\n",msg->boundary);

 //   msg_string = cmime_message_to_string(msg);
 //   printf("MESSAGE:\n%s\n",msg_string);
    //free(msg_string);
    //free(headers);

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