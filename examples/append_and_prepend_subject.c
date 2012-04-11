#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/cmime_message.h"


void usage() {
    printf("\n");
    printf("libcmime - simple api demonstration\n");
    printf("-----------------------------------\n");
    printf("demonstrates: append or prepend something to the subject string\n");
    printf("output: output is written to stdout if no output file is specified\n");
    printf("optional parameter: -a string to append \n");
    printf("optional parameter: -p string to prepend\n");
    printf("optional parameter: -o /path/to/out_file\n");
    printf("\nexample: ./examples/append_and_prepend_subject -a \"aaa aaa aaa\" -p \"bbb bbb bbb\"");
}


int main(int argc, char *argv[])
{
    // addresses can either be specified "just as an address" or like "John Doe <from@example.org>"
    char from[] = "from@example.org";
    char to[] = "to@example.org";
    char subject[] = "this is a subject";
    char body[] = "some very interesting body line";
    char *file = NULL;
    char *out = NULL;
    int option;

    char *append = NULL;
    char *prepend = NULL;

    // initialization for new CMimeMessage_T object which is created by using cmime_message_create_skeleton
    CMimeMessage_T *message = NULL;

    // check command line parameters
    while((option = getopt(argc,argv,"ha:p:o:")) != EOF) {
        switch(option) {
            case 'a':
                asprintf(&append, "%s", optarg);
                break; 
            case 'p':
                asprintf(&prepend, "%s", optarg);
                break; 
            case 'o':
                asprintf(&file, "%s", optarg);
                break; 
            case 'h':
                usage();
                break;
            default:
                usage();
        }
    }

    // create a basic skeletion with cmime_message_create_skeleton
    message = cmime_message_create_skeleton(from,to,subject);
    // add the body to object
    cmime_message_set_body(message,body);

    // append something to subject, if given
    if(append != NULL)
        cmime_message_append_subject(message, append);

    //prepend something to subject, if given
    if(prepend != NULL)
        cmime_message_prepend_subject(message, prepend);

    // assign the email to out or write it to file (depending on cli options)
    if(file != NULL) {
        if(cmime_message_to_file(message,file) > 0) {
            printf("file created: %s\n", file);
        } else {
            printf("error writing file: %s\n", file);
        }
    } else {
        out = cmime_message_to_string(message);
        printf("%s\n", out);
    }

    // free the initialized object
    cmime_message_free(message);

    // some clean up
    if(append != NULL)
        free(append);
    if(prepend != NULL)
        free(prepend);
    if(file != NULL)
        free(file);
    if(out != NULL)
        free(out);


    return (0);
}

