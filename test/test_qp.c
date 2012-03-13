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
//#include "test_data.h"

int main (int argc, char const *argv[])	{


	char *s = NULL;
	char quoted_printable_string1_plain[] = "Müllermilch";
 char quoted_printable_string1_encoded[] = "M=C3=BCllermilch";

 /*
	s = cmime_qp_encode(quoted_printable_string1_plain, NULL);
 assert(strcmp(s,quoted_printable_string1_encoded) == 0);
 free(s);

 char quoted_printable_string2_plain[] = "Hätten Hüte ein ß im Namen, wären sie möglicherweise keine Hüte mehr, sondern Hüße.";
 char quoted_printable_string2_encoded[] = "H=C3=A4tten H=C3=BCte ein =C3=9F im Namen, w=C3=A4ren sie m=C3=B6glicherweise keine H=C3=BCte mehr, sondern H=C3=BC=C3=9Fe.";

	s = cmime_qp_encode(quoted_printable_string2_plain, NULL);
	printf("%s", s);
	assert(strcmp(s,quoted_printable_string2_encoded)==0);
	free(s);
*/


	char quoted_printable_string3_plain[] = "Hätten Hüte ein ß im Namen, wären sie möglicherweise keine Hüte mehr, sondern Hüße.Hätten Hüte ein ß im Namen, wären sie möglicherweise keine Hüte mehr, sondern Hüße.Hätten Hütesondern Hüße.Hätten Hüte ein ß im Namen, wären sie möglicherweise keine Hüte mehr, sondern Hüße.Hätten Hüte";


 s = cmime_qp_encode(quoted_printable_string3_plain, NULL);
 printf("%s", s);

 FILE *fp = NULL;

 if((fp = fopen("/tmp/TEST", "w"))==NULL) {
    printf("Cannot open file.\n");
    exit(1);
 }

 fputs(s, fp);
 
free(s);



	return(0);
}
