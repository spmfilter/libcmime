%{
	#define _GNU_SOURCE
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include <string.h>
	
	#include "cmime_message.h"
	#include "cmime_list.h"
	#include "cmime_internal.h"
	
	extern int yylex();
	void yyerror(CMimeMessage_T *msg,char *s,...);
	extern int yylineno; 
%}

%parse-param {CMimeMessage_T *msg}

%union {
	CMimeMessage_T *m;
	CMimeHeader_T *h;
	char *string;
}

%token <string> HEADERNAME HEADERBODY EMPTY_LINE LINE CONTINUATION 
%type <string> body
%type <string> headerline

%type <string> continuations
%type <m> message
%type <h> header

%%

message:
		header EMPTY_LINE body { 
			printf("BODY\n");
		}
	;
	
header: 
		headerline { 
			printf("HEADERLINE: [%s]\n",$1);
		}
	|	header headerline { 
			printf("HEADERLINE LOOP: [%s]\n",$2);
		}
	;
	
headerline:
		HEADERNAME HEADERBODY {
			if (msg->linebreak == NULL)
				msg->linebreak = _cmime_internal_determine_linebreak($2);
			
		//	char *t;
		//	t = 
		//	$2 = strsep(&yytext,);
		//	$2 = str
			_cmime_internal_set_linked_header_value(msg->headers,$1,$2); 
			printf("ADDED HEADER [%s] [%s]\n",$1,$2);
		}
	| HEADERNAME HEADERBODY continuations {
		char *t;
		asprintf(&t,"%s%s",$2,$3);
		_cmime_internal_set_linked_header_value(msg->headers,$1,t);
		printf("T: [%s]\n",t);
//		printf("$1: [%s] $2: [%s] $3 [%s]\n",$1,$2,$3);
/*			char *t;
			char *nl;
			nl = _cmime_internal_determine_linebreak($3);
			asprintf(&t,"%s%s%s",$2,nl,$3);
			printf("C: [%s]\n",t);
			_cmime_internal_set_linked_header_value(msg->headers,$1,t);
			free(t); */
		}
	;

continuations:
		CONTINUATION {
			printf("CONTINUATION [%s]\n",$1);
		}
	| continuations CONTINUATION {
		printf("continuations [%s] [%s]\n",$1, $2);
	/*		char *t;
			asprintf(&t,"%s%s",$1,$2);	
			$$ = t;
			printf("continuations [%s] [%s]\n",$1, $2);
			printf("T2 [%s]\n",t); */
		}
	;
	
body:
		LINE {
			
		}
	|	body LINE {  }
	;
	
%%

void yyerror(CMimeMessage_T *msg,char *s,...) {
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}