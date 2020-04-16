cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

add_executable_from_dir_recursive(ate_common_test)

target_compile_definitions(ate_common_test
  PRIVATE
    ATE_COMMON_TEST_DATA_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data"
)

target_link_libraries(ate_common_test
  PRIVATE
    ${GTEST_BOTH_LIBRARIES}
    ate_common
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(ate_common_test
    PRIVATE
      stdc++fs
  )
elseif(MSVC)
  #check msvc version (later MSVC v141) which required to support file system library
  #https://docs.microsoft.com/en-us/cpp/standard-library/filesystem?view=vs-2019
  if(MSVC_VERSION LESS 1910) #VS 15.0 (v141 toolset)
    message(FATAL_ERROR "Unit test using file system library but not supported by current MSVC version: ${MSVC_VERSION}")
    RESULT_VARIABLE ("Unit test is uncompilable because compiler not support std::experimental::filesystem")
  endif()

  target_compile_definitions(ate_common_test
    PRIVATE
      _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
  )
endif()

gtest_add_tests(ate_common_test "" AUTO)
