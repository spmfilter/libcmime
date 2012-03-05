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
%token <string> HEADER_NAME HEADER_CONTENT GAP_LINE BODY_CONTENT LINE POSTFACE_LINE PART_END

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
    part { 
        //$1->parent_boundary = strdup(msg->boundary);
        $1->parent_boundary = cmime_flbi_get_parent_boundary(msg,NULL);
        cmime_list_append(msg->parts,$1);
        
        printf("\n=========================================================\n");
        printf("part\n");
        printf("PARENT: [%s]\nBOUNDARY: [%s]\n",$1->parent_boundary,$1->boundary);
        printf("=========================================================\n");
        printf("[\n%s\n]\n\n",cmime_part_to_string($1));


//        if ($1->boundary != NULL) {
//            //printf("SWITCHING TO SUB\n");
//            
//            $$ = $1->parts;
//            $1->prev = msg->parts;
//        } else {
//            //printf("SWITCHING TO MAIN\n");
//            
//            $$ = msg->parts; 
//        }
    //    printf("BOUND1: [%s]\n",$1->boundary);
    }
    | parts part { 
        char *prev = NULL;
        prev = ((CMimePart_T *)cmime_list_data(msg->parts->tail))->parent_boundary;
        $2->parent_boundary = cmime_flbi_get_parent_boundary(msg,prev);
        cmime_list_append(msg->parts,$2);
        
        printf("\n=========================================================\n");
        printf("parts part\n");
        printf("PARENT: [%s]\nBOUNDARY: [%s]\n",$2->parent_boundary,$2->boundary);
        printf("=========================================================\n");
        printf("[\n%s\n]\n\n",cmime_part_to_string($2));
         
//        if ($$->size > 0)
//            l = $$;
//        else 
//            l = msg->parts;
//
//        CMimeListElem_T *e = NULL;
//        e = cmime_list_tail(l);
//        if (e != NULL) {
//            CMimePart_T *p = NULL;
//            p = (CMimePart_T *)cmime_list_data(e);
//            printf("PARENT: [%s]\n",p->parent_boundary);
//            //$2->parent_boundary = strdup(p->parent_boundary);
//        } else 
//            $2->parent_boundary = strdup(msg->boundary);
//
//        cmime_list_append($$,$2); 
//        if ($2->boundary != NULL) {
//            $2->prev = $$;
//            $$ = $2 ->parts; 
//        }

        

        
    //    printf("BOUND2: [%s]\n",$2->boundary);
    } 
    | parts part PART_END {
        cmime_list_append(msg->parts,$2);
        
        $2->parent_boundary = (char *)calloc(strlen($3) - 2, sizeof(char *));
        strncpy($2->parent_boundary,$3,strlen($3) - 2);
        cmime_list_append(msg->parts,$2);
        
        printf("\n=========================================================\n");
        printf("parts part PART_END\n");
        printf("PART_END [%s]\n",$3);
        printf("PARENT: [%s]\nBOUNDARY: [%s]\n",$2->parent_boundary,$2->boundary);
        printf("=========================================================\n");
        printf("[\n%s\n]\n\n",cmime_part_to_string($2));

     //   printf("\n===================\n");
     //   printf("parts part PART_END\n===================\n[\n%s\n]\n\n",cmime_part_to_string($2));
        
    }
    | parts part PART_END postface {
//        CMimeListElem_T *e = NULL;
//        CMimePart_T *p = NULL;
     //   printf("P END: [%s]\n",$3);
        $2->parent_boundary = (char *)calloc(strlen($3) - 2, sizeof(char *));
        strncpy($2->parent_boundary,$3,strlen($3) - 2);
        cmime_list_append(msg->parts,$2);

        printf("\n=========================================================\n");
        printf("parts part PART_END postface\n");
        printf("PART_END [%s]\n",$3);
        printf("PARENT: [%s]\nBOUNDARY: [%s]\n",$2->parent_boundary,$2->boundary);
        printf("=========================================================\n");
        printf("[\n%s\n]\n\n",cmime_part_to_string($2));

//        e = cmime_list_head($$);
//        while(e != NULL) {
//            p = (CMimePart_T *)cmime_list_data(e);
//            if (cmime_flbi_cmp_closing_boundary(p->boundary,$3)==1) {
//                cmime_part_set_postface(p,$4);

                //printf("SWITCHING TO PREV\n"); 
                //$$ = msg->parts; 
                //printf("P SIZE: [%d]\n",p->parts->size);
                //printf("PREV: [%s]\n",cmime_part_to_string(p));
                //$$ = (CMimeList_T *)p->prev;
//                $$ = p->parts;
//                break;
//            }
//            e = e->next;
//        }
        
    }
;
    
part:
    mime_headers mime_body {
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