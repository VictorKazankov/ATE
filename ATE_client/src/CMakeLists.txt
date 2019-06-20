cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(GNUInstallDirs)

project(vhat_client)

add_library(vhat_client
  MODULE
    ate_interaction.cpp
    vhat_client.cpp
    squish/application_context.cpp
    squish/squish_API.cpp
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
  vhat_common
  pybind11::module
)

target_compile_definitions(vhat_client
  PUBLIC
    VHAT_CLIENT_CONFIG_DIR="${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}"
)
set_target_properties(vhat_client PROPERTIES
  INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
)
install(FILES
  ../data/vhat_client.ini
  DESTINATION "${CMAKE_INSTALL_SYSCONFDIR}"
)

if(UNIX)
  message(WARNING "Python 2.7 and debian-style install path were hardcoded (https://wiki.debian.org/Python#Deviations_from_upstream)"
          " as py 2.7 on Ubuntu 16/18 is rigid required target platform.")

  install(TARGETS
    vhat_client
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}/python2.7/dist-packages"
  )
elseif(MSVC)
  install(TARGETS
    vhat_client
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
  )
endif()
