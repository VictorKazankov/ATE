cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(TEST_NAME vhat_client_test)

add_executable_from_dir_recursive(${TEST_NAME})  

target_include_directories(${TEST_NAME}
  PRIVATE
    ../include
)

if(UNIX)
  target_link_libraries(${TEST_NAME}
    PRIVATE 
      ate_client_lib
      ${GTEST_BOTH_LIBRARIES}
      ${GMOCK_BOTH_LIBRARIES}
  )
elseif(MSVC)
  target_link_libraries(vhat_client_test
    PRIVATE
      ate_client_lib
      gtest_main
  )
  set_target_properties(gtest
                        gtest_main
                        vhat_client_library
                        vhat_client_test
                        PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                        "${CMAKE_BINARY_DIR}/test")
endif()

gtest_add_tests(${TEST_NAME} "" AUTO)
