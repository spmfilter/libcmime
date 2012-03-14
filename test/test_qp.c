/* libcmime - A C mime library
 * Copyright (C) 2012 Werner Detter <werner@aloah-from-hell.de>
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "../src/cmime_qp.h"
#include "../src/cmime_string.h"
#include "test_data.h"

int main (int argc, char const *argv[])	{
 
 FILE *f1 = NULL;
 FILE *f2 = NULL;
 FILE *f3 = NULL;

 char *fname = NULL;
 char *s = NULL;
 char line[250];
	char lt[]="\r\n";
 char file1[] = "qp_iso.txt";
 char file2[] = "qp_utf8.txt";
 char file3[] = "c0001.txt";


 printf("Bla\n");
 char qp_encoded_string_iso[] = "M=FCllermilch\r\n";
 char qp_encoded_string_utf8[] = "M=C3=BCllermilch\r\n";


 /* first we check the encoding from an ISO-8859-1 input file */
 asprintf(&fname,"%s/%s",SAMPLES_DIR,file1);

 f1 = fopen(fname, "r");
 
 while(fgets(line, 250, f1))
 {
		char *clean;
		clean = cmime_string_chomp(line);
		s = cmime_qp_encode(clean, lt);
		printf("clan: [%s]\n", clean);
		assert(strcmp(clean,qp_encoded_string_iso)==0);
 }
 fclose(f1);
 free(fname);
	free(s);

 /* then we check the enconding from an UTF8 input file */
 /*
 asprintf(&fname,"%s/%s",SAMPLES_DIR,file2);
 f2 = fopen(fname, "r");
 while(fgets(line, 250, f2))
 {
 	char *clean = NULL;
 	clean = cmime_string_chomp(line);
 	printf("[%s]\n", line);
		s = cmime_qp_encode(clean, lt);	
		printf("[%s]", clean);
		assert(strcmp(clean,qp_encoded_string_utf8)==0);
		free(s);
		free(clean);
 }
 fclose(f2);
 */


 /*
 asprintf(&fname,"%s/%s",SAMPLES_DIR,file3);

 f3 = fopen(fname, "r");
 while(fgets(line, 1000, f3))
 {
 	

		s = cmime_qp_encode(line, lt);
		//printf("[%s]", qp_encoded_string_utf8);
		printf("[%s]", s);
		//assert(strcmp(s,qp_encoded_string_utf8)==0);
		free(s);
 }
 fclose(f3);

        for (i = 0; i < strlen(inputline); i++)
        {
            if ( inputline[i] == '\n' || inputline[i] == '\r' )
                inputline[i] = '\0';
        }

 	*/

 //free(fname);
	return(0);
}
