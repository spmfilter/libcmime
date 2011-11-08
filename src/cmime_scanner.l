%option noyywrap 
%option reentrant 
%option nounput
%option bison-bridge

%s BODY HEADER

%{
	#include <stdio.h>
	#include <string.h>
	
	#include "cmime_flbi.h"
	#include "cmime_message.h"
	#include "cmime_parser.tab.h"
%}

LF_PATTERN							(\r\n)|(\r)|(\n)|(\x0c)
HEADERNAME_PATTERN			([A-Za-z0-9-]*:\ )
CONTINUATION_PATTERN		(\t.*{LF_PATTERN})
LINE_PATTERN						(.*{LF_PATTERN})
%%


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

{CONTINUATION_PATTERN}	{
	yylval->string = yytext;
	return CONTINUATION;   
}

{LF_PATTERN} {
	BEGIN BODY;
	return EMPTY_LINE;   
}

. { /* nothing */ } 

%%

CMimeMessage_T *cmime_scanner_read_file(const char *filename) {
	CMimeMessage_T *msg = cmime_message_new();
	yyscan_t scanner;
	FILE *fp = NULL;
	
	if(yylex_init(&scanner)) { 
		printf("no scanning today\n"); 
		return(NULL); 
	}
	
	if ((fp = fopen(filename, "rb")) == NULL) 
		return(NULL);

	
	yyrestart(fp,scanner);
	yyparse(scanner,msg);

	fclose(fp);
	yylex_destroy(scanner);

	return(msg);
}
