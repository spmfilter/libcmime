add_executable(test_base64 test_base64.c)
target_link_libraries(test_base64 cmime)
ADD_TEST(Base64Test ${EXECUTABLE_OUTPUT_PATH}/test_base64)

add_executable(test_address test_address.c)
target_link_libraries(test_address cmime)
ADD_TEST(AddressTest ${EXECUTABLE_OUTPUT_PATH}/test_address)

add_executable(test_list test_list.c)
target_link_libraries(test_list cmime)
ADD_TEST(ListTest ${EXECUTABLE_OUTPUT_PATH}/test_list)

add_executable(test_table test_table.c)
target_link_libraries(test_table cmime)
ADD_TEST(TableTest ${EXECUTABLE_OUTPUT_PATH}/test_table)

add_executable(test_string test_string.c)
target_link_libraries(test_string cmime)
ADD_TEST(StringTest ${EXECUTABLE_OUTPUT_PATH}/test_string)

add_executable(test_message test_message.c)
target_link_libraries(test_message cmime)
ADD_TEST(MessageTest ${EXECUTABLE_OUTPUT_PATH}/test_message)

add_executable(test_header test_header.c)
target_link_libraries(test_header cmime)
ADD_TEST(HeaderTest ${EXECUTABLE_OUTPUT_PATH}/test_header)

add_executable(test_part test_part.c)
target_link_libraries(test_part cmime)
ADD_TEST(PartTest ${EXECUTABLE_OUTPUT_PATH}/test_part)