set(ADDRESS_SRC 
	test_address.c
)

add_executable(test_address ${ADDRESS_SRC})
target_link_libraries(test_address cmime)

ADD_TEST(AddressTest ${EXECUTABLE_OUTPUT_PATH}/test_address)