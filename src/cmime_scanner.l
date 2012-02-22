%option noyywrap 
%option reentrant 
%option nounput
%option bison-bridge
%option bison-locations
 
%option yylineno
	
%x HEADER
%x GAP
%x BODY 
%x MIME_PART
%x MIME_HEADER
%x MIME_CONTENT
%x POSTFACE

%{
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	
	#include "cmime_flbi.h"
	#include "cmime_string.h"
	#include "cmime_internal.h"
	#include "cmime_part.h"
	#include "cmime_list.h"
	#include "cmime_message.h"
	#include "cmime_parser.tab.h"
	
	#define YY_EXTRA_TYPE CMimeYYExtra_T *
	#define YY_USER_ACTION yylloc->first_line = yylineno;
%}

LF_RE               (\r\n)|(\r)|(\n)|(\x0c)
HEADERNAME_RE       ([A-Za-z0-9-]*:\ )
HEADERBODY_RE       (.*({LF_RE}(\t|\ ).*)*{LF_RE}?)
BODY_RE             ((.*({LF_RE}))*)
LINE_RE             (.*{LF_RE})
POSTFACE_RE         (.*--{LF_RE})

%%

{HEADERNAME_RE} {
	BEGIN(HEADER);
	yylval->string = strsep(&yytext,":");
	return HEADER_NAME;
}

<HEADER>{HEADERBODY_RE} {
	BEGIN(INITIAL);

	if (yyextra->message->linebreak==NULL)
		yyextra->message->linebreak = strdup(_cmime_internal_determine_linebreak(yytext));
	
	if (yyextra->message->boundary==NULL) {
		if (cmime_flbi_match_boundary(yytext)==1)
			yyextra->message->boundary = cmime_flbi_get_boundary(yytext);	
	}

	yylval->string = cmime_string_chomp(yytext);
	
	
	return HEADER_CONTENT; 	 
}
	
{LF_RE} {
	yyextra->message->gap = strdup(yytext); 
	if (yyextra->message->boundary) 
		BEGIN(GAP);
	else
		BEGIN(BODY);
}

<GAP>{LINE_RE} {
	if (cmime_flbi_cmp_boundary(yyextra->message->boundary,yytext,yyextra->message->linebreak)==1) {
		BEGIN(MIME_PART);
	} else {
		yylval->string = yytext;
		return GAP_LINE;
	}
}

<BODY>{BODY_RE} {
	yylval->string = yytext;
	return BODY_CONTENT;
}
	
<MIME_CONTENT>{POSTFACE_RE} {
	if (strstr(yytext,yyextra->message->boundary)) {
		BEGIN(POSTFACE);
		yylval->string = cmime_string_chomp(yytext);
		return PART_END; 
	} else if (strstr(yytext,yyextra->sub_part_boundary)) {
		BEGIN(POSTFACE);
		yylval->string = cmime_string_chomp(yytext);
		return PART_END;
	}
}
	
<MIME_CONTENT>{LINE_RE} {
	if (cmime_flbi_cmp_boundary(yyextra->message->boundary,yytext,yyextra->message->linebreak)==1)
		BEGIN(MIME_PART);
	else {
		if (cmime_flbi_cmp_boundary(yyextra->sub_part_boundary,yytext,yyextra->message->linebreak)==1) 
	 		BEGIN(MIME_PART);
	 	else {
	 		yylval->string = yytext;

			return LINE;
		}
	}
}

<MIME_PART>{HEADERNAME_RE} {
	BEGIN(MIME_HEADER);
	yylval->string = strsep(&yytext,":");
	return HEADER_NAME;
}

<MIME_PART>{LF_RE} {
	BEGIN(MIME_CONTENT);
}

<MIME_HEADER>{HEADERBODY_RE} {
	BEGIN(MIME_PART);
	yylval->string = cmime_string_chomp(yytext);
	if (cmime_flbi_match_boundary(yytext)==1) 
		yyextra->sub_part_boundary = cmime_flbi_get_boundary(yytext);

	return HEADER_CONTENT;
}

	/*	
<POSTFACE>{BODY_RE} {	
	yylval->string = yytext; 
	printf("POSTFACE: [%s]\n",yylval->string);
	return POSTFACE_CONTENT;
}
	*/
<POSTFACE>{LINE_RE} {
	if (cmime_flbi_cmp_boundary(yyextra->message->boundary,yytext,yyextra->message->linebreak)==1) {
		BEGIN(MIME_PART);
	} else {
		yylval->string = yytext;
		return POSTFACE_LINE;
	}
}

. { } 

%%

int cmime_scanner_scan_file(CMimeMessage_T **message, FILE *fp) {
	yyscan_t scanner;
	CMimeYYExtra_T *extra;

	extra = (CMimeYYExtra_T *)calloc((size_t)1, sizeof(CMimeYYExtra_T));
	extra->message = *message;

	if(yylex_init_extra(extra,&scanner)) { 
		fprintf(stderr, "error: no scanning today\n"); 
		return(-4); 
	}

	yyrestart(fp,scanner); 
	yyparse(scanner,(*message));  
	
	extra->message = NULL;
	if (extra->sub_part_boundary!=NULL)
		free(extra->sub_part_boundary);
	
	free(extra);
	yylex_destroy(scanner);

	return(0);
}
