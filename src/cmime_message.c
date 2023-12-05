/* libcmime - A C mime library
 * Copyright (C) 2021 SpaceNet AG and Axel Steiner <ast@treibsand.com>
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

#include "cmime_address.h"
#include "cmime_message.h"
#include "cmime_list.h"
#include "cmime_string.h"
#include "cmime_header.h"
#include "cmime_part.h"
#include "cmime_internal.h"
#include "cmime_flbi.h"


#ifndef DOXYGEN_SHOULD_SKIP_THIS
typedef struct {
    char *stripped;
    CMimeStringList_T *mime_bodies;
} _StrippedData_T;
#endif /* DOXYGEN_SHOULD_SKIP_THIS */

/* find all boundaries in string */
CMimeStringList_T *_get_boundaries(char *s) {
    CMimeStringList_T *boundaries = NULL;   
    char *t = NULL;
    char *header = NULL;
    char *p = NULL;
    int pos = 0;
    char *it = NULL;
    char *nxt = NULL;
    size_t offset = 0;

    boundaries = cmime_string_list_new();

    /* search for content-type headers, where new boundaries are defined */    
    while ((it = strcasestr(s,"content-type:"))!=NULL) {
        offset = it-s;

        /* before the content-type header must not be any char except newline */
        if ((s[offset-1]!=(unsigned char)10)&&(s[offset-1]!=(unsigned char)13))
        {
            /* Change from RJW. My test email contains:
             * ARC-Message-Signature: i=1; a=rsa-sha256; c=relaxed/relaxed; d=microsoft.com;
             *    s=arcselector9901;
             *    h=From:Date:Subject:Message-ID:Content-Type:MIME-Version:X-MS-Exchange-AntiSpam-M
             * and that triggers this. Don't give up, just skip over it.
             */
            s = &s[offset+1];
            continue;
        }

        /* get all content-type header line(s) */
        header = (char *)calloc(sizeof(char),sizeof(char));
        while(*it != '\0') {
            nxt = it;
            nxt++;

            /* break if char is a newline is not followed by a whitespace or tabulator */
            
            // we have a CR followed by a LF
            if((*it==(unsigned char)13)&&(*nxt==(unsigned char)10)){
                nxt++;

                if((*nxt!=(unsigned char)9)&&(*nxt!=(unsigned char)32))
                    break;
            }  
            
            if (((*it==(unsigned char)13)||(*it==(unsigned char)10)) &&((*nxt!=(unsigned char)9)&&(*nxt!=(unsigned char)32))) {
                break;
            }
            

            header = (char *)realloc(header,pos + 1 + sizeof(char));
            header[pos++] = *it;
            it++;
        }
        header[pos] = '\0';
        pos = 0;

        s = it;
        /* now search for boundary= in content-type header */
        if ((it = strcasestr(header,"boundary="))!=NULL) { 
            p = strstr(it,"=");
            if (*++p=='"') 
                p++;
                    
            t = (char *)calloc(sizeof(char),sizeof(char));
            /* if found, extract boundary and append to list */
            while(*p != '\0') {
                if ((*p=='"')||(*p==';')||(*p==(unsigned char)10)||(*p==(unsigned char)13))
                    break;

                t = (char *)realloc(t,pos + (2 * sizeof(char)));
                t[pos++] = *p;
                p++;
            }
            t[pos] = '\0';
            pos = 0;
            cmime_string_list_insert(boundaries,t);
            free(t);
        }

        free(header);
    }
    
    return(boundaries);
}

/* re-add stripped bodies to message */
void _add_stripped_bodies(CMimeMessage_T **message, _StrippedData_T *sd) {
    int i;
    CMimeListElem_T *elem = NULL;
    CMimePart_T *part = NULL;
    char *mime_body = NULL;

    /* now the wedding between CMimeMessage_T and stripped content */
    i = 0;
    elem = cmime_list_head((*message)->parts);
    while(elem != NULL) {
        part = (CMimePart_T *)cmime_list_data(elem);
        mime_body = cmime_string_list_get(sd->mime_bodies,i);
        part->content = mime_body;
        i++;
        elem = elem->next;
    }
    free(sd->stripped);
}

_StrippedData_T *_strip_message(CMimeMessage_T **msg, char *buffer, int header_only) {
    _StrippedData_T *sd = NULL;
    _BoundaryInfo_T *info = NULL;
    char *newline_char = NULL;
    char *empty_line = NULL;
    char *it = NULL;
    size_t offset;
    int count = 0;
    _BoundaryInfo_T *nxt_info = NULL;
    char *nxt = NULL;
    char *mime_body_start = NULL;
    char *mime_body = NULL;
    char *t = NULL;
    
    sd = (_StrippedData_T *)calloc((size_t)1,sizeof(_StrippedData_T));
    sd->stripped = NULL;

    /* search newline and build header-body seperator */
    newline_char = _cmime_internal_determine_linebreak(buffer);
    (*msg)->linebreak = strdup(newline_char);
    asprintf(&empty_line,"%s%s",newline_char,newline_char);

    cmime_string_list_free((*msg)->boundaries);
    (*msg)->boundaries = _get_boundaries(buffer);
    if (cmime_string_list_get_count((*msg)->boundaries)!=0)
        (*msg)->boundary = strdup(cmime_string_list_get((*msg)->boundaries, 0));
    sd->mime_bodies = cmime_string_list_new();

    if (header_only == 1) {
        if((t = strstr(buffer,empty_line)) != NULL) {
            offset = strlen(buffer) - strlen(t);    
            sd->stripped = (char *)calloc(offset + sizeof(char),sizeof(char));
            strncpy(sd->stripped,buffer,offset);
            sd->stripped[offset] = '\0';
        } else
            sd->stripped = buffer;
    } else {
        if (cmime_string_list_get_count((*msg)->boundaries) > 0) {
            sd->stripped = (char *)calloc(sizeof(char),sizeof(char));
            it = buffer;
            while((it = strstr(it,"--"))!=NULL) {
                info = _cmime_internal_get_boundary_info((*msg)->boundaries,it,newline_char);
                if (info != NULL) {
                    /* check existing mime body marker */
                    if (mime_body_start != NULL) {
                        /* there is a start marker.... */
                        offset = strlen(mime_body_start) - strlen(it);
                        if (offset > 0) {
                            /* calculate offset to next boundary and copy body */
                            mime_body = (char *)calloc(offset + sizeof(char),sizeof(char));
                            strncpy(mime_body,mime_body_start,offset);
                            mime_body[strlen(mime_body)] = '\0';
                        } else {
                            /* this part seems to be empty */
                            mime_body = (char *)calloc(sizeof(char),sizeof(char));
                            mime_body[0] = '\0';
                        }
                        cmime_string_list_insert(sd->mime_bodies, mime_body);
                        free(mime_body);

                        offset = -1;
                        mime_body_start = NULL;
                    }

                    // check if it's a closing boundary
                    if (info->type == CMIME_BOUNDARY_CLOSE) {
                        nxt = strstr(it,newline_char);
                        if ((nxt = strstr(nxt,"--"))!=NULL) {
                            if ((nxt_info = _cmime_internal_get_boundary_info((*msg)->boundaries,nxt,newline_char))!=NULL) {  
                                /* we've found another boundary, so it's not the last part. 
                                 * Offset is the difference between previous found boundary and new one */
                                offset = strlen(it) - strlen(nxt); 
                                free(nxt_info->marker);
                                free(nxt_info);
                                nxt_info = NULL;
                            }
                        }

                        if (offset == -1)
                            /* since _match_boundary has not found another part behind,
                             * this seems to be the end...my best friend.
                             * Offset is the difference between previous found boundary and file end */
                            offset = strlen(it);                           
                    } else {
                        if (count == 0) {
                            /* this is the first run, so we have to copy the message
                             * headers first */
                            offset = strlen(buffer) - strlen(it);    
                            sd->stripped = (char *)realloc(sd->stripped,strlen(sd->stripped) + offset + sizeof(char));
                            strncat(sd->stripped,buffer,offset);
                            offset = -1;
                            
                            count++;
                        }

                        /* calculate offset for mime part headers */
                        t = strstr(it,empty_line);
                        t = t + strlen(empty_line);
                        offset = strlen(it) - strlen(t);
                        if (offset > 0)
                            /* Now it's time to grap the mime part body.
                             * Set a marker where the mime part content begins*/
                            mime_body_start = t;
                    }
                    
                    if (offset != -1) {
                        sd->stripped = (char *)realloc(sd->stripped,strlen(sd->stripped) + offset + sizeof(char));
                        strncat(sd->stripped,it,offset);
                        offset = -1;
                    }
                
                    free(info->marker);
                    free(info);
                    info = NULL;
                }
                it++;
            }
        } else {
            sd->stripped = buffer;
        }
    }
    free(empty_line);

    return(sd);
}

void _recipients_destroy(void *data) {
    CMimeAddress_T *ca = (CMimeAddress_T *)data;
    assert(data);
    cmime_address_free(ca);
}

void _rebuild_first_part(CMimeMessage_T *message) {
    CMimeListElem_T * e = NULL;
    CMimePart_T *p = NULL;
    CMimeInfo_T *mi = NULL;
    char *s = NULL;
    char *s2 = NULL;
    char *nl = NULL;

    if (message->parts->size == 1) {
        cmime_message_set_mime_version(message, "1.0");
        
        if (message->boundary == NULL)
            cmime_message_add_generated_boundary(message);
           
        e = cmime_list_head(message->parts);
        p = (CMimePart_T *)cmime_list_data(e);
        if (p->parent_boundary == NULL) {
            p->parent_boundary = strdup(message->boundary);
            s = cmime_part_get_content(p);

            mi = cmime_util_info_get_from_string(s);
            if (mi!=NULL) {
                if (mi->combined != NULL)
                    cmime_part_set_content_type(p, mi->combined);

                cmime_util_info_free(mi);
            } else {
                if (message->linebreak == NULL) {
                    nl = _cmime_internal_determine_linebreak(s);
                    if (nl == NULL)
                        nl = CRLF;

                    message->linebreak = strdup(nl);
                }
                mi = cmime_util_info_get_from_string(s);
                asprintf(&s2,"%s;%s\tcharset=%s",mi->mime_type,message->linebreak,mi->mime_encoding);
                cmime_part_set_content_type(p, s2);
                free(s2);
            }

            s2 = cmime_message_get_content_transfer_encoding(message);
            if (s2 != NULL)
                cmime_part_set_content_transfer_encoding(p, s2);
            else {
                if (cmime_string_is_7bit(s)==0)
                    cmime_part_set_content_transfer_encoding(p, "7bit"); 
            }

            p->last = 1;
        }
    }
}

void _append_boundary(char **out, const char *boundary,const char *linebreak, CMimeBoundaryType_T type) {
    char *s = NULL;
    if(boundary != NULL) {
        if (type == CMIME_BOUNDARY_OPEN)
            asprintf(&s,"--%s%s",boundary,linebreak);
        else if (type == CMIME_BOUNDARY_CLOSE)
            //asprintf(&s,"--%s--%s",boundary,linebreak);
            asprintf(&s,"--%s--",boundary);

        (*out) = (char *)realloc((*out),strlen((*out)) + strlen(s) + sizeof(char));
        strcat((*out),s);
        free(s);
    }
}

void _append_string(char **out, const char *s) {
    if (s != NULL) {
        (*out) = (char *)realloc((*out),strlen((*out)) + strlen(s) + sizeof(char));
        strcat((*out),s);
    }
}

void _append_child_parts(char **out, char *parent_boundary, CMimeMessage_T *message) {
    CMimeListElem_T *e = NULL;
    CMimePart_T *p = NULL;
    int len;
    char *s = NULL;
    int append = 0;

    e = cmime_list_head(message->parts);
    while(e != NULL) {
        p = (CMimePart_T *)cmime_list_data(e);

        if (p->parent_boundary==NULL) {
            append = 1;
        } else {
            if (strcmp(p->parent_boundary,parent_boundary)==0)
                append = 1;
        }

        if (append==1) {
            len = strlen(*out);
            if (((*out)[len - 1] != '\r') && (*out)[len - 1] != '\n')
                _append_string(&(*out),message->linebreak);

            _append_boundary(&(*out), p->parent_boundary, message->linebreak, CMIME_BOUNDARY_OPEN);
            s = cmime_part_to_string(p,message->linebreak);
            if (s)
                _append_string(&(*out),s);

            if (p->boundary!=NULL)
                _append_child_parts(out, p->boundary, message);

            if (p->last == 1) {
                len = strlen(s);
                if ((s[len - 1] != '\r') && s[len - 1] != '\n') 
                    _append_string(&(*out),message->linebreak);

                _append_boundary(&(*out), p->parent_boundary, message->linebreak, CMIME_BOUNDARY_CLOSE);
                if (p->postface != NULL) 
                    _append_string(&(*out),p->postface); 
            }

            if (s)
                free(s);

            append = 0;
        }
        e = e->next;
    }
}

CMimeMessage_T *cmime_message_new(void) {
    CMimeMessage_T *message = NULL;
    
    message = (CMimeMessage_T *)calloc((size_t)1, sizeof(CMimeMessage_T));

    if (cmime_list_new(&message->headers,_cmime_internal_header_destroy)!=0) {
        free(message);
        return(NULL);
    }
    
    message->sender = NULL;
    if (cmime_list_new(&message->recipients,_recipients_destroy)!=0) {
        free(message);
        return(NULL);
    }

    message->boundary = NULL;
    message->gap = NULL;
    message->linebreak = NULL;

    if (cmime_list_new(&message->parts,_cmime_internal_parts_destroy)!=0)  {
        free(message);
        return(NULL);
    }


    message->boundaries = cmime_string_list_new();
    return(message);
}

/** Free a CMimeMessage_T object  */
void cmime_message_free(CMimeMessage_T *message) {
    assert(message);

    cmime_address_free(message->sender);    
    cmime_list_free(message->recipients);

    cmime_list_free(message->headers);
    
    if (message->boundary!=NULL)
        free(message->boundary);
    
    if (message->gap!=NULL)
        free(message->gap);
    
    if (message->linebreak!=NULL)
        free(message->linebreak);
    
    cmime_list_free(message->parts);
    cmime_string_list_free(message->boundaries);

    free(message);
}

/* Set sender of message */
void cmime_message_set_sender(CMimeMessage_T *message, const char *sender) {
    CMimeAddress_T *ca = NULL;
    char *s = NULL;

    assert(message);
    // fallback if sender is empty
    if (sender == NULL) {
        asprintf(&sender, "");
    } 

    ca = cmime_address_parse_string(sender);
    if (message->sender != NULL) {
        cmime_address_free(message->sender);
        s = _cmime_internal_get_linked_header_value(message->headers, FROM_HEADER);
        if (s != NULL)
            free(s);
    }
    ca->type = CMIME_ADDRESS_TYPE_FROM;
    message->sender = ca;
    _cmime_internal_set_linked_header_value(message->headers, FROM_HEADER, NULL);
}

void cmime_message_set_sender_encode(CMimeMessage_T *message, const char *sender) {
    CMimeAddress_T *ca = NULL;
    char *name_encoded = NULL;
    char *encoded = NULL;

    assert(message);
    assert(sender);

    ca = cmime_address_parse_string(sender);
    // encode sender name, if necessary
    if (ca->name != NULL) {
        name_encoded = cmime_string_encode_to_7bit(ca->name,CMIME_STRING_ENCODING_B64);
        cmime_address_set_name(ca,name_encoded);
        ca->parsed = 0;
        free(name_encoded);
    }

    encoded = cmime_address_to_string(ca);
    cmime_message_set_sender(message, encoded);
    free(encoded);
    cmime_address_free(ca);
}

char *cmime_message_get_sender_string(CMimeMessage_T *message) {
    char *s = NULL;

    if (message->sender != NULL)
        s = cmime_address_to_string(message->sender);

    return(s);
}

void cmime_message_set_message_id(CMimeMessage_T *message, const char *mid) {
    char *id = NULL;
    int i;
    int pos = 0;
    if (mid[0] != '<') {
        id = (char *)malloc(strlen(mid) + 4);
        id[pos++] = '<';
        for (i = 0; i < strlen(mid); i++) {
            id[pos++] = mid[i];
        }
        id[pos++] = '>';
        id[pos++] = '\0';
    } else
        id = strdup(mid);

    _cmime_internal_set_linked_header_value(message->headers,"Message-ID",id);
    free(id);
}

char *cmime_message_get_message_id(CMimeMessage_T *message) {
    return(_cmime_internal_get_linked_header_value(message->headers,"Message-ID"));
}

int cmime_message_set_header(CMimeMessage_T *message, const char *header) {
    char *cp = NULL;
    char *tf = NULL;
    char *k = NULL;
    assert(message);
    assert(header);
    
    tf = cp = strdup(header);
    k = strsep(&cp,":");
    
    _cmime_internal_set_linked_header_value(message->headers,k,cp);
    free(tf);
    return(0);
}

CMimeHeader_T *cmime_message_get_header(CMimeMessage_T *message, const char *header) {
    CMimeHeader_T *h = NULL;
    CMimeListElem_T *e;
    
    assert(message);
    assert(header);
    
    e = cmime_list_head(message->headers);
    while(e != NULL) {
        h = (CMimeHeader_T *)cmime_list_data(e);
        if (strcasecmp(cmime_header_get_name(h),header)==0) {
            return(h);
        }
        e = e->next;
    }
    
    return(NULL);
}

int cmime_message_add_recipient(CMimeMessage_T *message, const char *recipient, CMimeAddressType_T t) {
    CMimeAddress_T * ca = NULL;
    int found = 0;
    
    assert(message);
    assert(recipient);

    // parse recipient string and create CMimeAddress_T object
    ca = cmime_address_parse_string(recipient);
    cmime_address_set_type(ca,t);
    if (message->recipients == NULL) {
        if (cmime_list_new(&message->recipients,_recipients_destroy)!=0) 
                return(-1);
    }   

    if (t != CMIME_ADDRESS_TYPE_FROM && cmime_list_append(message->recipients,ca)!=0)
        return(-1);

    if (found == 0) {
        switch(t) {
            case CMIME_ADDRESS_TYPE_FROM:
                break;
            case CMIME_ADDRESS_TYPE_TO:
                _cmime_internal_set_linked_header_value(message->headers, "To", NULL);
                break;
            case CMIME_ADDRESS_TYPE_CC:
                _cmime_internal_set_linked_header_value(message->headers, "Cc", NULL);
                break;  
            case CMIME_ADDRESS_TYPE_BCC:
                _cmime_internal_set_linked_header_value(message->headers, "Bcc", NULL);
                break;  
        }
    }

    return(0);
}

int cmime_message_add_recipient_encode(CMimeMessage_T *message, const char *recipient, CMimeAddressType_T t) {
    CMimeAddress_T * ca = NULL;
    char *name_encoded = NULL;
    char *encoded = NULL;
    int ret = -1;

    assert(message);
    assert(recipient);

    ca = cmime_address_parse_string(recipient);

    // encode recipient name, if necessary
    if (ca->name != NULL) {
        name_encoded = cmime_string_encode_to_7bit(ca->name,CMIME_STRING_ENCODING_B64);
        cmime_address_set_name(ca,name_encoded);
        ca->parsed = 0;
        free(name_encoded);
    }
    encoded = cmime_address_to_string(ca);
    ret = cmime_message_add_recipient(message,encoded,t);
    free(encoded);
    cmime_address_free(ca);

    return ret;
}

int cmime_message_add_recipient_to(CMimeMessage_T *message, const char *recipient) {
    assert(message);
    assert(recipient);
    
    if(cmime_message_add_recipient(message, recipient, CMIME_ADDRESS_TYPE_TO) != 0)
        return(-1);

    return(0);
}

int cmime_message_add_recipient_cc(CMimeMessage_T *message, const char *recipient) {
    assert(message);
    assert(recipient);

    if(cmime_message_add_recipient(message, recipient, CMIME_ADDRESS_TYPE_CC) != 0) 
        return(-1);

    return(0);
}

int cmime_message_add_recipient_bcc(CMimeMessage_T *message, const char *recipient) {
    assert(message);
    assert(recipient);

    if(cmime_message_add_recipient(message, recipient, CMIME_ADDRESS_TYPE_BCC) != 0) 
        return(-1);

    return(0);
}

void cmime_message_set_content_type(CMimeMessage_T *message, const char *s) {
    _cmime_internal_set_linked_header_value(message->headers,"Content-Type",s);
}

char *cmime_message_get_content_type(CMimeMessage_T *message) {
    return(_cmime_internal_get_linked_header_value(message->headers,"Content-Type"));
}

void cmime_message_set_content_transfer_encoding(CMimeMessage_T *message, const char *s) {
    _cmime_internal_set_linked_header_value(message->headers,"Content-Transfer-Encoding",s);
}

char *cmime_message_get_content_transfer_encoding(CMimeMessage_T *message) {
    return(_cmime_internal_get_linked_header_value(message->headers,"Content-Transfer-Encoding"));
}

void cmime_message_set_mime_version(CMimeMessage_T *message, const char *s) {
    _cmime_internal_set_linked_header_value(message->headers,"Mime-Version",s);
}

/* Return the full mime-version header, with possible comments */
char *cmime_message_get_mime_version(CMimeMessage_T *message) {
    return(_cmime_internal_get_linked_header_value(message->headers,"Mime-Version"));
}

void cmime_message_set_content_id(CMimeMessage_T *message, const char *s) { 
    _cmime_internal_set_linked_header_value(message->headers,"Content-ID",s);
}

char *cmime_message_get_content_id(CMimeMessage_T *message) {
    return(_cmime_internal_get_linked_header_value(message->headers,"Content-ID"));
}

void cmime_message_set_content_description(CMimeMessage_T *message, const char *s) {
    _cmime_internal_set_linked_header_value(message->headers,"Content-Description",s);
}

char *cmime_message_get_content_description(CMimeMessage_T *message) {
    return(_cmime_internal_get_linked_header_value(message->headers,"Content-Description"));
}

void cmime_message_set_date(CMimeMessage_T *message, const char *s) {
    _cmime_internal_set_linked_header_value(message->headers,"Date",s);
}

char *cmime_message_get_date(CMimeMessage_T *message) {
    return(_cmime_internal_get_linked_header_value(message->headers,"Date"));
}

int cmime_message_set_date_now(CMimeMessage_T *message) {
    time_t currtime;                                                    
    char s[128] = {0};     
    int i;
    
    assert(message);
    
    time(&currtime);                                                     
    i = strftime(s,sizeof(s),"%a, %d %b %Y %H:%M:%S %z",localtime(&currtime));
    if (i>0) {
        _cmime_internal_set_linked_header_value(message->headers,"Date",s);
        return(0);
    } else
        return(-1);
}

void cmime_message_set_boundary(CMimeMessage_T *message, const char *boundary) {
    char *s = NULL;
    char *nl = NULL;

    assert(message);
    assert(boundary);
    
    if (message->boundary != NULL)
        free(message->boundary);
        
    message->boundary = strdup(boundary);

    if (message->linebreak)
        nl = message->linebreak;
    else
        nl = CRLF;
    
    asprintf(&s,"multipart/mixed;%s\tboundary=\"%s\"",nl,message->boundary);
    _cmime_internal_set_linked_header_value(message->headers,"Content-Type",s);

    _rebuild_first_part(message);
    free(s);
}

char *cmime_message_generate_boundary(void) {
    char str[21];
    char *boundary = NULL;
    int i;
    static const char text[] = "abcdefghijklmnopqrstuvwxyz"
                               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "0123456789._-=";

    for ( i = 0; i < 20; ++i ) {
        str[i] = text[cmime_util_rand() % (sizeof text - 1)];
    }
    str[20] = '\0';
    
    asprintf(&boundary,"--=_Part_%s",str);
    return(boundary);
}

void cmime_message_add_generated_boundary(CMimeMessage_T *message) {
    char *bound = NULL;

    assert(message);

    bound = cmime_message_generate_boundary();
    cmime_message_set_boundary(message,bound);
    free(bound);
}

int cmime_message_from_file(CMimeMessage_T **message, const char *filename, int header_only) {
    int fd;
    FILE *fp;
    struct stat sb;
    char *addr;
    char *p = NULL;
    int ret = 0;
    _StrippedData_T *sd = NULL;

    assert((*message));
    assert(filename);
    assert((header_only == 0)||(header_only == 1));

    if (stat(filename,&sb) != 0) {
        perror("libcmime: stat failed");
        return(-1); 
    }

    if(!S_ISREG(sb.st_mode))
        return(-2); 

    /* open file for reading */
    fp = fopen(filename,"rb");
    if (fp == NULL) {
        perror("libcmime: error opening file for reading");
        return(-3);
    }
    fd = fileno(fp);
    if (fd == -1) {
        perror("libcmime: error opening file for reading");
        return(-3);
    }

#ifdef _WIN32
    p = (char *)calloc(sb.st_size + 1,sizeof(char));
    {
        char *q = p;
        size_t left = sb.st_size;
        do
        {
            size_t z = fread(q, 1, left, fp);
            left -= z;
        }
        while (left > 0);
    }
#else
    /* map file to memory */
    addr = mmap (NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    if (addr == MAP_FAILED) {
        fclose(fp);
        perror("libcmime: error mmapping the file");
        return(-3);
    }
    p = (char *)calloc(sb.st_size + 1,sizeof(char));
    memcpy(p, addr, sb.st_size);
#endif
    p[strlen(p)] =  '\0';

    if (fclose(fp)!=0)
        perror("libcmime: error failed closing file");

#ifndef _WIN32
    if (munmap (addr, sb.st_size) == -1) {
        perror("libcmime: error on munmap");
        return(-3); 
    }
#endif

    sd = _strip_message(message,p,header_only);
    ret = cmime_scanner_scan_buffer(message, sd->stripped);

    if (sd->stripped != p) {
        _add_stripped_bodies(message, sd);
    }
    
    free(sd->mime_bodies->node);
    free(sd->mime_bodies);
    free(sd);
    free(p);  

    return(ret);
}

int cmime_message_to_file(CMimeMessage_T *message, const char *filename) {
    FILE *fp = NULL;
    char *msg_string = NULL;
    int ret = 0;

    assert(message);
    assert(filename);

    msg_string = cmime_message_to_string(message);
    if ((fp = fopen(filename,"wb"))==NULL) {
        perror("libcmime: error writing file");
        return(-1);
    }
    if (msg_string != NULL) {
        ret = fwrite(msg_string,strlen(msg_string),1,fp);
        if (fclose(fp) != 0) {
            perror("libcmime: error closing file");
            return(-1);
        }
        free(msg_string);
    }
    return(ret);
}

char *cmime_message_to_string(CMimeMessage_T *message) {
    char *out = NULL;
    CMimeListElem_T *e = NULL;
    CMimeListElem_T *r = NULL;
    CMimeHeader_T *h = NULL;
    CMimeAddress_T *addr = NULL;
    CMimeAddressType_T t = -1;
    char *s = NULL;
    char *s2 = NULL;
    int count = 0;
    int pos = 0;

    assert(message);
    out = (char *)calloc(sizeof(char),sizeof(char));

    if (message->linebreak==NULL)
        message->linebreak = strdup(CRLF);

    /* check message id, and append if necessary */
    if (_cmime_internal_get_linked_header_value(message->headers,"Message-ID")==NULL) 
        cmime_message_add_generated_message_id(message);

    e = cmime_list_head(message->headers);
    while(e != NULL) {
        h = (CMimeHeader_T *)cmime_list_data(e);
        if (strcasecmp(h->name,"to")==0) 
            t = CMIME_ADDRESS_TYPE_TO;
        else if (strcasecmp(h->name,"cc")==0)
            t = CMIME_ADDRESS_TYPE_CC;
        else if (strcasecmp(h->name,"bcc")==0)
            t = CMIME_ADDRESS_TYPE_BCC;
        else if (strcasecmp(h->name,"from")==0)
            t = CMIME_ADDRESS_TYPE_FROM;

        if (t != -1) {
            asprintf(&s,"%s:",h->name);
            pos = strlen(s);
            if (t == CMIME_ADDRESS_TYPE_FROM) {
                s2 = cmime_address_to_string(message->sender);

                if (strlen(s2) > 0) {
                    /* if first char is not space or tab append a single space */
                    if ((s2[0] != (unsigned char)32) && ((s2[0] != (unsigned char)9))) {
                        s = (char *)realloc(s,strlen(s) + (3 * sizeof(char)));
                        s[pos++] = ' ';
                        s[pos++] = '\0';
                    }
                    s = (char *)realloc(s,strlen(s) + strlen(s2) + sizeof(char));
                    strcat(s,s2);
                }
                free(s2);    
                
            } else {
                r = cmime_list_head(message->recipients);
                count = 0;
                while(r != NULL) {
                    addr = (CMimeAddress_T *)cmime_list_data(r);
                    if (addr->type == t) {
                        s2 = cmime_address_to_string(addr);
                        if (strlen(s2) > 0) {
                            if (count==0) {
                                /* if first char is not space or tab append a single space */
                                if ((s2[0] != (unsigned char)32) && ((s2[0] != (unsigned char)9))) {
                                    s = (char *)realloc(s,strlen(s) + (2 * sizeof(char)));
                                    s[pos++] = ' ';
                                    s[pos++] = '\0';
                                }
                            }
                            count++;
                            s = (char *)realloc(s,strlen(s) + strlen(s2) + sizeof(char));
                            strcat(s,s2);
                        }
                        free(s2);    
			            
                        if (r->next != NULL) {
                            addr = (CMimeAddress_T *)cmime_list_data(r->next);
                            if (addr->type == t) {
                                s = (char *)realloc(s,strlen(s) + 1 + sizeof(char));
                                strcat(s,",");
                            }
                        }
                    }

                    r = r->next;
                }
            }
            t = -1;
        } else {
            s = cmime_header_to_string(h);
        }
    
        out = (char *)realloc(out,strlen(out) + strlen(s) + strlen(message->linebreak) + sizeof(char));
        strcat(out,s);
        strcat(out,message->linebreak);
        free(s);
        
        e = e->next;
    }

    /* check message gap */
    if (message->gap == NULL) {
        if (message->linebreak != NULL)
            message->gap = strdup(message->linebreak);
        else
            message->gap = strdup(CRLF);
    }
    _append_string(&out,message->gap);

    _append_child_parts(&out, message->boundary, message);
        
    return(out);
}

int cmime_message_from_string(CMimeMessage_T **message, const char *content, int header_only) {
    _StrippedData_T *sd = NULL;
    int ret = 0;
    char *p = NULL;

    assert((*message));
    assert(content);
    assert((header_only == 0)||(header_only == 1));
  
    p = strdup(content);
    sd = _strip_message(message,p,header_only);
    
    ret = cmime_scanner_scan_buffer(message, sd->stripped);

    if (sd->stripped != p) {
        _add_stripped_bodies(message,sd);    
    }

    free(sd->mime_bodies->node);
    free(sd->mime_bodies);
    free(sd);
    free(p);

    return(ret);
}

void cmime_message_set_subject(CMimeMessage_T *message, const char *s) {
    _cmime_internal_set_linked_header_value(message->headers,"Subject",s);
}

void cmime_message_set_subject_encode(CMimeMessage_T *message, const char *s) {
    char *encoded = NULL;

    assert(message);
    assert(s);

    encoded = cmime_string_encode_to_7bit(s,CMIME_STRING_ENCODING_B64);

    cmime_message_set_subject(message, encoded);
    free(encoded);
}

char *cmime_message_get_subject(CMimeMessage_T *message) {
    return(_cmime_internal_get_linked_header_value(message->headers,"Subject"));
}

void cmime_message_prepend_subject(CMimeMessage_T *message, const char *s) {
    char *orig_subject = NULL;
    char *new_subject = NULL;

    orig_subject = cmime_message_get_subject(message);
    asprintf(&new_subject, "%s %s", s, orig_subject);
    cmime_message_set_subject(message, new_subject);
    if(new_subject != NULL)
        free(new_subject);
}

void cmime_message_append_subject(CMimeMessage_T *message, const char *s) {
    char *orig_subject = NULL;
    char *new_subject = NULL;

    orig_subject = cmime_message_get_subject(message);
    asprintf(&new_subject, "%s %s", orig_subject, s);
    cmime_message_set_subject(message, new_subject);
    if(new_subject != NULL)
        free(new_subject);
}

char *cmime_message_generate_message_id(void) {
    char *mid = NULL;
    char *hostname = NULL;
    static const char base36[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int i,i2;
    int pos = 0;

    hostname = (char *)malloc(MAXHOSTNAMELEN);
    gethostname(hostname,MAXHOSTNAMELEN);
        
    mid = (char *)malloc(20 + strlen(hostname));
    for(i=0; i < 2; i++) {
        for (i2=0; i2<8; i2++) 
            mid[pos++] = base36[cmime_util_rand() % 36];
        
        if (i==0)
            mid[pos++] = '.';
        else
            mid[pos++] = '@';
    }
    mid[pos] = '\0';
    strcat(mid,hostname);
    free(hostname);
    return(mid);
}

void cmime_message_add_generated_message_id(CMimeMessage_T *message) {
    char *mid = NULL;

    assert(message);
    mid = cmime_message_generate_message_id();
    cmime_message_set_message_id(message,mid);
    free(mid);
}

int cmime_message_set_body(CMimeMessage_T *message, const char *content) {
    CMimePart_T *p = NULL;
    CMimeInfo_T *mi = NULL;

    assert(message);
    assert(content);  

    /* check if message object has a boundary and more the 1 mime parts */ 
    if ((message->boundary != NULL) && (message->parts->size > 1))
        return(-1);

    if (message->parts->size == 1) {
        p = (CMimePart_T *)cmime_list_pop_head(message->parts);
        cmime_part_free(p);
    }
    
    mi = cmime_util_info_get_from_string(content);
    if (mi!=NULL) {
        if (mi->combined != NULL)
            cmime_message_set_content_type(message, mi->combined);

        cmime_util_info_free(mi);
    }

    p = cmime_part_new();    
    cmime_part_set_content(p,content);
    cmime_list_append(message->parts,p);

    /* check message gap */
    if (message->gap == NULL) {
        if (message->linebreak != NULL)
            message->gap = strdup(message->linebreak);
        else
            message->gap = strdup(CRLF);
    }

    return(0);
}

int cmime_message_append_part(CMimeMessage_T *message, CMimePart_T *part) {
    CMimeListElem_T *elem = NULL;
    CMimePart_T *prev = NULL;
    assert(message);
    assert(part);
    
    if (message->parts->size == 1) {
        _rebuild_first_part(message); 
    }

    if (message->parts->size >= 1) {
        elem = cmime_list_tail(message->parts);
        prev = cmime_list_data(elem);
        prev->last = 0;
        part->last = 1;
        part->parent_boundary = strdup(prev->parent_boundary);
        cmime_list_append(message->parts,part);
        return(0);
    }  
    return(0);
}


void cmime_message_add_attachment(CMimeMessage_T *message, char *attachment) {

    assert(message);
    assert(attachment);

    CMimePart_T *part = cmime_part_new();
    CMimeListElem_T *elem = NULL;
    CMimePart_T *prev = NULL;

    /* check if there is a previous part */
    if (message->parts->size >= 1) {
        elem = cmime_list_tail(message->parts);
        prev = cmime_list_data(elem);
        prev->last = 0;
        
    }
    cmime_message_add_generated_boundary(message);
    cmime_part_from_file(&part, attachment,message->linebreak);
    part->parent_boundary = strdup(message->boundary);
    part->last = 1;
    cmime_list_append(message->parts,part);

}

CMimeMessage_T *cmime_message_create_skeleton(const char *sender, const char *recipient, const char *subject) {
    CMimeMessage_T *message = NULL;
    assert(sender);
    assert(recipient);
    assert(subject);

    message = cmime_message_new();
    // set sender, recipient, date, subject
    cmime_message_set_sender(message, sender);
    if (cmime_message_add_recipient_to(message, recipient) != 0) {
        cmime_message_free(message);
        return(NULL);
    }
    cmime_message_set_subject(message, subject);
    if (cmime_message_set_date_now(message) != 0) {
        cmime_message_free(message);
        return(NULL);
    }
    cmime_message_add_generated_message_id(message);
    return message;
}


int cmime_message_part_remove(CMimeMessage_T *message, CMimePart_T *part) {
    int ret = 0;
    CMimeListElem_T *elem = NULL;
    CMimePart_T *p = NULL;
    CMimePart_T *tf = NULL;
    CMimePart_T *prev = NULL;

    assert(message);
    assert(part);

    elem = cmime_list_head(message->parts);
    while(elem != NULL) {
        p = (CMimePart_T *)cmime_list_data(elem);
        if (p == part) {
            if (part->last == 1) {
                prev = (CMimePart_T *)cmime_list_data(elem->prev);
                prev->last = 1;
            }
            ret = cmime_list_remove(message->parts,elem,(void *)&tf);
            break;
        }
        elem = elem->next;
    }

    cmime_part_free(tf);

    return(ret);
}

int cmime_message_add_child_part(CMimeMessage_T *message, CMimePart_T *part, CMimePart_T *child, CMimeMultipartType_T subtype) {
    CMimeListElem_T *elem = NULL;
    CMimePart_T *current_part = NULL;
    char *boundary = NULL;
    char *content_type = NULL;
    char *nl = NULL;
    char *s = NULL;

    assert(message);
    assert(part);
    assert(child);

    elem = cmime_list_head(message->parts);
    while(elem != NULL) {
        current_part = cmime_list_data(elem);
        if (current_part == part) {
            /* add a boundary, if not present */
            if (part->boundary == NULL) {
                if (message->linebreak)
                    nl = message->linebreak;
                else
                    nl = CRLF;

                switch(subtype) {
                    case CMIME_MULTIPART_MIXED:
                        asprintf(&content_type,"multipart/mixed;%s\t",nl);
                        break;
                    case CMIME_MULTIPART_DIGEST:
                        asprintf(&content_type,"multipart/digest;%s\t",nl);
                        break;
                    case CMIME_MULTIPART_ALTERNATIVE:
                        asprintf(&content_type,"multipart/alternative;%s\t",nl);
                        break;
                    case CMIME_MULTIPART_MESSAGE:
                        asprintf(&content_type,"message/rfc822;%s\t",nl);
                        break;
                    default:
                        asprintf(&content_type,"multipart/mixed;%s\t",nl);
                        break;
                }

                boundary = cmime_message_generate_boundary();
                asprintf(&s,"boundary=\"%s\"",boundary);
                part->boundary = boundary;
                child->parent_boundary = strdup(boundary);

                content_type = (char *)realloc(content_type,strlen(content_type) + strlen(s) + sizeof(char));
                strcat(content_type,s);
                free(s);
                if (part->last == 1) {
                    part->last = 0;
                    child->last = 1;
                }

                cmime_part_set_content_type(part, content_type);
                free(content_type);
                if (cmime_list_insert_next(message->parts,elem,child)!=0)
                    return(-1);
                
                return(0);
            }
        }
        elem = elem->next;
    }

    return(-1);
}

