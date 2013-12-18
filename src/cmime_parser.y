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
    headers
    |headers gap parts
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
    HEADER_NAME {
        CMimeHeader_T *h = NULL;
        h = cmime_flbi_get_header(msg,$1,NULL);
        $$ = h;
    }
    | HEADER_NAME HEADER_CONTENT {
        CMimeHeader_T *h = NULL;
        h = cmime_flbi_get_header(msg,$1,$2);
        $$ = h;
    }
;

parts:
    BOUNDARY part { 
        $1 += 2;
        $2->parent_boundary = cmime_flbi_chomp_boundary($1,msg->linebreak);
        cmime_list_append(msg->parts,$2);
        //printf("\nBOUNDARY part\n==========================\n");
        //printf("boundary [%s]\nparent [%s]\n",$2->boundary,$2->parent_boundary);
        //printf("==========================\n");
    }
    | BOUNDARY part PART_END { 
        $1 += 2;
        $2->parent_boundary = cmime_flbi_chomp_boundary($1,msg->linebreak);
        $2->postface = cmime_flbi_boundary_linebreak($3,msg->linebreak);
        $2->last = 1;
        cmime_list_append(msg->parts,$2);
        //printf("\nBOUNDARY part PART_END\n==========================\n");
        //printf("boundary [%s]\nparent [%s]\n",$2->boundary,$2->parent_boundary);
        //printf("==========================\n");
    }
    | BOUNDARY part PART_END postface { 
        char *l = NULL;
        char *p = NULL;
        $1 += 2;
        $2->parent_boundary = cmime_flbi_chomp_boundary($1,msg->linebreak);
        l = cmime_flbi_boundary_linebreak($3,msg->linebreak);
        if (l!=NULL) {
            asprintf(&p,"%s%s",l,$4);
            free(l);
        } else
            p = strdup($4);
        free($4);
        $2->postface = p;
        $2->last = 1;
        cmime_list_append(msg->parts,$2);
        //printf("\nBOUNDARY part PART_END postface\n==========================\n");
        //printf("boundary [%s]\nparent [%s]\n",$2->boundary,$2->parent_boundary);
        //printf("==========================\n");
    }
    | parts BOUNDARY part {
        $2 += 2;
        $3->parent_boundary = cmime_flbi_chomp_boundary($2,msg->linebreak);
        cmime_list_append(msg->parts,$3);
        //printf("\nparts BOUNDARY part\n==========================\n");
        //printf("boundary [%s]\nparent [%s]\n",$3->boundary,$3->parent_boundary);
        //printf("==========================\n");
    } 
    | parts BOUNDARY part PART_END {
        $2 += 2;
        $3->parent_boundary = cmime_flbi_chomp_boundary($2,msg->linebreak);
        $3->last = 1;
        $3->postface = cmime_flbi_boundary_linebreak($4,msg->linebreak);
        cmime_list_append(msg->parts,$3);
        //printf("\nparts BOUNDARY part PART_END\n==========================\n");
        //printf("boundary [%s]\nparent [%s]\n",$3->boundary,$3->parent_boundary);
        //printf("==========================\n");
    }
    | parts BOUNDARY part PART_END postface {
        char *l = NULL;
        char *p = NULL;
        $2 += 2;
        $3->parent_boundary = cmime_flbi_chomp_boundary($2,msg->linebreak);
        $3->last = 1;

        l = cmime_flbi_boundary_linebreak($4,msg->linebreak);
        if (l!=NULL) {
            asprintf(&p,"%s%s",l,$5);
            free(l);
        } else
            p = strdup($5);
        //$3->postface = p;
        $3->postface = cmime_flbi_scan_postface(p,msg);
        free($5);
        free(p);
        cmime_list_append(msg->parts,$3);
        //printf("\nparts BOUNDARY part PART_END postface\n==========================\n");
        //printf("boundary [%s]\nparent [%s]\n",$3->boundary,$3->parent_boundary);
        //printf("postface [%s]\n",$3->postface);
        //printf("==========================\n");
        
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

