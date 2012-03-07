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
%token <string> HEADER_NAME HEADER_CONTENT GAP_LINE BODY_CONTENT LINE POSTFACE_LINE PART_END BOUNDARY

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
    BOUNDARY part { 
        //$1->parent_boundary = strdup(cmime_flbi_get_parent_boundary(msg,NULL,NULL));
        //$2->parent_boundary = strdup(msg->boundary);
        $2->parent_boundary = strdup($1);
        cmime_list_append(msg->parts,$2);
        
        printf("\n=========================================================\n");
        printf("part\n");
        printf("BOUND: [%s]\n",$1);
        printf("PARENT: [%s]\nBOUNDARY: [%s]\n",$2->parent_boundary,$2->boundary);
        printf("=========================================================\n");
        printf("[\n%s\n]\n\n",cmime_part_to_string($2));

    }
    | parts BOUNDARY part {
        //char *s = NULL; 
        //CMimePart_T *p = ((CMimePart_T *)cmime_list_data(msg->parts->tail)); 
        
        //prev = p->parent_boundary;
        //$2->parent_boundary = strdup(cmime_flbi_get_parent_boundary(msg,p->parent_boundary,p->boundary));
        
        //if (p->boundary != NULL) 
        //    $3->parent_boundary = strdup(p->boundary);
        //else 
        //    $3->parent_boundary = strdup(cmime_flbi_get_parent_boundary(msg,p->parent_boundary));

        $3->parent_boundary = strdup($2);
        cmime_list_append(msg->parts,$3);
        
        printf("\n=========================================================\n");
        printf("parts part\n");
        printf("BOUND: [%s]\n",$2);
        //printf("PREV PARENT: [%s]\n",p->parent_boundary);
        //printf("PREV BOUNDARY: [%s]\n",p->boundary);
        printf("PARENT: [%s]\nBOUNDARY: [%s]\n",$3->parent_boundary,$3->boundary);
        printf("=========================================================\n");
        printf("[\n%s\n]\n\n",cmime_part_to_string($3));

    } 
    | parts BOUNDARY part PART_END {
        //$3->parent_boundary = (char *)calloc(strlen($4) - 2, sizeof(char *));
        //$4 += 2;
        //strncpy($3->parent_boundary,$4,strlen($4) - 2);
        $3->parent_boundary = strdup($2);
        $3->last = 1;
        cmime_list_append(msg->parts,$3);
        
        printf("\n=========================================================\n");
        printf("parts part PART_END\n");
        printf("BOUND: [%s]\n",$2);
        printf("PART_END [%s]\n",$4);
        printf("PARENT: [%s]\nBOUNDARY: [%s]\n",$3->parent_boundary,$3->boundary);
        printf("=========================================================\n");
        printf("[\n%s\n]\n\n",cmime_part_to_string($3));
    }
    | parts BOUNDARY part PART_END postface {
        //$2->parent_boundary = (char *)calloc(strlen($3) - 2, sizeof(char *));
        //$3 += 2;
        //strncpy($2->parent_boundary,$3,strlen($3) - 2);
        $3->parent_boundary = strdup($2);
        $3->last = 1;
        $3->postface = strdup($5);
        cmime_list_append(msg->parts,$3);

        printf("\n=========================================================\n");
        printf("parts part PART_END postface\n");
        printf("BOUND: [%s]\n",$2);
        printf("PART_END [%s]\n",$4);
        printf("PARENT: [%s]\nBOUNDARY: [%s]\n",$3->parent_boundary,$3->boundary);
        printf("=========================================================\n");
        printf("[\n%s\n]\n\n",cmime_part_to_string($3));
    }
;
    
part:
    mime_headers mime_body {
    //    printf("PARENT: [%s]\n",$1);
        CMimePart_T *p = cmime_part_new();
        cmime_part_set_content(p,$2);
        free($2);
        $$ = p; 
        p->headers = $1;
        cmime_flbi_check_part_boundary(p);
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