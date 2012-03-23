#BISON_TARGET(cmime_parser cmime_parser.y ${CMAKE_CURRENT_BINARY_DIR}/cmime_parser.tab.c)
#FLEX_TARGET(cmime_scanner cmime_scanner.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)

BISON_TARGET(cmime_parser cmime_parser.y ${CMAKE_CURRENT_SOURCE_DIR}/cmime_parser.tab.c)
FLEX_TARGET(cmime_scanner cmime_scanner.l ${CMAKE_CURRENT_SOURCE_DIR}/lex.yy.c)

ADD_FLEX_BISON_DEPENDENCY(cmime_scanner cmime_parser)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)

set(LIBCMIME_SRC 
	cmime_address.c
	cmime_flbi.c
	cmime_list.c
	cmime_message.c
	cmime_header.c
	cmime_string.c
	cmime_part.c
	cmime_base64.c
	cmime_qp.c
	cmime_util.c
	cmime_internal.c
	${BISON_cmime_parser_OUTPUTS}
	${FLEX_cmime_scanner_OUTPUTS}
)

add_library(cmime SHARED ${LIBCMIME_SRC})
target_link_libraries(cmime ${FLEX_LIBRARIES})
set_property(TARGET cmime PROPERTY PUBLIC_HEADER cmime.h cmime_config.h)
set_property(TARGET cmime PROPERTY PRIVATE_HEADER
        cmime_address.h
        cmime_base64.h
        cmime_header.h
        cmime_list_.h
        cmime_message.h
        cmime_part.h
        cmime_qp.h
        cmime_string.h
        cmime_util.h
)

set_property(TARGET cmime PROPERTY VERSION ${CMIME_VERSION})
set_property(TARGET cmime PROPERTY SOVERSION ${CMIME_VERSION})

install(TARGETS cmime
	LIBRARY DESTINATION lib
	PUBLIC_HEADER DESTINATION include
	PRIVATE_HEADER DESTINATION include/cmime
)
