#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/cmime_message.h"


void usage() {
	printf("\n");
	printf("libcmime - simple api demonstration\n");
	printf("-----------------------------------\n");
	printf("demonstrates: creation of new CMimeMessage_T object with cmime_message_create_skeleton function.\n");
	printf("output: output is written to stdout if no output file is specified (-f /path/to/out_file.txt)\n");
	printf("\n");
	exit(0);
}


int main(int argc, char *argv[])
{
	char from[] = "from@example.org";
	char to[] = "to@example.org";
	char subject[] = "this is a subject";
	char body[] = "some very interesting body line";
	char *file = NULL;
	char *out = NULL;
	int option;

	// initialization for new CMimeMessage_T object which is created by using cmime_message_create_skeleton
	CMimeMessage_T *message = NULL;

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

	// create a basic skeletion with cmime_message_create_skeleton
	message = cmime_message_create_skeleton(from,to,subject);
	// add the body to object
	cmime_message_set_body(message,body);

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

	//some clean up
	if(file != NULL)
		free(file);
	if(out != NULL)
		free(out);

	return (0);
}

