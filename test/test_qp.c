/* libcmime - A C mime library
 * Copyright (C) SpaceNet AG and Werner Detter <werner@aloah-from-hell.de>
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

void remove_output_file(char *filename) {
		remove(filename);
}

int main (int argc, char const *argv[])	{
 
 /* declarations */
 FILE *f1 = NULL;
 FILE *f1_out = NULL;
 FILE *f1_out_in = NULL;
 char file1[] = "qp_iso.txt";
 char file1_out[] = "qp_iso_out.txt";
 char qp_encoded_string_iso[] = "H=E4tten H=FCte ein =DF im Namen, w=E4ren sie m=F6glicherweise keine H=FCte mehr";

 FILE *f2 = NULL;
 FILE *f2_out = NULL;
 FILE *f2_out_in = NULL;
 char file2[] = "qp_utf8.txt";
 char file2_out[] = "qp_utf8_out.txt";
 char qp_encoded_string_utf8[] = "H=C3=A4tten H=C3=BCte ein =C3=9F im Namen, w=C3=A4ren sie m=C3=B6glicherweise keine H=C3=BCte mehr";

 char *fname = NULL;
 char *si = NULL; // string in 
 char *so = NULL; // string out
 char line[250];
 char lt[]="\r\n";

 /** 
  * first we check the encoding from an ISO-8859-1 input file 
  * in the first run, we read out the input file, convert the 
  * input string to QP and write the encoded string to output
  * file. Subsequently the string in the output file needs to
  * be verified, then the origin output-file is deleted 
  */
 asprintf(&fname,"%s/%s",SAMPLES_DIR,file1);
 f1 = fopen(fname, "r");
 while(fgets(line, 250, f1)) {
		char *clean;
		clean = cmime_string_chomp(line);
		si = cmime_qp_encode(clean, lt);
 }
 fclose(f1);
 free(fname);
	// write the encoded string to the temporary output file
	asprintf(&fname,"%s/%s",SAMPLES_DIR,file1_out);
	f1_out = fopen (fname, "w");
	if (f1_out != NULL) { 
		fprintf(f1_out,"%s",si);
		fclose(f1_out);
	}
	free(si);
	// read it out again
	f1_out_in = fopen(fname, "r");
	while(fgets(line, 250, f1_out_in))
 {
		so = cmime_string_chomp(line);
 }
 fclose(f1_out_in);
	remove_output_file(fname);
 free(fname);
 // compare the strings 
	assert(strcmp(so,qp_encoded_string_iso)==0);


	/** 
 * we now do the same but for an utf8 encoded input string
 */
 asprintf(&fname,"%s/%s",SAMPLES_DIR,file2);
 f2 = fopen(fname, "r");
 while(fgets(line, 250, f2)) {
		char *clean;
		clean = cmime_string_chomp(line);
		si = cmime_qp_encode(clean, lt);
 }
 fclose(f2);
 free(fname);
	// write the encoded string to the temporary output file
	asprintf(&fname,"%s/%s",SAMPLES_DIR,file2_out);
	f2_out = fopen (fname, "w");
	if (f2_out != NULL) { 
		fprintf(f2_out,"%s",si);
		fclose(f2_out);
	}
	free(si);
	// read it out again 
	f2_out_in = fopen(fname, "r");
	while(fgets(line, 250, f2_out_in))
 {
		so = cmime_string_chomp(line);
 }
 fclose(f2_out_in);
 remove_output_file(fname);
 free(fname);
 // compare the strings 
	assert(strcmp(so,qp_encoded_string_utf8)==0);

	return(0);
}
