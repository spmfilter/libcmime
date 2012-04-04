/* libcmime - A C mime library
 * Copyright (C) 2012 SpaceNet AG and Axel Steiner <ast@treibsand.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
#include "cmime_qp.h"


typedef enum _BoundaryType {
    BOUNDARY_OPEN, 
    BOUNDARY_CLOSE,
} _BoundaryType_T;

void _recipients_destroy(void *data) {
    assert(data);
    CMimeAddress_T *ca = (CMimeAddress_T *)data;
    cmime_address_free(ca);
}

void _rebuild_first_part(CMimeMessage_T *message) {
    CMimeListElem_T * e = NULL;
    CMimePart_T *p = NULL;
    CMimeInfo_T *mi = NULL;
    char *s = NULL;
    char *content_type = NULL;
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
            mi = cmime_util_info_get(s);

            if (cmime_string_is_7bit(s)==0)
                cmime_part_set_content_transfer_encoding(p, "7bit"); 
            else
                /* TODO: check other transfer encodings */
                cmime_part_set_content_transfer_encoding(p, "8bit");
            

            if (message->linebreak == NULL) {
                nl = _cmime_internal_determine_linebreak(s);
                if (nl == NULL)
                    nl = CRLF;

                message->linebreak = strdup(nl);
            }
            asprintf(&content_type,"%s;%s\tcharset=%s",mi->mime_type,message->linebreak,mi->mime_encoding);
            cmime_part_set_content_type(p, content_type);
            free(content_type);

            p->last = 1;
            cmime_util_info_free(mi);
        }
    }
}

void _append_boundary(char **out, const char *boundary,const char *linebreak, _BoundaryType_T type) {
    char *s = NULL;
    if(boundary != NULL) {
        if (type == BOUNDARY_OPEN)
            asprintf(&s,"--%s%s",boundary,linebreak);
        else if (type == BOUNDARY_CLOSE)
            asprintf(&s,"--%s--%s",boundary,linebreak);
        
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

CMimeMessage_T *cmime_message_new(void) {
    CMimeMessage_T *message = NULL;
    
    message = (CMimeMessage_T *)calloc((size_t)1, sizeof(CMimeMessage_T));

    if (cmime_list_new(&message->headers,_cmime_internal_header_destroy)!=0)
        return(NULL);
    
    message->sender = NULL;
    if (cmime_list_new(&message->recipients,_recipients_destroy)!=0) 
            return(NULL);

    message->boundary = NULL;
    message->gap = NULL;
    message->linebreak = NULL;

    if (cmime_list_new(&message->parts,_cmime_internal_parts_destroy)!=0) 
            return(NULL);

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
    
    free(message);
}

/* Set sender of message */
void cmime_message_set_sender(CMimeMessage_T *message, const char *sender) {
    CMimeAddress_T *ca = NULL;
    char *s = NULL;

    assert(message);
    assert(sender);

    ca = cmime_address_parse_string(sender);
    if (message->sender != NULL) {
        cmime_address_free(message->sender);
        s = _cmime_internal_get_linked_header_value(message->headers, "From");
        if (s != NULL)
            free(s);
    }
    ca->type = CMIME_ADDRESS_TYPE_FROM;
    message->sender = ca;
    _cmime_internal_set_linked_header_value(message->headers, "From", NULL);
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
    
    return(h);
}

int cmime_message_add_recipient(CMimeMessage_T *message, const char *recipient, CMimeAddressType_T t) {
    CMimeAddress_T * ca = NULL;
    CMimeListElem_T *elem;
    CMimeAddress_T *tca = NULL;
    char *s1 = NULL;
    char *s2 = NULL;
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

    // check if given recipient already exists
    elem = cmime_list_head(message->recipients);
    while(elem != NULL) {
        tca = (CMimeAddress_T *)cmime_list_data(elem);
        
        if (cmime_address_get_type(tca) == t) {
            s1 = cmime_address_to_string(tca);
            s2 = cmime_address_to_string(ca);
            if (strcmp(s1,s2)==0) {
                cmime_list_remove(message->recipients,elem,NULL);
                found = 1;
                break;
            }
            free(s1);
            free(s2);
        }
        elem = elem->next;
    }

    if (cmime_list_append(message->recipients,ca)!=0)
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

    srand(time(NULL));

    for ( i = 0; i < 20; ++i ) {
        str[i] = text[rand() % (sizeof text - 1)];
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

int cmime_message_from_file(CMimeMessage_T **message, const char *filename) {
    struct stat fileinfo;
    FILE *fp = NULL;
    int ret = 0;
    
    assert((*message));
    assert(filename);
    
    if (stat(filename,&fileinfo) != 0)
        return(-1); /* stat error */
    
    if(!S_ISREG(fileinfo.st_mode))
        return(-2); /* not regular file */

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        perror("libcmime: error opening file");
        return(-3);
    }

    ret = cmime_scanner_scan_file(message,fp);
    
    if (fclose(fp)!=0) {
        perror("libcmime: error closing file");
        return(-1);
    }
    
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
    CMimePart_T *p = NULL;
    CMimeAddress_T *addr = NULL;
    CMimeAddressType_T t = -1;
    char *s = NULL;
    char *s2 = NULL;
    int len = 0;

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
            asprintf(&s,"%s: ",h->name);
            if (t == CMIME_ADDRESS_TYPE_FROM) {
                s2 = cmime_address_to_string(message->sender);
                s = (char *)realloc(s,strlen(s) + strlen(s2) + sizeof(char));
                strcat(s,s2);
                free(s2);    
            } else {
                r = cmime_list_head(message->recipients);
                while(r != NULL) {
                    addr = (CMimeAddress_T *)cmime_list_data(r);
                    if (addr->type == t) {
                        s2 = cmime_address_to_string(addr);
                        s = (char *)realloc(s,strlen(s) + strlen(s2) + sizeof(char));
                        strcat(s,s2);
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

    e = cmime_list_head(message->parts);
    while(e != NULL) {
        p = (CMimePart_T *)cmime_list_data(e);

        len = strlen(out);
        if ((out[len - 1] != '\r') || out[len - 1] != '\n')
            _append_string(&out,message->linebreak);
        
        _append_boundary(&out, p->parent_boundary, message->linebreak, BOUNDARY_OPEN);
        s = cmime_part_to_string(p,message->linebreak);
        if (s)
            _append_string(&out,s);

        if (p->last == 1) {
            len = strlen(s);
            if ((s[len - 1] != '\r') || s[len - 1] != '\n')
                _append_string(&out,message->linebreak);
            
            _append_boundary(&out, p->parent_boundary, message->linebreak, BOUNDARY_CLOSE);
            if (p->postface != NULL) 
                _append_string(&out,p->postface);    
        }

        if (s)
            free(s);
        e = e->next;
    }
    
    return(out);
}

int cmime_message_from_string(CMimeMessage_T **message, const char *content) {
    FILE *fp = NULL;
    int ret = 0;
    
    assert((*message));
    assert(content);

    fp = tmpfile();
    if (fp == NULL) {
        perror("libcmime: error creating temporary file");
        return(-2);
    }
    if (fwrite(content,strlen(content),1,fp)==0)
        return(-3);
    
    rewind(fp);
    ret = cmime_scanner_scan_file(message,fp);  
    if (fclose(fp)!=0) {
        perror("libcmime: error closing file");
        return(-1);
    }
    
    return(ret);
}

void cmime_message_set_subject(CMimeMessage_T *message, const char *s) {
    _cmime_internal_set_linked_header_value(message->headers,"Subject",s);
}

char *cmime_message_get_subject(CMimeMessage_T *message) {
    return(_cmime_internal_get_linked_header_value(message->headers,"Subject"));
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
    srandom(time(NULL));
    for(i=0; i < 2; i++) {
        for (i2=0; i2<8; i2++) 
            mid[pos++] = base36[random() % 36];
        
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
    
    mi = cmime_util_info_get(content);
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

    if (message->parts->size >= 1) {
        elem = cmime_list_tail(message->parts);
        prev = cmime_list_data(elem);
        prev->last = 0;
        part->last = 1;
        cmime_list_append(message->parts,part);
        return(0);
    } else if (message->parts->size == 1) {
        _rebuild_first_part(message);    
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
    cmime_part_from_file(&part, attachment);
    part->parent_boundary = strdup(message->boundary);
    part->last = 1;
    cmime_list_append(message->parts,part);

}





