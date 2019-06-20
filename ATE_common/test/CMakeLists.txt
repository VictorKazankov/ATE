cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(VHAT_COMMON_TEST_DATA_PATH "${CMAKE_CURRENT_SOURCE_DIR}/data")

add_executable(vhat_common_test
  common/common.cpp
  config/config.cpp
  logger/logger.cpp
  message_factory/request.cpp
  utils/geometry_types.cpp
)

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

target_compile_definitions(vhat_common_test
  PRIVATE
    VHAT_COMMON_TEST_DATA_PATH="${VHAT_COMMON_TEST_DATA_PATH}"
)

# Add vhat_server_test to the CTest
# TODO: replace add_test with gtest_discover_tests after upgrade minimum CMake version to 3.10
# https://cmake.org/cmake/help/latest/module/GoogleTest.html
add_test(
  NAME VHAT_Common_Tests
  COMMAND vhat_common_test
)
