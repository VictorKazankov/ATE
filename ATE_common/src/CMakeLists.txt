cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# JsonCpp
# http://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html
# Debian/Ubuntu package name: libjsoncpp-dev
find_package(jsoncpp REQUIRED)

conditional_set(LIBRARY_TYPE STATIC SHARED WIN32)
add_library(ate_common
  ${LIBRARY_TYPE}
    config/config.cpp
    logger/logger_setup.cpp
    message_factory/json_utils.cpp
    message_factory/message_factory.cpp
    utils/geometry_types.cpp
    utils/squish_types_converter.cpp
    ate_error.cpp
    common.cpp
)

configure_file(../include/version/version_defines.h.in ../include/version/version_defines.h)

target_include_directories(ate_common
  PUBLIC
    ../include
    "${CMAKE_CURRENT_BINARY_DIR}/../include"
)

target_link_libraries(ate_common
  PUBLIC
    Boost::asio
    inihcpp
    jsoncpp_lib_static
    spdlog::spdlog
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options(ate_common
    PUBLIC
      -pedantic
      -Wall
      -Werror
      -Wextra
  )
elseif(MSVC)
  target_compile_options(ate_common
    PUBLIC
      /W4
      /WX
  )
endif()

if(UNIX)
  install(TARGETS
    ate_common
    LIBRARY DESTINATION ${ATE_INSTALL_LIBDIR}
  )
endif()

#######################################################
#                   Test coverage
#######################################################
if (ATE_COVERAGE)
  target_compile_options(ate_common PUBLIC --coverage)
  target_link_libraries(ate_common PUBLIC gcov)
endif (ATE_COVERAGE)
