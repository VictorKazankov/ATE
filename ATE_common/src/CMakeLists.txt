cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(GNUInstallDirs)

# JsonCpp
# http://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html
# Debian/Ubuntu package name: libjsoncpp-dev
find_package(jsoncpp REQUIRED)

add_library(vhat_common
  SHARED
    config/config.cpp
    logger/logger_setup.cpp
    message_factory/json_utils.cpp
    message_factory/message_factory.cpp
    utils/geometry_types.cpp
    common.cpp
)

configure_file(../include/version/version_defines.h.in ../include/version/version_defines.h)

target_include_directories(vhat_common
  PUBLIC
    ../include
    "${CMAKE_CURRENT_BINARY_DIR}/../include"
)

target_link_libraries(vhat_common
  PUBLIC
    Boost::asio
    inihcpp
    jsoncpp_lib_static
    spdlog::spdlog
)

if(UNIX)
  target_compile_options(vhat_common
    PUBLIC
      -pedantic
      -Wall
      -Werror
      -Wextra
  )
elseif(MSVC)
  target_compile_options(vhat_common
    PUBLIC
      -DNOMINMAX
  )
  set_target_properties(vhat_common 
    PROPERTIES
      WINDOWS_EXPORT_ALL_SYMBOLS TRUE
  )
endif()

if(UNIX)
  install(TARGETS
    vhat_common
    LIBRARY DESTINATION ${VHAT_INSTALL_LIBDIR}
  )
elseif(MSVC)
  install(TARGETS
    vhat_common
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
endif()

#######################################################
#                   Test coverage
#######################################################
if (VHAT_COVERAGE)
  target_compile_options(vhat_common PRIVATE --coverage)
  target_link_libraries(vhat_common PRIVATE gcov)
endif (VHAT_COVERAGE)
