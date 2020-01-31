cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(GNUInstallDirs)

project(vhat_client)

set(SOURCE_LIST
  api_aggregator.cpp 
  ate_interaction.cpp
  ate_client.cpp
  json_rpc_parser.cpp
  squish/application_context.cpp
  squish/squish_API.cpp
)

add_library(vhat_client
  MODULE
    ${SOURCE_LIST}
)

set_target_properties(vhat_client PROPERTIES
  PREFIX "${PYTHON_MODULE_PREFIX}"
  SUFFIX "${PYTHON_MODULE_EXTENSION}"
)

target_include_directories(vhat_client
  PUBLIC
    ../include
)

add_library(vhat_client_library STATIC
  ${SOURCE_LIST}
)

target_include_directories(vhat_client_library
  PUBLIC
    ../include
)

target_link_libraries(vhat_client_library
  PUBLIC
    pybind11::module
    ate_common
)

target_compile_definitions(vhat_client_library
  PUBLIC
    ATE_CLIENT_CONFIG_DIR="${ATE_INSTALL_SYSCONFDIR}"
)

target_link_libraries(vhat_client
  ate_common
  pybind11::module
)

target_compile_definitions(vhat_client
  PUBLIC
    ATE_CLIENT_CONFIG_DIR="${ATE_INSTALL_SYSCONFDIR}"
)
message(STATUS "ATE_CLIENT_CONFIG_DIR: " ${ATE_INSTALL_SYSCONFDIR})

set_target_properties(vhat_client PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${ATE_INSTALL_LIBDIR}"
)

install(FILES
  ../data/ate_client.ini
  DESTINATION ${ATE_INSTALL_SYSCONFDIR}
)

if(UNIX)
  message(WARNING "Python 2.7 and debian-style install path were hardcoded (https://wiki.debian.org/Python#Deviations_from_upstream)"
          " as py 2.7 on Ubuntu 16/18 is rigid required target platform.")

  install(TARGETS
    vhat_client
    LIBRARY DESTINATION "${ATE_INSTALL_LIBPREFIX}/python2.7/dist-packages"
  )
elseif(MSVC)
  install(TARGETS
    vhat_client
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  )
endif()

#######################################################
#                   Test coverage
#######################################################
if (ATE_COVERAGE)
  target_compile_options(vhat_client_library PRIVATE --coverage)
  target_link_libraries(vhat_client_library PRIVATE 
                        pybind11::module
                        ate_common
                        gcov)
endif(ATE_COVERAGE)
