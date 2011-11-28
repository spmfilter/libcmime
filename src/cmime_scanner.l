%option noyywrap 
%option reentrant 
%option nounput
%option bison-bridge
%option bison-locations
 
%option yylineno
	
%x HEADER
%x BODY 
%x GAP
%x MIME_PART
%x PART_HEADER

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
HEADERBODY_RE				(.*({LF_RE}(\t|\s).*)*{LF_RE}?)
BODY_RE							((.*({LF_RE}))*)
LINE_RE							(.*{LF_PATTERN})




LF_PATTERN							(\r\n)|(\r)|(\n)|(\x0c)
 
HEADERNAME_PATTERN			(^[A-Za-z0-9-]*:\ ) 

GAP_PATTERN							(^{LF_PATTERN})
CONTINUATION_PATTERN		([ \t].*{LF_PATTERN})
LINE_PATTERN						(.*{LF_PATTERN})
BOUNDARY_PATTERN				([ \t]*.*boundary=(.*){LF_PATTERN})

HEADERCONTENT_PATTERN		(.*\n((\t*)|(\s*).*\n)*)

%%

{HEADERNAME_RE} {
	BEGIN(HEADER);
	yylval->string = strsep(&yytext,":");
	return HEADERNAME;
}

<HEADER>{HEADERBODY_RE} {
	BEGIN(INITIAL);
	if (yyextra->linebreak==NULL)
		yyextra->linebreak = strdup(_cmime_internal_determine_linebreak(yytext));
	
	cmime_flbi_check_boundary(yyextra,yytext);	
	
	yylval->string = cmime_string_strip(yytext);
	return HEADERBODY; 	 
}

{LF_RE} {
	yyextra->gap = strdup(yytext); 
	if (yyextra->boundary)
		BEGIN GAP;
 	else
		BEGIN BODY;
}

<BODY>{BODY_RE} {
	yylval->string = yytext;
	return PARTBODY;
}

<GAP>{LINE_RE} {
	if (strstr(yytext,yyextra->boundary))
		BEGIN MIME_PART;
	else {
		yylval->string = yytext;
		return LINE;
	}
}

<MIME_PART>{HEADERNAME_RE} {
	BEGIN PART_HEADER;
	yylval->string = strsep(&yytext,":");
	return HEADERNAME;
}

<MIME_PART>{LF_RE} {
	BEGIN BODY;
}

<PART_HEADER>{HEADERBODY_RE} {
	BEGIN(MIME_PART);

	yylval->string = cmime_string_strip(yytext);
	return HEADERBODY;
}



















 /*
{HEADERNAME_PATTERN} {
	BEGIN(HEADER);
	printf("HEADERNAME [%s]\n", yytext);
}

<HEADER>{HEADERCONTENT_PATTERN} {
		printf("matched next header\n"); 
}

<HEADER>{HEADERNAME_PATTERN} {
	printf("AHA [%s]\n",yytext);
	BEGIN(INITIAL);
}

	
<BODY>{LINE_PATTERN}	{
	yylval->string = yytext;
	return LINE;
}

{HEADERNAME_PATTERN} { 
	BEGIN HEADER;
	yylval->string = strsep(&yytext,":");
	return HEADERNAME;   
}

<HEADER>{LINE_PATTERN} {
	BEGIN INITIAL;
	yylval->string = yytext;
	
	return HEADERBODY;  
}

{BOUNDARY_PATTERN} {
	yyextra->boundary = _extract_boundary(yytext);  
	yylval->string = yytext; 
	return CONTINUATION; 
}

{CONTINUATION_PATTERN}	{
	yylval->string = yytext;
	return CONTINUATION;   
}

{GAP_PATTERN} {
	BEGIN GAP;
	printf("GAP: [%s]\n",yytext);
	return GAP;
}

{LF_PATTERN} {
	BEGIN BODY;
	return EMPTY_LINE;   
}
	*/

. { } 

	

%%

CMimeMessage_T *cmime_scanner_scan_file(const char *filename) {
	CMimeMessage_T *msg = cmime_message_new();
	yyscan_t scanner;
	FILE *fp = NULL;
	
	if(yylex_init_extra(msg,&scanner)) { 
		fprintf(stderr, "error: no scanning today\n"); 
		return(NULL); 
	}
	
	if ((fp = fopen(filename, "rb")) == NULL) {
		fprintf(stderr, "error: failed opening %s\n",filename); 
		return(NULL);
	}
	
	yyrestart(fp,scanner); 
	yyparse(scanner,msg);  
	
	fclose(fp); 
	yylex_destroy(scanner);

	return(msg);
}
