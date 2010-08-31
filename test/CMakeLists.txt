add_executable(test_address test_address.c)
target_link_libraries(test_address cmime)

ADD_TEST(AddressTest ${EXECUTABLE_OUTPUT_PATH}/test_address)