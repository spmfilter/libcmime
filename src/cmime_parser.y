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
    #include "cmime_string.h"
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
%token <string> HEADER_NAME TO_HEADER HEADER_CONTENT GAP_LINE BODY_CONTENT LINE POSTFACE_LINE PART_END BOUNDARY

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
    | headers parts
    | headers BODY_CONTENT {
        CMimePart_T *p = cmime_part_new();
        cmime_part_set_content(p,$2);
        cmime_list_append(msg->parts,p);
    } 
;
        
headers:
    header { 
        if ($1 != NULL)
            cmime_list_append(msg->headers,$1); 
    }
    | headers header { 
        if ($2 != NULL)
            cmime_list_append(msg->headers,$2); 
    }
;
    
header:
    HEADER_NAME HEADER_CONTENT {
        CMimeAddressType_T t = -1;
        char *s = NULL;
        char *it = NULL;
        int in_name = 0;
        int pos = 0;
        CMimeHeader_T *h = NULL;

        /* got a header with message recipients? */
        if (strcasecmp($1,"from")==0) {
            t = CMIME_ADDRESS_TYPE_FROM;
        } else if (strcasecmp($1,"to")==0) {
            t = CMIME_ADDRESS_TYPE_TO;
        } else if (strcasecmp($1,"cc")==0) {
            t = CMIME_ADDRESS_TYPE_CC;
        } else if (strcasecmp($1,"bcc")==0) {
            t = CMIME_ADDRESS_TYPE_BCC;
        }
        
        if ((t != -1) && (t != CMIME_ADDRESS_TYPE_FROM)) {
            it = $2;
            s = (char *)calloc((size_t)1,sizeof(char));
            while(*it != '\0') {
                if ((*it == '"')||(*it == '\'')) {
                    if (in_name == 0)
                        in_name = 1;
                    else 
                        in_name = 0;
                }
                
                if ((*it == ',') && (in_name == 0)) {
                    cmime_message_add_recipient(msg,s,t);
                    free(s);
                    s = (char *)calloc((size_t)1,sizeof(char));
                    pos = 0;
                    *it++;
                } else {
                    s = (char *)realloc(s,strlen(s) + sizeof(char) + sizeof(char));
                    s[pos++] = *it++;
                    s[pos] = '\0';
                }
            }
            s[pos] = '\0';

            cmime_message_add_recipient(msg,s,t);
            free(s);
        } else if (t == CMIME_ADDRESS_TYPE_FROM) {
            cmime_message_set_sender(msg,$2);
        } else {
            h = cmime_header_new();
            cmime_header_set_name(h,$1);
            cmime_header_set_value(h,$2,0);
        }
        $$ = h;
    }
;

parts:
    BOUNDARY part { 
        $1 += 2;
        $2->parent_boundary = strdup($1);
        cmime_list_append(msg->parts,$2);
    }
    | parts BOUNDARY part {
        $2 += 2;
        $3->parent_boundary = strdup($2);
        cmime_list_append(msg->parts,$3);
    } 
    | parts BOUNDARY part PART_END {
        $2 += 2;
        $3->parent_boundary = strdup($2);
        $3->last = 1;
        cmime_list_append(msg->parts,$3);
    }
    | parts BOUNDARY part PART_END postface {
        $2 += 2;
        $3->parent_boundary = strdup($2);
        $3->last = 1;
        $3->postface = strdup($5);
        free($5);
        cmime_list_append(msg->parts,$3);
    }
;
    
part:
    mime_headers {
        CMimePart_T *p = NULL;
        p = cmime_part_new();
        // free allocated headers list, because we get a 
        // new one
        cmime_list_free(p->headers);
        p->headers = $1;
        cmime_flbi_check_part_boundary(p);
        $$ = p;
    }
    | mime_headers mime_body {
        CMimePart_T *p = NULL;
        p = cmime_part_new();
        cmime_part_set_content(p,$2);
        free($2);
        // free allocated headers list, because we get a 
        // new one
        cmime_list_free(p->headers);
        p->headers = $1;
        cmime_flbi_check_part_boundary(p);
        $$ = p;
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