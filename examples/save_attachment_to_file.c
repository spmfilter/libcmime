#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/cmime_message.h"

void usage() {
    printf("\n");
    printf("libcmime - simple api demonstration\n");
    printf("-----------------------------------\n");
}

int main(int argc, char *argv[]) {
    int option;
    int retval = 0;
    char *in_file = NULL;
    char *out_file = NULL;
    int ret;
    int i=0;

    char *s = NULL;
    char *s2 = NULL;
    char *msg_string = NULL;

    // check command line parameters
    while((option = getopt(argc,argv,"hi:o:")) != EOF) {
        switch(option) {
            case 'i':
                asprintf(&in_file, "%s", optarg);
                break;
            case 'o':
                asprintf(&out_file, "%s", optarg);
                break;  
            case 'h':
                usage();
                break; 
            default:
                usage();
        }
    }



    if(in_file != NULL && out_file != NULL) {

        CMimeMessage_T *message = cmime_message_new();
        CMimePart_T *mp = NULL;
        CMimeListElem_T *elem = NULL;

        i = cmime_message_from_file(&message,in_file);
        if(i == 0) {

            elem = cmime_list_head(message->parts);
            while(elem != NULL) {
                mp = (CMimePart_T *)cmime_list_data(elem);
                ret = cmime_part_to_file(mp,out_file);
                printf("RET: [%d]\n",ret);
                elem = elem->next;
                break;
            }

        } else {
            printf("failed opening file [%s]\n", in_file);
            retval = -1;
        }
    } else {
        printf("you have to specify an input file with -i and an output file with -o\n");
        retval = -1;
    }


    if(in_file != NULL)
        free(in_file);
    if(out_file != NULL)
        free(out_file);


    return retval;


}