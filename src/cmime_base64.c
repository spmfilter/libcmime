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

/* 
 * Original implementation by Sebastian Jaekel <sebastian.jaekel@googlemail.com>
 */

#define _GNU_SOURCE

#include "cmime_base64.h"

/** translation table (RFC 1113) */
static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

/* translation table to decode */
static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

void cmime_base64_encode_block(unsigned char in[3], unsigned char out[4], int len) {
    out[0] = cb64[ in[0] >> 2 ];
    out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
    out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
    out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
}

void cmime_base64_decode_block(unsigned char in[4], unsigned char out[3]) {   
    out[ 0 ] = (unsigned char ) (in[0] << 2 | in[1] >> 4);
    out[ 1 ] = (unsigned char ) (in[1] << 4 | in[2] >> 2);
    out[ 2 ] = (unsigned char ) (((in[2] << 6) & 0xc0) | in[3]);
}

void cmime_base64_encode_file(FILE *infile, FILE *outfile, int linelen) {
    unsigned char in[3], out[4];
    int i = 0;
    int len = 0;
    int blocksout = 0;

    while(!feof(infile)) {
        len = 0;

        for(i=0; i<3; i++) {
            in[i] = (unsigned char)getc(infile);

            if(!feof(infile)) {
                len++;
            } else {
                in[i] = 0;
            }
        }

        if(len) {
            cmime_base64_encode_block(in, out, len);
            for(i=0; i<4; i++) {
                putc(out[i], outfile);
            }
            
            blocksout++;
        }

        if((blocksout >= (linelen / 4) || feof(infile)) && (linelen != 0)) {
            if(blocksout) {
                fprintf(outfile, "\r\n");
            }

            blocksout = 0;
        }
    }
}

void cmime_base64_decode_file(FILE *infile, FILE *outfile) {
    unsigned char in[4], out[3], v;
    int i, len;

    while(!feof(infile)) {
        for(len = 0, i = 0; i < 4 && !feof(infile); i++) {
            v = 0;
            while(!feof(infile) && (v == 0)) {
                v = (unsigned char) getc( infile );
                v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
                if( v ) {
                    v = (unsigned char) ((v == '$') ? 0 : v - 61);
                }
            }
            
            if(!feof(infile)) {
                len++;
                if(v) {
                    in[ i ] = (unsigned char) (v - 1);
                }
            } else {
                in[i] = 0;
            }
        }
        if(len) {
            cmime_base64_decode_block(in, out);
            for(i = 0; i < len - 1; i++) {
                putc(out[i], outfile);
            }
    }
    }
}

char *cmime_base64_encode_string(const char *source) {
    unsigned char in[3], out[4];
    int i,j;
    int len;
    char *target = NULL;
    int pos = 0;

    /* laenge der ein/ausgabe */
    int source_len = strlen(source);
    int target_len = source_len + (0.35 * source_len) + 1;

    target = (char *)calloc(target_len, sizeof(char) + 1);

    for(j=0; j<source_len; j+=3) {
        len = 0;

        for(i=0; i<3; i++) {
            if((i + j) < source_len) {
                in[i] = (unsigned char)source[j + i];
                len++;
            } else {
                in[i] = 0;
            }
        }

        cmime_base64_encode_block(in, out, len);
        for(i=0; i<4; i++) {
            target[pos++] = out[i];
        }
    }

    return(target);
}

char *cmime_base64_decode_string(const char *source) {
    unsigned char in[4], out[3], v;
    int i,j;
    char *target = NULL;
    int pos = 0;
    
    int source_len = strlen(source);
    int target_len = source_len - (0.35 * source_len) + 1;
    
    target = (char *)calloc(target_len,sizeof(char) + 1);

    for(j=0; j<source_len; j+=4) {
        v = 0;
        for(i=0; i<4; i++) {
            v = (unsigned char)source[j + i];
            v = (unsigned char) ((v < 43 || v > 122) ? 0 : cd64[ v - 43 ]);
            v = (unsigned char) ((v == '$') ? 0 : v - 61);
            
            if (v)
                in[i] = (unsigned char)(v - 1);
            else 
                in[i] = 0;
        }
    
        cmime_base64_decode_block(in, out);
        for(i=0; i<3; i++) {
            target[pos++] = out[i]; 
        }
    }

    return(target);
}
