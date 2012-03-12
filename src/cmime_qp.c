/* libcmime - A C mime library
 * Copyright (C) 2012 SpaceNet AG and Werner Detter (werner@aloah-from-hell.de)
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

/* 
 * quoted-printable implementation was inspired by altermime http://www.altermime.org
 */
#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "cmime_qp.h"

/* Quoted-Printable decoding Mode */
#define DECODE_QP_MODE_DEFAULT  0
#define DECODE_QP_MODE_ISO      1

/* Quoted-Printable translation-table */
static unsigned char hexconv[256]={
               20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,\
               20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,\
               20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,\
                0,    1,    2,    3,    4,    5,    6,    7,    8,    9,   20,   20,   20,   20,   20,   20,\
               20,   10,   11,   12,   13,   14,   15,   20,   20,   20,   20,   20,   20,   20,   20,   20,\
               20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,\
               20,   10,   11,   12,   13,   14,   15,   20,   20,   20,   20,   20,   20,   20,   20,   20,\
               20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,\
               20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,\
               20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,\
               20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,\
               20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,\
               20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,\
               20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,\
               20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,\
               20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,   20 \
};

char *cmime_qp_decode(char *line_in, int mode, char esc_char)  {
   
    char *line_out = NULL;
    char c;                                                       /* output character */
    int opos = 0;                                                 /* output positioning */
    int ipos;                                                   	/* input positioning */
    int len = strlen(line_in);                      							/* length of inputstring */

    /* we malloc size of line_in char as through decoding string just can get shorter, not longer */
    line_out = malloc(strlen(line_in)+1*sizeof(char));

    /* we now go through every character of the string */                           
    for(ipos = 0; ipos < len; ipos++) {
        c = line_in[ipos];
   
        /* start cracking if we reach the esc char */
        if (c == esc_char) {
            /* check if there are to other chars */
            if((ipos+2)<len) {
                /* remember position */
                int orig_pos = ipos;
                
                /* absorb whitespaces */
                if (1) {
                    char *w = &(line_in[ipos +1]);
                    while ((*w == '\t') || (*w == ' ')) {w++;ipos++;}
                }

                /* check for linebreak */   
                if (( line_in[ipos +1] == '\n') || (line_in[ipos +1] == '\r' )) {
                    ipos++;
                    if ((ipos+1 < len) && (( line_in[ipos +1] == '\n') || (line_in[ipos +1] == '\r' ))) {
                        ipos++;
                    }
                    continue;
                } else {
                    /* go back to origin pos */
                    ipos = orig_pos;
                    /* convert char from hex to dec */
                    if(ipos < len - 1) {
                        if(hexconv[(int)line_in[ipos+1]] == 20 || hexconv[(int)line_in[ipos+2]] == 20) {
                            printf("invalid character for quoted-printable detected\n");
                        } else {
                            /* convert an skip 2 chars */
                            c = (char)hexconv[(int)line_in[ipos+1]]*16 + hexconv[(int)line_in[ipos+2]];
                            ipos+=2;
                        }
                    } else {
                        printf("no chars left!");
                    }
                }
            } else {
            		line_out[opos] = '\0';
                /* if we're out of chars, quit */
                break;
            }
        } else if (( c == '_') && (mode == DECODE_QP_MODE_ISO)) {
        	/* RFC2047 says: if you encounter a '_' charactier in the ISO encoding then this must be converted to a space */
          c = ' ';
        }
        /* put in the new character, converted or not */
        line_out[opos] = c;
        opos++;
    }
    /* terminate string */
    line_out[opos]='\0';

    return line_out;
}

/**
 * HELPERS for Decoding, so just passing the char pointer is necessary
 */
char *cmime_qp_decode_text ( char *line_in) {
	char *line_out = NULL;
	line_out = cmime_qp_decode(line_in, DECODE_QP_MODE_DEFAULT,'=');
	return line_out;
}

char *cmime_qp_decode_iso ( char *line_in) {
	char *line_out = NULL;
	line_out = cmime_qp_decode(line_in, DECODE_QP_MODE_DEFAULT,'=');
	return line_out;
}

char *cmime_qp_decode_multipart ( char *line_in) {
	char *line_out = NULL;
	line_out = cmime_qp_decode(line_in, DECODE_QP_MODE_DEFAULT,'%');
	return line_out;
}

char *cmime_qp_encode(char *line_in, char *lt) {	 
	char *line_out = NULL;   													/* output array */
	size_t line_out_s; 																/* size of output array */
	char CRLF[]="\r\n"; 															/* line terminator, if none defined we use this one */
	
	char *line_s;
	char *line_e;
	char *p;
	char *op;
	char *pp; 
	char paragraph[100];
	size_t pp_remain = 100;
	size_t out_remain;
	char *in_data_limit = line_in + strlen(line_in); 	/**/
	size_t cur_line_len = 0;

	/* output array can only be 3 times size of line_in + 1 for \0 */
	line_out_s = strlen(line_in)*3+1;
  line_out = malloc(line_out_s*sizeof(char));

	/* set line terminator in case there is none given */
	if(lt == NULL) lt = CRLF;
	
	line_s = NULL;
	line_e = line_in;
	
	/* set the output buffer variables */
	op = line_out;
	out_remain = line_out_s;

	do{
		char charout[4];
		int charout_size = 0;

		if (line_e != '\0') {
			if (line_s == NULL) {
				line_s = line_in;
			} else {
				line_s = line_e;
			}

			line_e = strstr(line_s, lt);
			if (line_e == NULL) {
					/* no CLRF found */
			  	line_e = line_in + strlen(line_in);
			} else { 
				line_e += strlen(lt);
			}
		}

		/* init paragraph */
		paragraph[0] = '\0';
		pp = paragraph;
		pp_remain = sizeof(paragraph);
		cur_line_len = 0;
		/* set p to point to the start of the new line that we have to encode */
		p = line_s;

		while ((p < line_e)) {

			if (*p < ' ' || *p == '=' || *p > 126) {
				/* encode as hex */
				snprintf( charout, sizeof(charout), "=%02X", (unsigned char)*p);
				charout_size = 3;
			} else {
				/* encode verbatim */
				snprintf( charout, sizeof(charout), "%c", *p);
				charout_size = 1;
			}

			if (cur_line_len + charout_size >= 79) { 							// Was 76, updated to fix Outlook problems
				snprintf(op, out_remain, "%s=%s", paragraph, lt);
				op += strlen(paragraph); 															// +3; /** jump the output + =\r\n **/
				out_remain -= (strlen(paragraph)); 										// 	Was +3, updated to fix Outlook problems
				
				/* reinitialize the paragraph */
				paragraph[0] = '\0';
				pp_remain = sizeof(paragraph);
				pp = paragraph;
				cur_line_len=-1;
			}

			snprintf(pp, pp_remain, "%s", charout);
			pp += charout_size;
			pp_remain -= charout_size;
			p++;
			cur_line_len += charout_size; 
		}
		/* for each char in the line to be converted */
		snprintf(op, out_remain, "%s", paragraph);
		op += (strlen(paragraph) +2);
		out_remain -= (strlen(paragraph) +2);

	} while ((line_e < in_data_limit) && (*line_e != '\0'));

	return line_out;
}

char *cmime_qp_rm_charenc(char *line_in) {
	char *line_out = NULL;
	int ipos = 0;
	int opos = 0;
	int line_in_len = strlen(line_in);

	line_out = malloc(strlen(line_in)+1*sizeof(char));
	for(ipos=0; ipos<line_in_len; ipos++) {
		if(line_in[ipos] == '=') {
			int orig_ipos = ipos;

			if(line_in[ipos+1] == '?') {
				ipos++;
				
				do {
					ipos++;
				} while (line_in[ipos] != '?');

				ipos++;

				if(tolower(line_in[ipos]) == 'q' || tolower(line_in[ipos]) == 'b') {
					ipos+=2;
				
					do {
						line_out[opos] = line_in[ipos];
						opos++;
						ipos++;	
					} while (line_in[ipos] != '?');

					ipos++;
					continue;
				}
			} else {
				ipos = orig_ipos;
				line_out[opos] = line_in[ipos];
				opos++;
			}
		} else {	
			line_out[opos] = line_in[ipos];
			opos++;
		}
	}
	line_out[opos++] = '\0';
	 return line_out;
}