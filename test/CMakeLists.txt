add_executable(test_address test_address.c)
target_link_libraries(test_address cmime)

ADD_TEST(AddressTest ${EXECUTABLE_OUTPUT_PATH}/test_address)

add_executable(test_list test_list.c)
target_link_libraries(test_list cmime)

ADD_TEST(ListTest ${EXECUTABLE_OUTPUT_PATH}/test_list)

add_executable(test_table test_table.c)
target_link_libraries(test_table cmime)

ADD_TEST(TableTest ${EXECUTABLE_OUTPUT_PATH}/test_table)

add_executable(test_message test_message.c)
target_link_libraries(test_message cmime)

ADD_TEST(MessageTest ${EXECUTABLE_OUTPUT_PATH}/test_message)