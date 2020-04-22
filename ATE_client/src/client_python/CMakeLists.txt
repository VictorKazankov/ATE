cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

pybind11_add_module(client_python ate_client_python.cpp)

target_include_directories(client_python PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

target_link_libraries(client_python PRIVATE ate_client_lib)

set_target_properties(client_python PROPERTIES OUTPUT_NAME "vhat_client")

if(UNIX)
  set_target_properties(client_python PROPERTIES
    INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${ATE_INSTALL_LIBDIR}"
  )
endif()

if(UNIX)
  message("INFO: ATE client install path was hardcoded for Python 2.7 and Debian system "
          "(https://wiki.debian.org/Python#Deviations_from_upstream)")

  install(TARGETS
    client_python
    LIBRARY DESTINATION "${ATE_INSTALL_LIBPREFIX}/python2.7/dist-packages"
  )
elseif(WIN32)
  install(TARGETS
    client_python
    LIBRARY DESTINATION "${ATE_PYTHON_SITEPACKAGES_DIR}"
  )
endif()
