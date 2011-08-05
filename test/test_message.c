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

#include "../src/cmime_address.h"
#include "../src/cmime_message.h"
#include "../src/cmime_header.h"
#include "../src/cmime_list.h"
#include "../src/cmime_part.h"

#include "test_data.h"

int main (int argc, char const *argv[]) {
	CMimeMessage_T *msg = cmime_message_new();
	char *s = NULL;
	char *s2 = NULL;
	CMimeHeader_T *h = NULL;
	CMimeList_T *recipient_list = NULL;
	CMimeListElem_T *elem;
	int retval;
	CMimePart_T *part;
	
	cmime_message_set_sender(msg,addr_string1);
	s = cmime_message_get_sender(msg);
	assert(strcmp(s,addr_string1)==0);
	free(s);
	
	// check if sender will be overwritten
	cmime_message_set_sender(msg,addr_string2);
	s = cmime_message_get_sender(msg);
	assert(strcmp(s,addr_string2)==0);
	free(s);
	
	// set message id
	cmime_message_set_message_id(msg,message_id_string);
	s = cmime_message_get_message_id(msg);
	assert(strcmp(s,message_id_string)==0);
	
	// set a test header
	if (cmime_message_set_header(msg, header_string1)!=0) 
		return(-1);
	h = cmime_message_get_header(msg,header_string1_key);
	assert(strcmp(cmime_header_get_value(h,0),header_string1_value)==0);
	
	// overwrite header
	if (cmime_message_set_header(msg, header_string1_2)!=0)
		return(-1);
		
	h = cmime_message_get_header(msg,header_string1_key);
	assert(strcmp(cmime_header_get_value(h,0),header_string2_value)==0);

	// check with lower case key
	h = cmime_message_get_header(msg,header_string1_key_lower);
	assert(strcmp(cmime_header_get_value(h,0),header_string2_value)==0);

	// check recipients
	if (cmime_message_add_recipient(msg,addr_string1,CMIME_ADDRESS_TYPE_TO)!=0)
		return(-1);
	
	if (cmime_message_add_recipient(msg,addr_string2,CMIME_ADDRESS_TYPE_CC)!=0)
		return(-1);
		
	recipient_list = cmime_message_get_recipients(msg);
	assert(recipient_list);

	elem = cmime_list_head(recipient_list);
	while(elem != NULL) {
		s = cmime_address_to_string((CMimeAddress_T *)cmime_list_data(elem));
		assert(s);
		printf("got recipient: '%s'\n",s);
		free(s);
		elem = elem->next;
	}
	
	// checking date
	if (cmime_message_set_date_now(msg)!=0)
		return(-1);
		
	s = cmime_message_get_date(msg);
	assert(s);
			
	// check boundary stuff
	s = cmime_message_generate_boundary();
	cmime_message_set_boundary(msg,s);
	s2 = cmime_message_get_boundary(msg);
	assert(strcmp(s,s2)==0);
	
	free(s);
	cmime_message_free(msg);

	msg = cmime_message_new();
	retval = cmime_message_from_file(&msg,"/Users/ast/Desktop/Eicar.eml");
	elem = cmime_list_tail(msg->parts);
	part = (CMimePart_T *)cmime_list_data(elem);
	s = cmime_part_to_string(part);
//	printf("Part:\n%s\n", s);
	free(s);
//	printf("SIZE: %d\n", cmime_list_size(msg->parts));
//	printf("ENCODING: %s\n", cmime_part_get_content_transfer_encoding(part)); 
//	printf("TYPE: [%s]\n", cmime_part_get_content_type(part));
	
	s = cmime_message_to_string(msg);
	printf("%s\n", s);
	cmime_message_free(msg);
	
	return(0);
}