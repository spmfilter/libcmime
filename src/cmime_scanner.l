%option noyywrap case-insensitive yylineno
%s BODY HEADER

%{
	#include "cmime_parser.tab.h"
%}

LF_PATTERN							(\r\n)|(\r)|(\n)|(\x0c)
HEADERNAME_PATTERN			([A-Za-z0-9-]*:\ )
CONTINUATION_PATTERN		(\t.*{LF_PATTERN})
LINE_PATTERN						(.*{LF_PATTERN})

%%

<BODY>{LINE_PATTERN}	{
	printf("body line: %s\n", yytext);
	yylval.string = yytext;
	return LINE;
}

{HEADERNAME_PATTERN} { 
	BEGIN HEADER;
	strtok(yytext, ":");
	printf("HEADERNAME: [%s]\n",yytext);
	yylval.string = yytext;
	return HEADERNAME;   
}

<HEADER>{LINE_PATTERN} {
	printf("HEADER IS: [%s]",yytext); 
	BEGIN INITIAL;
	yylval.string = yytext;
	return HEADERBODY;  
}

{CONTINUATION_PATTERN}	{
	printf("CONTINUES: [%s]\n", yytext);
	yylval.string = yytext; 
	return CONTINUATION;   
}

{LF_PATTERN} {
	BEGIN BODY;
	return EMPTY_LINE;   
}

. { /* nothing */ }

%%