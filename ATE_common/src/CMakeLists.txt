cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(GNUInstallDirs)

add_library(vhat_common
  SHARED
    config/config.cpp
    logger/logger_setup.cpp
    message_factory/json_utils.cpp
    message_factory/message_factory.cpp
    utils/geometry_types.cpp
    common.cpp
)

# JsonCpp
# http://open-source-parsers.github.io/jsoncpp-docs/doxygen/index.html
# Debian/Ubuntu package name: libjsoncpp-dev
find_package(jsoncpp REQUIRED)

target_include_directories(vhat_common
  PUBLIC
    ../include
)

target_link_libraries(vhat_common
  Boost::asio
  inihcpp
  jsoncpp_lib
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
endif(UNIX)

install(TARGETS
  vhat_common
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
