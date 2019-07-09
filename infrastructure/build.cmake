cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

# Examples for usage:
# cmake -P build.cmake
# cmake -DVHAT_BUILD_CLIENT:BOOL=OFF -P build.cmake
# cmake -DVHAT_BUILD_CLIENT:BOOL=OFF -DVHAT_WITH_TESTS:BOOL=OFF -P build.cmake

# Supported options:
# VHAT_BUILD_ROOT:PATH
# VHAT_BUILD_CLIENT:BOOL
# VHAT_BUILD_SERVER:BOOL
# VHAT_INSTALL:BOOL
# VHAT_INSTALL_SERVER_SERVICE:BOOL
# VHAT_CREATE_DEBIAN_PACKAGE:BOOL
# VHAT_WITH_TESTS:BOOL
# CMAKE_BUILD_TYPE:STRING=[Debug|Release|RelWithDebInfo|MinSizeRel|…]
# CMAKE_TOOLCHAIN_FILE:FILEPATH

if(UNIX)
  option(VHAT_BUILD_CLIENT "Build VHAT client" ON)
  option(VHAT_BUILD_SERVER "Build VHAT server" ON)
  option(VHAT_WITH_TESTS "Build VHAT Unit Tests" ON)
  option(VHAT_INSTALL "Install VHAT" ON)
  option(VHAT_INSTALL_SERVER_SERVICE "Install VHAT server service" OFF)
  option(VHAT_CREATE_DEBIAN_PACKAGE "Create Debian package" ON)
elseif(WIN32)
  option(VHAT_BUILD_CLIENT "Build VHAT client" ON)
  option(VHAT_BUILD_SERVER "Build VHAT server" OFF) 
  option(VHAT_WITH_TESTS "Build VHAT Unit Tests" ON)
  option(VHAT_INSTALL "Install VHAT" ON)
  option(VHAT_INSTALL_SERVER_SERVICE "Install VHAT server service" OFF)
  option(VHAT_CREATE_DEBIAN_PACKAGE "Create Debian package" OFF)
else()
  message(FATAL_ERROR "build.cmake only support UNIX and WIN32 build")
  RESULT_VARIABLE ("Platform is not supported")
endif()

get_filename_component(VHAT_SOURCE_ROOT "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type: [Debug|Release|RelWithDebInfo|MinSizeRel|…]")

if(UNIX)
  set(CMAKE_TOOLCHAIN_FILE "${VHAT_SOURCE_ROOT}/infrastructure/cmake/toolchains/desktop.cmake" CACHE FILEPATH "CMake toolchain")
elseif(WIN32)
  set(CMAKE_TOOLCHAIN_FILE "${VHAT_SOURCE_ROOT}/infrastructure/cmake/toolchains/desktop_win32.cmake" CACHE FILEPATH "Cmake toolchain for Win32")
endif()

get_filename_component(VHAT_SOURCE_ROOT_DIR_NAME "${VHAT_SOURCE_ROOT}" NAME)
get_filename_component(VHAT_BUILD_ROOT "${VHAT_SOURCE_ROOT}/../build-${VHAT_SOURCE_ROOT_DIR_NAME}-${CMAKE_BUILD_TYPE}" ABSOLUTE CACHE)

set(CMAKE_INSTALL_PREFIX "${VHAT_BUILD_ROOT}/install" CACHE PATH "Install prefix")

message(STATUS "Install VHAT: " ${VHAT_INSTALL})

if (EXISTS "${VHAT_BUILD_ROOT}")
  message(STATUS ${VHAT_BUILD_ROOT} " already exists. Updating old build")
else()
  message(STATUS "Creating VHAT build root directory: " ${VHAT_BUILD_ROOT})
  file(MAKE_DIRECTORY "${VHAT_BUILD_ROOT}")
endif()

execute_process(
  COMMAND
    "${CMAKE_COMMAND}" -E chdir "${VHAT_BUILD_ROOT}"
    "${CMAKE_COMMAND}" "${VHAT_SOURCE_ROOT}"
      -GNinja
      -DCMAKE_BUILD_TYPE:STRING='${CMAKE_BUILD_TYPE}'
      -DCMAKE_INSTALL_PREFIX:PATH='${CMAKE_INSTALL_PREFIX}'
      -DCMAKE_PREFIX_PATH:PATH=/home/nvidia/gstreamer114_build/
      -DCMAKE_TOOLCHAIN_FILE:FILEPATH='${CMAKE_TOOLCHAIN_FILE}'
      -DVHAT_BUILD_CLIENT:BOOL=${VHAT_BUILD_CLIENT}
      -DVHAT_BUILD_SERVER:BOOL=${VHAT_BUILD_SERVER}
      -DVHAT_WITH_TESTS:BOOL=${VHAT_WITH_TESTS}
      -DVHAT_INSTALL_SERVER_SERVICE:BOOL=${VHAT_INSTALL_SERVER_SERVICE}
      -DVHAT_CREATE_DEBIAN_PACKAGE:BOOL=${VHAT_CREATE_DEBIAN_PACKAGE}

  RESULT_VARIABLE GENERATING_RETURN
)

if(NOT 0 EQUAL ${GENERATING_RETURN})
  message(FATAL_ERROR "build.cmake generating terminated with failure. Return code: " ${GENERATING_RETURN})
endif()

execute_process(
  COMMAND
    "${CMAKE_COMMAND}" -E chdir "${VHAT_BUILD_ROOT}"
    "${CMAKE_COMMAND}" --build .

  RESULT_VARIABLE BUILDING_RETURN
)

if(NOT 0 EQUAL ${BUILDING_RETURN})
  message(FATAL_ERROR "VHAT building terminated with failure. Return code: " ${BUILDING_RETURN})
endif()

if(VHAT_INSTALL)
  execute_process(
    COMMAND
      "${CMAKE_COMMAND}" -E chdir "${VHAT_BUILD_ROOT}"
      "${CMAKE_COMMAND}" --build . --target install

    RESULT_VARIABLE INSTALLING_RETURN
  )

  if(NOT 0 EQUAL ${INSTALLING_RETURN})
    message(FATAL_ERROR "VHAT installing terminated with failure. Return code: " ${INSTALLING_RETURN})
  endif()
endif(VHAT_INSTALL)

if(VHAT_CREATE_DEBIAN_PACKAGE)
  execute_process(
    COMMAND
      "${CMAKE_COMMAND}" -E chdir "${VHAT_BUILD_ROOT}"
      cpack

    RESULT_VARIABLE PACK_RETURN
  )

  if(NOT 0 EQUAL ${PACK_RETURN})
    message(FATAL_ERROR "VHAT pack terminated with failure. Return code: " ${PACK_RETURN})
  endif()
endif(VHAT_CREATE_DEBIAN_PACKAGE)
