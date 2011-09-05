set(LIBCMIME_SRC 
	cmime_address.c
	cmime_list.c
	cmime_message.c
	cmime_header.c
	cmime_string.c
	cmime_part.c
	cmime_base64.c
	cmime_util.c
	cmime_internal.c
)

add_library(cmime SHARED ${LIBCMIME_SRC})
set_property(TARGET cmime PROPERTY PUBLIC_HEADER cmime.h)
set_property(TARGET cmime PROPERTY VERSION ${CMIME_VERSION})
set_property(TARGET cmime PROPERTY SOVERSION ${CMIME_VERSION})

install(TARGETS cmime
	LIBRARY DESTINATION lib
	PUBLIC_HEADER DESTINATION include
)