%{
	#include <stdio.h>
	extern int yylex();
	void yyerror(char *s, ...);
%}

%union {
	char *string;
}

%token <string> HEADERNAME HEADERBODY EMPTY_LINE LINE CONTINUATION 
%type <string> header
%type <string> body
%type <string> headerline
%type <string> continuations

%%

input:
		message
	|	input message
	;

message:
		header EMPTY_LINE body {
			printf("BODY\n");
		}
	;
	
header: 
		headerline { printf("HEADERLINE"); }
	|	header headerline { printf("HEADER\n"); }
	;
	
headerline:
		HEADERNAME HEADERBODY {
			printf("HEADERBODY");
		}
	| HEADERNAME HEADERBODY continuations {
			printf("continuations\n");
		}
	;

continuations:
		CONTINUATION {
			printf("CONTINUATION\n");
		}
	| continuations CONTINUATION {
			printf("BLA\n");
		}
	;
	
body:
		LINE {
			printf("LINE");
		}
	|	body LINE { printf("BODY"); }
	;
	
%%