#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/cmime_message.h"

void usage() {
    printf("\n");
    printf("libcmime - simple api demonstration\n");
    printf("-----------------------------------\n");
    printf("demonstrates: manual creation of new CMimeMessage_T mime message\n");
    printf("output: output is written to stdout if no output file is specified (-f /path/to/out_file.txt)\n");
    printf("\n");
    exit(0);
}


int main(int argc, char *argv[]) {

    // addresses can either be specified "just as an address" or like "John Doe <from@example.org>"
    char from[] = "from@example.org";
    char to[] = "to@example.org";
    char *file = NULL;
    char *out = NULL;
    char *msgid = NULL;
    int option;

    // check command line parameters
    while((option = getopt(argc,argv,"hf:")) != EOF) {
        switch(option) {
            case 'f':
                asprintf(&file, "%s", optarg);
                break; 
            case 'h':
                usage();
                break; 
            default:
                usage();
        }
    }


    CMimeMessage_T *message = cmime_message_new();
 
    // set the sender of the message
    cmime_message_set_sender(message,from);

    // add an recipient, this can also be done with cmime_message_add_recipient_to
    cmime_message_add_recipient(message, to, CMIME_ADDRESS_TYPE_TO);

    // set subject
    cmime_message_set_subject(message, "This is an exmaple");

    // generate date header
    cmime_message_set_date_now(message);

    // generate a message id and add it to our message
    msgid = cmime_message_generate_boundary();
    cmime_message_set_message_id(message, msgid);
    
    // add content to the body
    cmime_message_set_body(message,"This is the message body");

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
    if(out != NULL)
        free(out);
    if(msgid != NULL)
        free(msgid);
    if(file != NULL)
        free(file);
    

    return (0);
}
