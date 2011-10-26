%{
	#define _GNU_SOURCE
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include <string.h>
	
	#include "cmime_string.h"
	#include "cmime_message.h"
	#include "cmime_list.h"
	#include "cmime_internal.h"
	
	extern int yylex();
	void yyerror(CMimeMessage_T *msg,char *s,...);
	extern int yylineno; 
%}

%parse-param {CMimeMessage_T *msg}

%union {
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
			/* printf("BODY\n"); */
		}
	;
	
header: 
		headerline { /* nothing */	}
	|	header headerline 
	;
	
headerline:
		HEADERNAME HEADERBODY {
			char *t = $2;
			if (msg->linebreak == NULL) 
				msg->linebreak = _cmime_internal_determine_linebreak(t);
			
			t = strsep(&t,msg->linebreak);
			_cmime_internal_set_linked_header_value(msg->headers,$1,t); 
			
		}
	| HEADERNAME HEADERBODY continuations {		
		_cmime_internal_set_linked_header_value(msg->headers,$1,$2);
		}
	;

continuations:
		CONTINUATION { /* nothing */ }
	| continuations CONTINUATION {
			$1 = cmime_string_chomp($1);
			$$ = $1; 
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