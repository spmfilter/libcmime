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
%token <string> HEADER_NAME HEADER_CONTENT GAP_LINE BODY_CONTENT LINE POSTFACE_LINE PART_END

%type <l> headers
%type <l> mime_headers
%type <h> header

%type <l> parts
%type <p> part

%type <string> mime_body
%type <string> postface
%%

message:
	headers gap parts
/*	| headers gap parts POSTFACE_CONTENT {
		msg->postface = (char *)calloc((size_t)1,strlen($4) + sizeof(char));
		strcat(msg->postface,$4);
	} */
	| headers BODY_CONTENT {
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
	HEADER_NAME HEADER_CONTENT {
		CMimeHeader_T *h = cmime_header_new();
		cmime_header_set_name(h,$1);
		cmime_header_set_value(h,$2,0);
		$$ = h;
	}
;

parts:
	part { 
		printf("PART APPEND <-> [%s]\n",$1->boundary);
		cmime_list_append(msg->parts,$1);
		if ($1->boundary)
			$$ = $1->parts;
		else
			$$ = msg->parts; 
	}
	| parts part { 
		printf("PARTS APPEND <-> [%s]\n",$2->boundary);
	/*	cmime_list_append(msg->parts,$2); */
		cmime_list_append($$,$2); 
	} 
	| parts part PART_END {
		printf("PART END ohne postface\n");
		CMimeListElem_T *e = NULL;
		CMimePart_T *p = NULL;
		e = cmime_list_head(msg->parts);
		while(e != NULL) {
			p = (CMimePart_T *)cmime_list_data(e);
			if (cmime_flbi_cmp_boundary(p->boundary,$3,msg->linebreak)==2) {
				printf("APPEND SUB\n");
				cmime_list_append(p->parts,$2);
				$$ = msg->parts; 
				break;
			}
			e = e->next;
		}
	}
	| parts part PART_END postface {
		printf("PART END\n");
		CMimeListElem_T *e = NULL;
		CMimePart_T *p = NULL;
		e = cmime_list_head(msg->parts);
		while(e != NULL) {
			p = (CMimePart_T *)cmime_list_data(e);
			if (cmime_flbi_cmp_boundary(p->boundary,$3,msg->linebreak)==2) {
				printf("APPEND SUB\n");
				cmime_part_set_postface(p,$4);
				printf("APPEND POSTFACE: [%s]\n",p->postface);
				cmime_list_append(p->parts,$2);
				$$ = msg->parts; 
				break;
			}
			e = e->next;
		}
		
	}
;
	
part:
	mime_headers mime_body {
		CMimePart_T *p = cmime_part_new();
		cmime_part_set_content(p,$2);
		free($2);
		$$ = p; 
		p->headers = $1;
		cmime_flbi_check_part_boundary(p);
		if (p->boundary != NULL) {
			printf("THIS PART HAS SUBPARTS [%s]\n",p->boundary);
		}
	}
; 
	
mime_headers: 
	header { 
		CMimeList_T *l = NULL;
		cmime_list_new(&l,_cmime_internal_header_destroy);
		cmime_list_append(l,$1);
		$$ = l;
/*		printf("HEADER NEW [%s] <-> [%s]\n",$1->name,cmime_header_get_value($1,0)); */
	}
	| mime_headers header { 
/*		printf("HEADER APPEND [%s] <-> [%s]\n",$2->name,cmime_header_get_value($2,0)); */
		cmime_list_append($1,$2); 
	}
;
	
mime_body:
	LINE {
		$$ = (char *)calloc((size_t)1,strlen($1) + sizeof(char));
		strcat($$,$1); 
	}
	| mime_body LINE  {
		$$ = (char *)realloc($$,strlen($$) + strlen($2) + sizeof(char));
		strcat($$,$2);
	}
;

gap:
	GAP_LINE {
		msg->gap = (char *)realloc(msg->gap,strlen(msg->gap) + strlen($1) + sizeof(char));
		strcat(msg->gap,$1);
	}
	| gap GAP_LINE {
		msg->gap = (char *)realloc(msg->gap,strlen(msg->gap) + strlen($2) + sizeof(char));
		strcat(msg->gap,$2);
	}
;

postface:
	POSTFACE_LINE {
		$$ = (char *)calloc((size_t)1,strlen($1) + sizeof(char));
		strcat($$,$1); 
	}
	| postface POSTFACE_LINE {
		$$ = (char *)realloc($$,strlen($$) + strlen($2) + sizeof(char));
		strcat($$,$2);
	}
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