#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/cmime_message.h"


void usage() {
    printf("\n");
    printf("libcmime - simple api demonstration\n");
    printf("-----------------------------------\n");
    printf("demonstrates: creation of new CMimeMessage_T object with cmime_message_create_skeleton function and add an attachment via cmime_message_add_attachment\n");
    printf("output: output is written to stdout if no output file is specified\n");
    printf("required parameter: -a /path/to/attachment\n");
    printf("optional parameter: -f /path/to/out_file\n");
    exit(0);
}


int main(int argc, char *argv[])
{
    // addresses can either be specified "just as an address" or like "John Doe <from@example.org>"
    char from[] = "from@example.org";
    char to[] = "to@example.org";
    char subject[] = "this is a subject";
    char body[] = "some very interesting body line";
    char *file = NULL;
    char *attachment = NULL;
    char *out = NULL;
    int option;
    int retval = 0;

    // initialization for new CMimeMessage_T object which is created by using cmime_message_create_skeleton
    CMimeMessage_T *message = NULL;

    // check command line parameters
    while((option = getopt(argc,argv,"hf:a:")) != EOF) {
        switch(option) {
            case 'f':
                asprintf(&file, "%s", optarg);
                break; 
            case 'a':
                asprintf(&attachment, "%s", optarg);
                break;
            case 'h':
                usage();
                break; 
            default:
                usage();
        }
    }
    
    // use helper function cmime_message_add_attachment() to add an attachment
    if(attachment != NULL) {
       
        // create a basic skeletion with cmime_message_create_skeleton
        message = cmime_message_create_skeleton(from,to,subject);
        
        // add the body to object
        cmime_message_set_body(message,body);
        cmime_message_add_attachment(message, attachment);

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

    } else {
        printf("you have to specify an attachment with -a\n");
        retval = -1;
    }

    // some clean up
    if(file != NULL)
        free(file);
    if(attachment != NULL)
        free(attachment);
    if(out != NULL)
        free(out);

    return retval;
}

