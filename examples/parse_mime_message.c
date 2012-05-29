#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/cmime_message.h"
#include "../src/cmime_address.h"
#include "../src/cmime_list.h"
#include "../src/cmime_string.h"


void usage() {
    printf("\n");
    printf("libcmime - simple api demonstration\n");
    printf("-----------------------------------\n");
    printf("demonstrates: parsing an email message \n");
    printf("output: output is written to stdout if no output file is specified\n");
    printf("required parameter: -i /path/to/input_file\n");
}

int main(int argc, char *argv[])
{
    int option;
    int retval = 0;
    char *in_file = NULL;
    int i=0;

    char *s = NULL;
    char *s2 = NULL;
    char *msg_string = NULL;

    // check command line parameters
    while((option = getopt(argc,argv,"hi:")) != EOF) {
        switch(option) {
            case 'i':
                asprintf(&in_file, "%s", optarg);
                break; 
            case 'h':
                usage();
                break; 
            default:
                usage();
        }
    }

    if(in_file != NULL) {

        CMimeMessage_T *message = cmime_message_new();
       // CMimeHeader_T *header = NULL;
        //CMimeList_T *recipient_list = NULL;
        CMimeListElem_T *elem = NULL;
        CMimeAddress_T *ca = NULL;

        i = cmime_message_from_file(&message,"/home/werner/libcmime/samples/m1006.txt",0);
        
        if(i == 0) {
            printf("Message summary:\n=========================================\n");
            s = cmime_message_get_sender_string(message);
            printf("Sender: [%s]\n",s);
            free(s);

            printf("Recipients (%d):\n",message->recipients->size);
            elem = cmime_list_head(message->recipients);
            while(elem != NULL) {
                ca = (CMimeAddress_T *)cmime_list_data(elem);
                s = cmime_address_to_string(ca);
                s2 = cmime_string_strip(s);
                printf("- [%s]\n",s2);
                free(s);
                elem = elem->next;
            }

            printf("Mime Version: [%s]\n",cmime_message_get_mime_version(message));
            printf("Content ID: [%s]\n",cmime_message_get_content_id(message));
            printf("Content Type: [%s]\n",cmime_message_get_content_type(message));
            printf("Number of message headers: [%d]\n",message->headers->size);
            printf("Number of mime parts: [%d]\n",cmime_message_part_count(message));
            printf("=========================================\n\n");

            msg_string = cmime_message_to_string(message);
            printf("%s\n",msg_string);
            free(msg_string);
            cmime_message_free(message);

        } else {
            printf("failed opening file [%s]\n", in_file);
            retval = -1;
        }
    } else {
        printf("you have to specify an input file with -i\n");
        retval = -1;
    }

    if(in_file != NULL)
        free(in_file);


	return retval;
}





