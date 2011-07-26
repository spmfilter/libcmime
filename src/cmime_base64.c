/* libcmime - A C mime library
 * Copyright (C) 2011 Axel Steiner <ast@treibsand.com>
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
 * Original implementation by Sebastian Jaekel <sebastian.jaekel@googlemail.com>
 */

#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmime_base64.h"

/** translation table (RFC 1113) */
static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void cmime_base64_encode_block(unsigned char in[3], unsigned char out[4], int len) {
	out[0] = cb64[ in[0] >> 2 ];
	out[1] = cb64[ ((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4) ];
	out[2] = (unsigned char) (len > 1 ? cb64[ ((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6) ] : '=');
	out[3] = (unsigned char) (len > 2 ? cb64[ in[2] & 0x3f ] : '=');
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

		if(blocksout >= (linelen / 4) || feof(infile)) {
			if(blocksout) {
				fprintf(outfile, "\r\n");
			}

			blocksout = 0;
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

	target = (char *)calloc(target_len, sizeof(char));

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
