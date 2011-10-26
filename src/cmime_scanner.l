%option noyywrap yylineno
%s BODY HEADER

%{
	#include <string.h>
	#include "cmime_string.h"
	#include "cmime_message.h"
	#include "cmime_internal.h"
	#include "cmime_parser.tab.h"
%}

LF_PATTERN							(\r\n)|(\r)|(\n)|(\x0c)
HEADERNAME_PATTERN			([A-Za-z0-9-]*:\ )
CONTINUATION_PATTERN		(\t.*{LF_PATTERN})
LINE_PATTERN						(.*{LF_PATTERN})
%%


<BODY>{LINE_PATTERN}	{
	yylval.string = yytext;
	return LINE;
}

{HEADERNAME_PATTERN} { 
	BEGIN HEADER;
	yylval.string = strsep(&yytext,":");
	return HEADERNAME;   
}

<HEADER>{LINE_PATTERN} {
	BEGIN INITIAL;
	yylval.string = yytext;
	
	return HEADERBODY;  
}

{CONTINUATION_PATTERN}	{
	yylval.string = yytext;
	return CONTINUATION;   
}

{LF_PATTERN} {
	BEGIN BODY;
	return EMPTY_LINE;   
}

. { /* nothing */ } 

%%