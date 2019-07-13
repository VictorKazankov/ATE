include(CheckCXXCompilerFlag)

# cmake GTests (included only from 3.9.1)
include(${CMAKE_CURRENT_LIST_DIR}/GoogleTest.cmake)

# Add cxx compiler flag
# FLAG flag
function(add_cxx_compiler_flag FLAG)
  string(FIND "${CMAKE_CXX_FLAGS}" "${FLAG}" flag_already_set)
  if(flag_already_set EQUAL -1)
    check_cxx_compiler_flag("${FLAG}" flag_supported)
    if(flag_supported)
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAG}" PARENT_SCOPE)
    endif()
  endif()
endfunction()

# Add library from all folder recursive
# TARGET - executable name
function(add_executable_from_dir_recursive TARGET)
  # build from current dir
  file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
  add_executable(${TARGET} ${SOURCE_FILES})
endfunction()
