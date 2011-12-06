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
	
	#define YY_EXTRA_TYPE CMimeMessage_T *
	#define YY_USER_ACTION yylloc->first_line = yylineno;
%}

LF_RE								(\r\n)|(\r)|(\n)|(\x0c)
HEADERNAME_RE				([A-Za-z0-9-]*:\ )
HEADERBODY_RE				(.*({LF_RE}(\t|\ ).*)*{LF_RE}?)
BODY_RE							((.*({LF_RE}))*)
LINE_RE							(.*{LF_RE})

%%

{HEADERNAME_RE} {
	BEGIN(HEADER);
	yylval->string = strsep(&yytext,":");
	return HEADER_NAME;
}

	
<HEADER>{HEADERBODY_RE} {
	BEGIN(INITIAL);
	printf("HEADER: [%s]\n",yytext);
	
	if (yyextra->linebreak==NULL)
		yyextra->linebreak = strdup(_cmime_internal_determine_linebreak(yytext));
	
	cmime_flbi_check_boundary(yyextra,yytext);	
	
	yylval->string = cmime_string_strip(yytext);
	
	
	return HEADER_CONTENT; 	 
}
	/*
<HEADER>{LINE_RE} {
	BEGIN(INITIAL);
	printf("HEADER: [%s]\n",yytext);
	
	if (yyextra->linebreak==NULL)
		yyextra->linebreak = strdup(_cmime_internal_determine_linebreak(yytext));
	
	cmime_flbi_check_boundary(yyextra,yytext);	
	
	yylval->string = cmime_string_strip(yytext);
	
	
	return HEADER_LINE;
}
	
{CONTINUATION_RE} {
	printf("CONTINUES: [%s]\n",yytext);
	strtok(yytext, "\n");
	yylval->string = yytext;
	return HEADER_CONTINUATION; 
}
	*/
	
{LF_RE} {
	yyextra->gap = strdup(yytext); 
	if (yyextra->boundary) 
		BEGIN(GAP);
 	 else
		BEGIN(BODY);
}

<GAP>{LINE_RE} {
	if (strstr(yytext,yyextra->boundary))
		BEGIN(MIME_PART);
	else {
		yylval->string = yytext;
		return GAP_LINE;
	}
}

<BODY>{BODY_RE} {
	yylval->string = yytext;
	return BODY_CONTENT;
}

<MIME_CONTENT>{LINE_RE} {
	if (strstr(yytext,yyextra->boundary))
		BEGIN(MIME_PART);
	else {
		yylval->string = yytext;
		return LINE;
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

	yylval->string = cmime_string_strip(yytext);
	return HEADER_CONTENT;
}


. { } 

%%

int cmime_scanner_scan_file(CMimeMessage_T **message, FILE *fp) {
	yyscan_t scanner;
	
	if(yylex_init_extra((*message),&scanner)) { 
		fprintf(stderr, "error: no scanning today\n"); 
		return(-4); 
	}

	yyrestart(fp,scanner); 
	yyparse(scanner,(*message));  
	
	yylex_destroy(scanner);

	return(0);
}
