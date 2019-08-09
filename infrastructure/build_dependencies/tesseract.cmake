cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

set(LEPTONICA_ROOT_DIR /usr/local/ CACHE PATH "Path with Leptonica library files")
message(STATUS "Path with Leptonica library files: " ${LEPTONICA_ROOT_DIR})

set(TESSERACT_INSTALL_PREFIX /usr/local/ CACHE PATH "Tesseract install root directory")
message(STATUS "Tesseract install root directory: " ${TESSERACT_INSTALL_PREFIX})

get_filename_component(VHAT_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../.." ABSOLUTE)
message(STATUS "Repository root: " ${VHAT_ROOT})

set(VHAT_CMAKE_TOOLCHAIN_FILE "${VHAT_ROOT}/infrastructure/cmake/toolchains/desktop.cmake" CACHE PATH "CMake toolchain file")
message(STATUS "Toolchain: " ${VHAT_CMAKE_TOOLCHAIN_FILE})

get_filename_component(TESSERACT_ROOT "${VHAT_ROOT}/../third_party/Tesseract" ABSOLUTE CACHE)
message(STATUS "Tesseract build root: " ${TESSERACT_ROOT})

### Determining Tesseract version
if(NOT DEFINED VHAT_TESSERACT_VERSION)
  file(STRINGS version.sh VHAT_LIBRARY_VERSIONS)
  foreach(VHAT_LIB_VER ${VHAT_LIBRARY_VERSIONS})
    if(${VHAT_LIB_VER} MATCHES "^VHAT_TESSERACT_VERSION=*")
      string(REPLACE "VHAT_TESSERACT_VERSION=" "" VHAT_TESSERACT_VERSION ${VHAT_LIB_VER})
    endif()
  endforeach()
endif(NOT DEFINED VHAT_TESSERACT_VERSION)

message(STATUS "VHAT_TESSERACT_VERSION: " ${VHAT_TESSERACT_VERSION})

### Downloading the Tesseract

set(TESSERACT_ARCHIVE_FILENAME "${VHAT_TESSERACT_VERSION}.tar.gz")

set(TESSERACT_ARCHIVE_FILE "${TESSERACT_ROOT}/${TESSERACT_ARCHIVE_FILENAME}")
message(STATUS "Tesseract archive destination: " ${TESSERACT_ARCHIVE_FILE})

set(TESSERACT_URL "https://github.com/tesseract-ocr/tesseract/archive/${TESSERACT_ARCHIVE_FILENAME}")
message(STATUS "Tesseract URL: " ${TESSERACT_URL})

if(NOT EXISTS "${TESSERACT_ARCHIVE_FILE}")
  file(
    DOWNLOAD "${TESSERACT_URL}" "${TESSERACT_ARCHIVE_FILE}"
    SHOW_PROGRESS
  )
else()
  message(STATUS "Tesseract archive was downloaded earlier. Skip downloading")
endif()

# Extracting the Tesseract archive
execute_process(
  COMMAND
    "${CMAKE_COMMAND}" -E chdir "${TESSERACT_ROOT}"
    "${CMAKE_COMMAND}" -E tar xvf "${TESSERACT_ARCHIVE_FILE}"

  RESULT_VARIABLE TAR_RETURN
)

if(NOT 0 EQUAL ${TAR_RETURN})
  message(FATAL_ERROR "tar terminated with failure. Return code: " ${TAR_RETURN})
endif()

set(TESSERACT_SOURCE_PATH "${TESSERACT_ROOT}/tesseract-${VHAT_TESSERACT_VERSION}")
message(STATUS "Directory with Teseract source code: " ${TESSERACT_SOURCE_PATH})

set(TESSERACT_BUILD_PATH "${TESSERACT_ROOT}/build-tesseract-${VHAT_TESSERACT_VERSION}")
message(STATUS "Directory with Teseract build: " ${TESSERACT_BUILD_PATH})

### Building Tesseract using Autotools

include(ProcessorCount)
include("${VHAT_CMAKE_TOOLCHAIN_FILE}")

execute_process(
  COMMAND
    "${CMAKE_COMMAND}" -E chdir "${TESSERACT_SOURCE_PATH}"
    ./autogen.sh
  RESULT_VARIABLE AUTOGEN_RETURN
)

if(NOT 0 EQUAL ${AUTOGEN_RETURN})
  message(FATAL_ERROR "Tesseract autogen terminated with failure. Return code: " ${AUTOGEN_RETURN})
endif()

execute_process(
  COMMAND
    "${CMAKE_COMMAND}" -E chdir "${TESSERACT_SOURCE_PATH}"
    ./configure
      "--prefix=${TESSERACT_INSTALL_PREFIX}"
      "CC=${CMAKE_C_COMPILER}"
      "CXX=${CMAKE_CXX_COMPILER}"
      "PKG_CONFIG_PATH=${LEPTONICA_ROOT_DIR}/lib/pkgconfig"
  RESULT_VARIABLE CONFIGURE_RETURN
)

if(NOT 0 EQUAL ${AUTOGEN_RETURN})
  message(FATAL_ERROR "Tesseract configutation terminated with failure. Return code: " ${CONFIGURE_RETURN})
endif()
ProcessorCount(PROCESSOR_COUNT)
if(0 EQUAL PROCESSOR_COUNT)
  message(WARNING "Couldn't count the number of processors")
else()
  message(STATUS "Count of processors: " ${PROCESSOR_COUNT})
  set(PARALLEL_ARG "-j${PROCESSOR_COUNT}")
endif()

execute_process(
  COMMAND
    "${CMAKE_COMMAND}" -E chdir "${TESSERACT_SOURCE_PATH}"
    make ${PARALLEL_ARG}
  RESULT_VARIABLE MAKE_RETURN
)

if(NOT 0 EQUAL ${MAKE_RETURN})
  message(FATAL_ERROR "Tesseract Make terminated with failure. Return code: " ${MAKE_RETURN})
endif()

execute_process(
  COMMAND
    "${CMAKE_COMMAND}" -E chdir "${TESSERACT_SOURCE_PATH}"
    sudo make install
  RESULT_VARIABLE INSTALLING_RETURN
)

if(NOT 0 EQUAL ${INSTALLING_RETURN})
  message(FATAL_ERROR "Tesseract installing terminated with failure. Return code: " ${INSTALLING_RETURN})
endif()
