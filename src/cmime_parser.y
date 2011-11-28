%{
	#define _GNU_SOURCE
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include <string.h>

	#include "cmime_list.h"
	#include "cmime_header.h"
	#include "cmime_message.h"
	#include "cmime_flbi.h"
	#include "cmime_part.h"
	#include "cmime_internal.h"
%}

%locations 
%error-verbose
%pure-parser
%lex-param {void * scanner}
%parse-param {void * scanner}
%parse-param {CMimeMessage_T *msg}

%union {
	char *string;
	CMimeHeader_T *h;
	CMimePart_T *p;
	CMimeList_T *l;
}
%token <string> HEADERNAME HEADERBODY EMPTY_LINE PARTBODY LINE

%type <string> headers
%type <l> mime_headers
%type <h> header

%type <string> parts
%type <p> part

%type <string> body

%%

message:
	headers gap parts
	| headers body {
		CMimePart_T *p = cmime_part_new();
		cmime_part_set_content(p,$2);
		cmime_list_append(msg->parts,p);
	}
;
		
headers:
	header { cmime_list_append(msg->headers,$1); }
	| headers header { cmime_list_append(msg->headers,$2); }
;
	
header:
	HEADERNAME HEADERBODY {
		CMimeHeader_T *h = cmime_header_new();
		cmime_header_set_name(h,$1);
		cmime_header_set_value(h,$2,0);
		$$ = h;
	}
;
	
parts:
	part { cmime_list_append(msg->parts,$1); }
	| parts part { cmime_list_append(msg->parts,$2); } 
;
	
part:
	mime_headers body {
		CMimePart_T *p = NULL;
		p = (CMimePart_T *)calloc((size_t)1, sizeof(CMimePart_T));
		p->content = NULL;
		$$ = p; 
		printf("PART NEW\n");
		p->headers = $1;
		printf("SIZE: [%d]\n",p->headers->size);
	}
; 
	
mime_headers: 
	header { 
		CMimeList_T *l = NULL;
		cmime_list_new(&l,_cmime_internal_header_destroy);
		cmime_list_append(l,$1);
		$$ = l;
	}
	| mime_headers header { 
		cmime_list_append($1,$2); 
	}
;

body:
	PARTBODY {
		$$ = $1;
	}
;

gap:
	LINE {
		msg->gap = (char *)realloc(msg->gap,strlen(msg->gap) + strlen($1) + sizeof(char));
		strcat(msg->gap,$1);
	}
	| gap LINE {
		msg->gap = (char *)realloc(msg->gap,strlen(msg->gap) + strlen($2) + sizeof(char));
		strcat(msg->gap,$2);
	}
;
	
%%

void yyerror(YYLTYPE *yyllocp, void *scanner, CMimeMessage_T *msg, const char *s, ...) {
  va_list ap;
  va_start(ap, s);

	if(yyllocp->first_line)
		fprintf(stderr, "%d.%d-%d.%d: error: ", yyllocp->first_line, \
		yyllocp->first_column, yyllocp->last_line, yyllocp->last_column); 
		
	vfprintf(stderr, s, ap);
	fprintf(stderr, "\n");
}