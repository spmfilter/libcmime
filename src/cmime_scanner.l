%option noyywrap 
%option reentrant 
%option nounput
%option bison-bridge
%option bison-locations
 
%option yylineno
    
%x HEADER
%x RECIPIENT_LIST
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
    
    #define YY_EXTRA_TYPE CMimeMessage_T *
    #define YY_USER_ACTION yylloc->first_line = yylineno;
    #define YY_FATAL_ERROR(msg) fprintf(stderr, "%s\n", msg);
%}

LF_RE               (\r\n)|(\r)|(\n)|(\x0c)
HEADERNAME_RE       ([^ \n\t\r]+:)
HEADERBODY_RE       (.*({LF_RE}(\t|\ ).*)*{LF_RE}?)
BODY_RE             ((.*({LF_RE}))*)
LINE_RE             (.*{LF_RE})

%%

{HEADERNAME_RE} {
    BEGIN(HEADER);
    yylval->string = strsep(&yytext,":");
    
    return HEADER_NAME;
}

<HEADER>{HEADERBODY_RE} {
    BEGIN(INITIAL);

    if (yyextra->linebreak==NULL) 
        yyextra->linebreak = strdup(_cmime_internal_determine_linebreak(yytext));
        
    yylval->string = cmime_string_chomp(yytext);
    return HEADER_CONTENT;   
}
    
{LF_RE} {
    yyextra->gap = strdup(yytext); 
    
    if (yyextra->boundaries->count > 0)
        BEGIN(GAP);
    else
        BEGIN(BODY);
}

<GAP>{LINE_RE} {
    _BoundaryInfo_T *info = NULL;
    
    info = _cmime_internal_get_boundary_info(yyextra->boundaries,yytext,yyextra->linebreak);
    if (info!=NULL) {
        if (info->type == CMIME_BOUNDARY_OPEN) {
            free(info->marker);
            free(info);
            BEGIN(MIME_PART);
            yylval->string = yytext;
            return BOUNDARY;
        }
        free(info);
    } 
 
    yylval->string = yytext;
    return GAP_LINE;

}

<BODY>{BODY_RE} {
    yylval->string = yytext;
    return BODY_CONTENT;
}


<MIME_CONTENT>{LINE_RE} {
    _BoundaryInfo_T *info = NULL;
    yylval->string = yytext; 
        
    info = _cmime_internal_get_boundary_info(yyextra->boundaries,yytext,yyextra->linebreak);
    
    if (info!=NULL) {
        if (info->type == CMIME_BOUNDARY_CLOSE) {
            free(info->marker);
            free(info);
            BEGIN(POSTFACE);
            return PART_END;
        } else {
            free(info->marker);
            free(info);
            BEGIN(MIME_PART); 
            return BOUNDARY;
        }
    } else {
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
    yylval->string = cmime_string_chomp(yytext);
    
    return HEADER_CONTENT;
}


<POSTFACE>{LINE_RE} {
    _BoundaryInfo_T *info = NULL;
    
    info = _cmime_internal_get_boundary_info(yyextra->boundaries,yytext,yyextra->linebreak);
    
    if (info!=NULL) {
        if (info->type == CMIME_BOUNDARY_OPEN) {
            free(info->marker);
            free(info);
            BEGIN(MIME_PART);
            yylval->string = yytext;
            return BOUNDARY;
        } else {
            free(info->marker);
            free(info);
            yylval->string = yytext;
            return POSTFACE_LINE;
        }
    } else {
        yylval->string = yytext;
        return POSTFACE_LINE;
    }
}

. { } 

%%

int cmime_scanner_scan_buffer(CMimeMessage_T **message, char *buffer) {
    int ret;
    yyscan_t scanner;
    YY_BUFFER_STATE buf;

    if(yylex_init_extra(*message,&scanner)) { 
        fprintf(stderr, "error: no scanning today\n"); 
        return(-4); 
    }

    buf = yy_scan_string(buffer, scanner);
    yy_switch_to_buffer(buf,scanner);
    ret = yyparse(scanner,(*message));
    yy_delete_buffer(buf,scanner);

    yylex_destroy(scanner);

    return(ret);
}
