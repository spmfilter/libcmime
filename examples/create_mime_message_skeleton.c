#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../src/cmime_message.h"


void usage(void) {
	printf("\n\n");
	printf("simple api demonstration. this one creates a new CMimeMessage_T object with the help of the cmime_message_create_skeleton\n");
	printf("function. The output is written to stdout if no output file is specified with -f /path/to/output_file.txt\n\n");
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

