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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "../src/cmime_base64.h"

#include "test_data.h"

int main (int argc, char const *argv[])	{
	char *out = NULL;
	char *out2 = NULL;
	FILE *fp = NULL;
	FILE *fp2 = NULL;
	long size = 0;
	char *in_data = NULL;
	char *out_data = NULL;
	char *in_64 = NULL;
	
	out = cmime_base64_encode_string(test_string1);
	free(out);
	
	out = cmime_base64_encode_string(test_string5);
	assert(strcmp(out,test_string5_base64)==0);
	out2 = cmime_base64_decode_string(out);
	assert(strcmp(test_string5,out2)==0);
	free(out2);
	free(out);
	
	if ((fp = fopen(CONTENT_FILE, "rb")) == NULL) 
		return(-1);
		
	if (fseek(fp, 0, SEEK_END)!=0)
		return(-1);
		
	size = ftell(fp);
	rewind(fp);	
	in_data = (char*) calloc(sizeof(char), size + 20);
	fread(in_data, 1, size, fp);
	if(ferror(fp))
		return(-1);

	rewind(fp);
	if ((fp2 = fopen("test_base64.out","wb")) == NULL)
		return(-1);

	cmime_base64_encode_file(fp,fp2,0);
	in_64 = cmime_base64_encode_string(in_data);
	
	size = ftell(fp2);
	rewind(fp2);
	out_data = (char *)calloc(sizeof(char),size + 20);
	fp2 = freopen("test_base64.out","rb",fp2);
	fread(out_data,1,size,fp2);
	if (ferror(fp2))
		return(-1);

	assert(strcmp(in_64,out_data)==0);
	free(in_64);
	free(out_data);
	free(in_data);
	
	fclose(fp2);
	fclose(fp);
	
	if (remove("test_base64.out")!=0)
		return(-1);
	
	return(0);
}
