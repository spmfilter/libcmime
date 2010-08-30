set(LIBCMIME_SRC 
	cmime_address.c
)

add_library(cmime SHARED ${LIBCMIME_SRC})
set_property(TARGET cmime PROPERTY PUBLIC_HEADER cmime.h)
set_property(TARGET cmime PROPERTY VERSION ${CMIME_VERSION})
set_property(TARGET cmime PROPERTY SOVERSION ${CMIME_VERSION})

install(TARGETS cmime
	LIBRARY DESTINATION lib
	PUBLIC_HEADER DESTINATION include
)