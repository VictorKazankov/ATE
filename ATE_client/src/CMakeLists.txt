cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(GNUInstallDirs)

project(vhat_client)

add_library(vhat_client
  MODULE
    ate_interaction.cpp
    ate_client.cpp
    squish/application_context.cpp
    squish/squish_API.cpp
    squish/squish_types.cpp
)

set_target_properties(vhat_client PROPERTIES
  PREFIX "${PYTHON_MODULE_PREFIX}"
  SUFFIX "${PYTHON_MODULE_EXTENSION}"
)

target_include_directories(vhat_client
  PUBLIC
    ../include
)

target_link_libraries(vhat_client
  ate_common
  pybind11::module
)

target_compile_definitions(vhat_client
  PUBLIC
    ATE_CLIENT_CONFIG_DIR="${CMAKE_INSTALL_PREFIX}/${ATE_INSTALL_SYSCONFDIR}"
)

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
