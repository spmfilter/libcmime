#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>

#include "../src/cmime_flbi.h"
#include "../src/cmime_message.h"
#include "../src/cmime_internal.h"

typedef struct {
    char **boundaries;
    int num_boundaries;
} CMimeBoundaries_T;


CMimeBoundaries_T *_get_boundaries(char *s) {
    CMimeBoundaries_T *boundaries = NULL;   
    char *t = NULL;
    char *header = NULL;
    char *p = NULL;
    char *p2 = NULL;
    int pos = 0;
    char *it = NULL;

    boundaries = (CMimeBoundaries_T *)calloc((size_t)1, sizeof(CMimeBoundaries_T));
    boundaries->num_boundaries = 0;
    boundaries->boundaries = (char **)calloc((size_t)1, sizeof(char *));
    
    while ((p = strcasestr(s,"content-type:"))!=NULL) {
        header = (char *)calloc(sizeof(char),sizeof(char));
        while(*p != '\0') {
            it = p;
            it++;

            /* break if char is a newline is not followed by a whitespace or tabulator */
            if (((*p==(unsigned char)10)||(*p==(unsigned char)13)) &&
                    ((*it!=(unsigned char)9)&&(*it!=(unsigned char)32)))
                break;

            header = (char *)realloc(header,pos + 1 + sizeof(char));
            header[pos++] = *p;
            p++;
        }
        header[pos] = '\0';
        pos = 0;

        s = p;
        
        if ((p = strcasestr(header,"boundary="))!=NULL) { 
            p2 = strstr(p,"=");
            if (*++p2=='"') 
                p2++;
                    
            t = (char *)calloc(sizeof(char),sizeof(char));
            while(*p2 != '\0') {
                if ((*p2=='"')||(*p2==(unsigned char)10)||(*p2==(unsigned char)13))
                    break;

                t = (char *)realloc(t,pos + (2 * sizeof(char)));
                t[pos++] = *p2;
                p2++;
            }
            t[pos] = '\0';
            pos = 0;
            boundaries->boundaries = (char **)realloc(boundaries->boundaries, (boundaries->num_boundaries + 1) * sizeof(char *));
            boundaries->boundaries[boundaries->num_boundaries++] = t;
        }

        free(header);
    }
    
    return boundaries;
}

void parse_input(char *buffer) {
    CMimeMessage_T *msg = cmime_message_new();
    char *nl = NULL;
    char *dnl;

    char *headers = NULL;
    char *begin_body = NULL;

    int len_headers;

//    char *search_key = NULL;
//    char *t = NULL;

    int i;
    char *msg_string = NULL;
    CMimeBoundaries_T *bounds = NULL;

    /* search newline and build header-body seperator */
    nl = _cmime_internal_determine_linebreak(buffer);
    asprintf(&dnl,"%s%s",nl,nl);

    bounds = _get_boundaries(buffer);

    /* copy headers of message */
    begin_body = strstr(buffer,dnl);
    free(dnl);

    len_headers = strlen(buffer) - strlen(begin_body);
    headers = malloc(len_headers + sizeof(char));
    strncpy(headers,buffer,len_headers);
  
    

    for(i=0; i < bounds->num_boundaries; i++) {
        if (bounds->boundaries[i]!=NULL) {
            printf("BOUNDARY: [%s]\n",bounds->boundaries[i]);
            free(bounds->boundaries[i]);
        }
    }

    free(bounds->boundaries);
    free(bounds); 
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
    free(msg_string);
    free(headers);

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