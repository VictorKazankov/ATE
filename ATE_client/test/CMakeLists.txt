cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_executable_from_dir_recursive(ate_client_test)

target_include_directories(ate_client_test
  PRIVATE
    ../include
)

target_link_libraries(ate_client_test
  PRIVATE
    ate_client_lib
    ${GTEST_BOTH_LIBRARIES}
    ${GMOCK_BOTH_LIBRARIES}
)

gtest_add_tests(ate_client_test "" AUTO)
