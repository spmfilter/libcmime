/* libcmime - A C mime library
 * Copyright (C) 2013 SpaceNet AG and Axel Steiner <ast@treibsand.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#define _GNU_SOURCE

#include "cmime_message.h"
#include "cmime_part.h"
#include "cmime_util.h"
#include "cmime_internal.h"
#include "cmime_base64.h"
#include "cmime_string.h"
#include "cmime_list.h"
#include "cmime_qp.h"


/* search for header value in pointer p 
 * and return value as newly allocated string */
char *_parse_header(char *p) {
    char *token = NULL;
    char *cp = NULL;
    char *tf = NULL;
    char *out = NULL;
    int i = 0;
    char *nl = _cmime_internal_determine_linebreak(p);
    char *brkb = NULL;
    tf = cp = strdup(p);
    
    for (token = strtok_r(cp,nl,&brkb); token; token = strtok_r(NULL,nl,&brkb)) {   

        if (i==0) {
            asprintf(&out,"%s%s",token,nl);
        } else {
            if (isspace(token[0])!=0) {
                char *new_out;
                asprintf(&new_out, "%s%s%s", out, token, nl);
                free(out);
                out = new_out;
            } else
                break;
        }
        i++;
    }
    
    free(tf);
    return(out);
}

CMimePart_T *cmime_part_new(void) {
    CMimePart_T *part = NULL;
    
    part = (CMimePart_T *)calloc((size_t)1, sizeof(CMimePart_T));
    
    if (cmime_list_new(&part->headers,_cmime_internal_header_destroy)!=0) {
        free(part);
        return(NULL);
    }

    part->content = NULL;
    part->boundary = NULL;
    part->parent_boundary = NULL;
    part->postface = NULL;
    part->last = 0;

    return(part);
}

void cmime_part_free(CMimePart_T *part) {
    assert(part);
    
    cmime_list_free(part->headers);
    
    if (part->content != NULL)
        free(part->content);
    
    if (part->boundary != NULL)
        free(part->boundary);

    if (part->parent_boundary != NULL)
        free(part->parent_boundary);

    if (part->postface != NULL)
        free(part->postface);
        
    free(part);
}


void cmime_part_set_content_type(CMimePart_T *part, const char *s) {
    assert(part);
    assert(s);

    _cmime_internal_set_linked_header_value(part->headers,"Content-Type",s);
}

char *cmime_part_get_content_type(CMimePart_T *part) {
    assert(part);
    
    return(_cmime_internal_get_linked_header_value(part->headers,"Content-Type"));
}

void cmime_part_set_content_disposition(CMimePart_T *part, const char *s) {
    assert(part);
    assert(s);

    _cmime_internal_set_linked_header_value(part->headers,"Content-Disposition",s);
}

char *cmime_part_get_content_disposition(CMimePart_T *part) {
    assert(part);
    
    return(_cmime_internal_get_linked_header_value(part->headers,"Content-Disposition"));
}

void cmime_part_set_content_transfer_encoding(CMimePart_T *part, const char *s) {
    assert(part);
    assert(s);

    _cmime_internal_set_linked_header_value(part->headers,"Content-Transfer-Encoding",s);
}

char *cmime_part_get_content_transfer_encoding(CMimePart_T *part) {
    assert(part);
    
    return(_cmime_internal_get_linked_header_value(part->headers,"Content-Transfer-Encoding"));
}

void cmime_part_set_content_id(CMimePart_T *part, const char *s) {
    assert(part);
    assert(s);
    
    _cmime_internal_set_linked_header_value(part->headers,"Content-ID",s);
}

char *cmime_part_get_content_id(CMimePart_T *part) {
    return(_cmime_internal_get_linked_header_value(part->headers,"Content-ID"));
}

void cmime_part_set_content(CMimePart_T *part, const char *s) {
    assert(part);
    assert(s);
    
    if (part->content != NULL)
        free(part->content);
    
    part->content = strdup(s);
}

char *cmime_part_to_string(CMimePart_T *part, const char *nl) {
    char *out = NULL;
    char *content = NULL; 
    char *ptemp = NULL;
    int with_headers = 0;
    CMimeListElem_T *e;
    CMimeHeader_T *h;
    
    assert(part);

    content = cmime_part_get_content(part);
    
    if (nl == NULL) {
        if (content != NULL)
            nl = _cmime_internal_determine_linebreak(content);  
        
        if (nl == NULL)
            nl = CRLF;
    }

    out = (char *)calloc(1,sizeof(char));
    if (cmime_list_size(part->headers)!=0) {
        e = cmime_list_head(part->headers);
        while(e != NULL) {
            h = (CMimeHeader_T *)cmime_list_data(e);
            ptemp = cmime_header_to_string(h);
            /* check whether we have to add a newline, or not */
            if ( strcmp(ptemp + strlen(ptemp) - strlen(nl), nl) == 0) {
                out = (char *)realloc(out,strlen(out) + strlen(ptemp) + 1);
                strcat(out,ptemp);
            } else {
                out = (char *)realloc(out,strlen(out) + strlen(ptemp) + strlen(nl) + 1);
                strcat(out,ptemp);
                strcat(out,nl);
            }
            free(ptemp);
            e = e->next;
        } 
        with_headers = 1; 
    }
    
    if (with_headers==1) {
        out = (char *)realloc(out,strlen(out) + strlen(nl) + 2);
        strcat(out,nl);
    } 

    if (content != NULL) {
        out = (char *)realloc(out,strlen(out) + strlen(content) + 2);
        strcat(out,content);
    }
    return(out);
}

int cmime_part_from_file(CMimePart_T **part, char *filename, const char *nl) {
    struct stat fileinfo;
    int retval = 0;
    FILE *fp = NULL;
    int encode = 0;
    int i = 0;
    int len = 0;
    int blocksout = 0;
    int pos = 0;
    unsigned char in[3], out[4];
    char *ptemp = NULL;
    CMimeInfo_T *mi = NULL;

    assert((*part));
    assert(filename);

    /* only regular files please */
    retval = stat(filename,&fileinfo);
    if (retval == 0) {
        if(S_ISREG(fileinfo.st_mode)) {
            if (nl == NULL) 
                nl = _cmime_internal_determine_linebreak_from_file(filename);
            
            mi = cmime_util_info_get_from_file(filename);
            if (strcmp(mi->mime_encoding,"binary")==0) 
                asprintf(&ptemp,"%s;%s\tname=%s%s",mi->mime_type,nl,basename(filename),nl);
            else 
                asprintf(&ptemp,"%s;%s\tcharset=%s%s",mi->mime_type,nl,mi->mime_encoding,nl);
            
            cmime_util_info_free(mi);
            cmime_part_set_content_type((*part),ptemp);
            encode = (strncmp(ptemp,MIMETYPE_TEXT_PLAIN,strlen(MIMETYPE_TEXT_PLAIN)) == 0) ? 0 : 1;
            
                    
            if (encode == 1) {
                free(ptemp);
                asprintf(&ptemp,"base64%s",nl);
            } else {
                if (strstr(ptemp,"ascii")!=NULL) {
                    free(ptemp);
                    asprintf(&ptemp,"7bit%s",nl);
                } else {
                    free(ptemp);
                    asprintf(&ptemp,"8bit%s",nl);
                }
            }

            cmime_part_set_content_transfer_encoding((*part),ptemp);
            free(ptemp);
            
            asprintf(&ptemp,"attachment;%s\tfilename=%s%s",nl,basename(filename),nl);
            cmime_part_set_content_disposition((*part),ptemp);      
            free(ptemp);
            
            fp = fopen(filename, "rb");
            if (fp != NULL) {
                (*part)->content = (char *)calloc(1,sizeof(char));
                while(!feof(fp)) {
                    len = 0;
                    
                    for(i=0; i<3; i++) {
                        in[i] = (unsigned char)fgetc(fp);

                        if(!feof(fp)) {
                            len++;
                        } else {
                            in[i] = 0;
                        }
                    }

                    if (len) {
                        if (encode == 0) {
                            (*part)->content = (char *)realloc((*part)->content,strlen((*part)->content) + sizeof(in) + sizeof(char) +1);
                            for(i=0; i<3; i++) {
                                (*part)->content[pos++] = in[i];    
                            }
                            (*part)->content[pos] = '\0';
                        } else {
                            cmime_base64_encode_block(in,out,len);
                            (*part)->content = (char *)realloc((*part)->content,strlen((*part)->content) + sizeof(out) + sizeof(char) + 1);
                            for (i=0; i<4;i++) {
                                (*part)->content[pos++] = out[i];
                            }
                            (*part)->content[pos] = '\0';                       
                        }
                        blocksout++;
                    }

                    if(blocksout >= (LINE_LENGTH / 4) || feof(fp)) {
                        if(blocksout && (encode == 1)) {
                            /* if base64 data, we need to do a line break after LINE_LENGTH chars */
                            (*part)->content = (char *)realloc((*part)->content,strlen((*part)->content) + strlen(nl) + sizeof(char));
                            
                            for (i=0; i<strlen(nl);i++) {
                                (*part)->content[pos++] = nl[i];    
                            }
                            (*part)->content[pos] = '\0';
                        }
                    blocksout = 0;
                    } 
                }
                if (fclose(fp)!=0)
                    perror("libcmime: error closing file");
            } else {
                perror("libcmime: error opening file");
                return (-3); /* failed to open file */
            }           
        } else {
            return(-2); /* not regular file */
        } 
    } else {
        return(-1); /* stat error */
    }
    
    return(0);
}

static int
my_strmatch(const char *big, const char *little)
{
    size_t z = strlen(little);

    while (isspace(*big))
        big++;

    if (strncmp(big, little, z))
        return 1; /* No match */

    big += z;
    while (isspace(*big))
        big++;

    return *big != 0;
}

int cmime_part_to_file(CMimePart_T *part, char *filename) {
    int retval = 0;
    FILE *fp = NULL;
    char *encoding = NULL;
    char *decoded_str = NULL;

    // todo: extend for x-uuencode
    const char base64[] = "base64";
    const char qp[] = "quoted-printable";

    assert(part);
    assert(filename);

    // check for encodings
    encoding = cmime_part_get_content_transfer_encoding(part);

    if(encoding == NULL) {
        asprintf(&decoded_str,"%s",part->content);
    } else if (my_strmatch(encoding,qp)==0) {
        decoded_str = cmime_qp_decode_text(part->content);
    } else if (my_strmatch(encoding,base64)==0) {
        decoded_str = cmime_base64_decode_string(part->content);  
    } else {
        asprintf(&decoded_str,"%s",part->content);
    }
    fp = fopen(filename, "wb");
    if(fp != NULL) {
        fwrite(decoded_str,strlen(decoded_str),1,fp);
        if (fclose(fp)!=0)
            perror("libcmime: error closing file");   
    } else {
        perror("libcmime: error opening file");
        retval = -3; /* failed to open file */ 
    }
    // some cleanup
    // RJW: Do NOT free encoding! It's a header and will be freed later.
    // if(encoding!=NULL)
    //    free(encoding);
    if(decoded_str!=NULL)
        free(decoded_str);

    return retval;
}


int cmime_part_from_string(CMimePart_T **part, const char *content) {
    char *ptemp = NULL;
    char *ptemp2 = NULL;
    char *body = NULL;
    char *lb = NULL;
    char *dlb;
    char *it = NULL;

    assert((*part));
    assert(content);
    
    lb = _cmime_internal_determine_linebreak(content);
    if (lb == NULL)
        return(-1);
    
    asprintf(&dlb,"%s%s",lb,lb);

    ptemp = strstr(content,dlb);
    if (ptemp != NULL) {
        it = (char *)content;
        while (*it != '\0') {
            if (strncasecmp(it,PART_CONTENT_TYPE_PATTERN,strlen(PART_CONTENT_TYPE_PATTERN))==0) {               
                it = it + sizeof(PART_CONTENT_TYPE_PATTERN);
                ptemp2 = _parse_header(it);         
                cmime_part_set_content_type((*part),ptemp2);
                free(ptemp2);
            }

            if (strncasecmp(it,PART_CONTENT_TRANSFER_ENCODING_PATTERN,strlen(PART_CONTENT_TRANSFER_ENCODING_PATTERN))==0) {
                it = it + sizeof(PART_CONTENT_TRANSFER_ENCODING_PATTERN);
                ptemp2 = _parse_header(it);
                cmime_part_set_content_transfer_encoding((*part),ptemp2);
                free(ptemp2);
            }
            
            if (strncasecmp(it,PART_CONTENT_DISPOSITION_PATTERN,strlen(PART_CONTENT_DISPOSITION_PATTERN))==0) {
                it = it + sizeof(PART_CONTENT_DISPOSITION_PATTERN);
                ptemp2 = _parse_header(it);
                cmime_part_set_content_disposition((*part),ptemp2);
                free(ptemp2);
            }   
                
            if (strncasecmp(it,PART_CONTENT_ID_PATTERN,strlen(PART_CONTENT_ID_PATTERN))==0) {
                it = it + sizeof(PART_CONTENT_ID_PATTERN);
                ptemp2 = _parse_header(it);
                cmime_part_set_content_id((*part),ptemp2);
                free(ptemp2);
            }   

            // break if body begins 
            if (strncmp(it,dlb,strlen(dlb))==0)
                break;
                
            it++;
        }
        
        ptemp += strlen(dlb);
        body = strdup(ptemp);
    } else {
        body = strdup(content);
    }
    
    cmime_part_set_content((*part),body);
    free(body);
    free(dlb);
    
    return(0);
}

void cmime_part_set_postface(CMimePart_T *part, const char *s) {
    assert(part);
    assert(s);
    
    part->postface = strdup(s);
}
