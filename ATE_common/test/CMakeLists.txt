cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(VHAT_COMMON_TEST_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data")
set(TEST_NAME vhat_common_test)

add_executable_from_dir_recursive(${TEST_NAME})

if(UNIX)
  target_link_libraries(vhat_common_test
    PRIVATE
      gtest_main
      stdc++fs
      vhat_common
      )
elseif(MSVC)
  #check msvc version (later MSVC v141) which required to support file system library
  #https://docs.microsoft.com/en-us/cpp/standard-library/filesystem?view=vs-2019
  if(MSVC_VERSION LESS 1910) #VS 15.0 (v141 toolset)
    message(FATAL_ERROR "Unit test using file system library but not supported by current MSVC version: ${MSVC_VERSION}")
    RESULT_VARIABLE ("Unit test is uncompilable because compiler not support std::experimental::filesystem")
  else()
    target_link_libraries(vhat_common_test
      PRIVATE
      gtest_main
      vhat_common
    )
    set_target_properties( gtest 
                           gtest_main
                           vhat_common
                           vhat_common_test 
                           PROPERTIES RUNTIME_OUTPUT_DIRECTORY 
                           "${CMAKE_BINARY_DIR}/test" )

  endif()
endif()

target_compile_definitions(${TEST_NAME}
  PRIVATE
    VHAT_COMMON_TEST_DATA_PATH="${VHAT_COMMON_TEST_DATA_PATH}"
)

gtest_add_tests(${TEST_NAME} "" AUTO)
