cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# Examples for usage:
# cmake -P build.cmake
# cmake -DATE_BUILD_CLIENT:BOOL=OFF -P build.cmake
# cmake -DATE_BUILD_CLIENT:BOOL=OFF -DATE_WITH_TESTS:BOOL=OFF -P build.cmake

# Supported options:
# ATE_BUILD_ROOT:PATH
# ATE_BUILD_CLIENT:BOOL
# ATE_BUILD_SERVER:BOOL
# ATE_INSTALL:BOOL
# ATE_INSTALL_SERVER_SERVICE:BOOL
# VHAT_INSTALL_ICON_STORAGE:BOOL
# ATE_CREATE_DEBIAN_PACKAGE:BOOL
# ATE_WITH_TESTS:BOOL
# CMAKE_BUILD_TYPE:STRING=[Debug|Release|RelWithDebInfo|MinSizeRel|…]
# CMAKE_TOOLCHAIN_FILE:FILEPATH

if(UNIX)
  option(ATE_BUILD_CLIENT "Build ATE client" ON)
  option(ATE_BUILD_SERVER "Build ATE server" ON)
  option(ATE_WITH_TESTS "Build ATE Unit Tests" ON)
  option(ATE_INSTALL "Install ATE" ON)
  option(ATE_INSTALL_SERVER_SERVICE "Install ATE server service" OFF)
  option(VHAT_INSTALL_ICON_STORAGE "Install ATE icon storage" OFF)
  option(ATE_CREATE_DEBIAN_PACKAGE "Create Debian package" OFF)
elseif(WIN32)
  option(ATE_BUILD_CLIENT "Build ATE client" ON)
  option(ATE_BUILD_SERVER "Build ATE server" OFF) 
  option(ATE_WITH_TESTS "Build ATE Unit Tests" ON)
  option(ATE_INSTALL "Install ATE" ON)
  option(ATE_INSTALL_SERVER_SERVICE "Install ATE server service" OFF)
  option(VHAT_INSTALL_ICON_STORAGE "Install VHAT icon storage" OFF)
  option(ATE_CREATE_DEBIAN_PACKAGE "Create Debian package" OFF)
else()
  message(FATAL_ERROR "build.cmake only support UNIX and WIN32 build")
  RESULT_VARIABLE ("Platform is not supported")
endif()

get_filename_component(ATE_SOURCE_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type: [Debug|Release|RelWithDebInfo|MinSizeRel|…]")

if(UNIX)
  set(CMAKE_TOOLCHAIN_FILE "${ATE_SOURCE_ROOT}/infrastructure/cmake/toolchains/desktop.cmake" CACHE FILEPATH "CMake toolchain")
  set(CMAKE_GENERATOR_DEFAULT "Ninja")
elseif(WIN32)
  set(CMAKE_TOOLCHAIN_FILE "${ATE_SOURCE_ROOT}/infrastructure/cmake/toolchains/desktop_win32.cmake" CACHE FILEPATH "Cmake toolchain for Win32")
  set(CMAKE_GENERATOR_DEFAULT "NMake Makefiles")
endif()

get_filename_component(ATE_SOURCE_ROOT_DIR_NAME "${ATE_SOURCE_ROOT}" NAME)
get_filename_component(ATE_BUILD_ROOT "${ATE_SOURCE_ROOT}/../build-${ATE_SOURCE_ROOT_DIR_NAME}-${CMAKE_BUILD_TYPE}" ABSOLUTE CACHE)

set(CMAKE_INSTALL_PREFIX "${ATE_BUILD_ROOT}/install" CACHE PATH "Install prefix")

message(STATUS "Install ATE: " ${ATE_INSTALL})

if (EXISTS "${ATE_BUILD_ROOT}")
  message(STATUS ${ATE_BUILD_ROOT} " already exists. Updating old build")
else()
  message(STATUS "Creating ATE build root directory: " ${ATE_BUILD_ROOT})
  file(MAKE_DIRECTORY "${ATE_BUILD_ROOT}")
endif()

message(STATUS "CMAKE_GENERATOR: " ${CMAKE_GENERATOR})

execute_process(
  COMMAND
    "${CMAKE_COMMAND}" -E chdir "${ATE_BUILD_ROOT}"
    "${CMAKE_COMMAND}" "${ATE_SOURCE_ROOT}"
      -G${CMAKE_GENERATOR_DEFAULT}
      -DCMAKE_BUILD_TYPE:STRING='${CMAKE_BUILD_TYPE}'
      -DCMAKE_INSTALL_PREFIX:PATH='${CMAKE_INSTALL_PREFIX}'
      -DCMAKE_PREFIX_PATH:PATH='${CMAKE_PREFIX_PATH}'
      -DCMAKE_TOOLCHAIN_FILE:FILEPATH='${CMAKE_TOOLCHAIN_FILE}'
      -DATE_BUILD_CLIENT:BOOL=${ATE_BUILD_CLIENT}
      -DATE_BUILD_SERVER:BOOL=${ATE_BUILD_SERVER}
      -DATE_WITH_TESTS:BOOL=${ATE_WITH_TESTS}
      -DATE_INSTALL_SERVER_SERVICE:BOOL=${ATE_INSTALL_SERVER_SERVICE}
      -DVHAT_INSTALL_ICON_STORAGE:BOOL=${VHAT_INSTALL_ICON_STORAGE}
      -DATE_CREATE_DEBIAN_PACKAGE:BOOL=${ATE_CREATE_DEBIAN_PACKAGE}

  RESULT_VARIABLE GENERATING_RETURN
)

if(NOT 0 EQUAL ${GENERATING_RETURN})
  message(FATAL_ERROR "build.cmake generating terminated with failure. Return code: " ${GENERATING_RETURN})
endif()

execute_process(
  COMMAND
    "${CMAKE_COMMAND}" -E chdir "${ATE_BUILD_ROOT}"
    "${CMAKE_COMMAND}" --build .

  RESULT_VARIABLE BUILDING_RETURN
)

if(NOT 0 EQUAL ${BUILDING_RETURN})
  message(FATAL_ERROR "ATE building terminated with failure. Return code: " ${BUILDING_RETURN})
endif()

if(ATE_INSTALL)
  execute_process(
    COMMAND
      "${CMAKE_COMMAND}" -E chdir "${ATE_BUILD_ROOT}"
      "${CMAKE_COMMAND}" --build . --target install

    RESULT_VARIABLE INSTALLING_RETURN
  )

  if(NOT 0 EQUAL ${INSTALLING_RETURN})
    message(FATAL_ERROR "ATE installing terminated with failure. Return code: " ${INSTALLING_RETURN})
  endif()
endif(ATE_INSTALL)

if(ATE_CREATE_DEBIAN_PACKAGE)
  execute_process(
    COMMAND
      "${CMAKE_COMMAND}" -E chdir "${ATE_BUILD_ROOT}"
      cpack

    RESULT_VARIABLE PACK_RETURN
  )

  if(NOT 0 EQUAL ${PACK_RETURN})
    message(FATAL_ERROR "ATE pack terminated with failure. Return code: " ${PACK_RETURN})
  endif()
endif(ATE_CREATE_DEBIAN_PACKAGE)
