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

### Determining the type of build [CMake|Make]

if(NOT DEFINED VHAT_BUILD_TESSERACT_VIA_CMAKE)
  message(STATUS "Determining the type of build [CMake|Make] ...")
  execute_process(
    COMMAND uname -p
    OUTPUT_VARIABLE UNAME_STDOUT
  )

  string(TOLOWER "${UNAME_STDOUT}" TARGET_ARCHITECTURE)

  if(${TARGET_ARCHITECTURE} MATCHES "(x86)|(amd64)")
    set(VHAT_BUILD_TESSERACT_VIA_CMAKE ON)
  else()
    set(VHAT_BUILD_TESSERACT_VIA_CMAKE OFF)
  endif()
endif()

message(STATUS "Build Tesseract via CMake: " ${VHAT_BUILD_TESSERACT_VIA_CMAKE})

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

### Building Tesseract

if(${VHAT_BUILD_TESSERACT_VIA_CMAKE})

  ## Building Tesseract via CMake

  if(EXISTS "${TESSERACT_BUILD_PATH}")
    message(STATUS ${TESSERACT_BUILD_PATH} " already exists. Updating old build")
  else()
    message(STATUS "Creating Tesseract build root directory: " ${TESSERACT_BUILD_PATH})
    file(MAKE_DIRECTORY "${TESSERACT_BUILD_PATH}")
  endif()

  # Definition of the Leptonica_DIR is a workaround for usage find_package(Leptonica)
  # instead of pkg_check_modules(Leptonica)
  # It's necessary because pkg_check_modules doesn't search modules in the /opt directory
  # in default mode instead of find_package

  # Generating Tesseract project
  execute_process(
    COMMAND
      "${CMAKE_COMMAND}" -E chdir "${TESSERACT_BUILD_PATH}"
      "${CMAKE_COMMAND}" "${TESSERACT_SOURCE_PATH}"
        -GNinja
        -DCMAKE_BUILD_TYPE=Release
        -DCMAKE_INSTALL_PREFIX:PATH='${TESSERACT_INSTALL_PREFIX}'
        -DCMAKE_INSTALL_RPATH_USE_LINK_PATH:BOOL=ON
        -DCMAKE_TOOLCHAIN_FILE:FILEPTAH='${VHAT_CMAKE_TOOLCHAIN_FILE}'
        -DLeptonica_DIR:PATH='${LEPTONICA_ROOT_DIR}'

    RESULT_VARIABLE GENERATING_RETURN
  )

  if(NOT 0 EQUAL ${GENERATING_RETURN})
    message(FATAL_ERROR "Tesseract CMake generating terminated with failure. Return code: " ${GENERATING_RETURN})
  endif()

  # Compiling Tesseract project
  execute_process(
    COMMAND
      "${CMAKE_COMMAND}" -E chdir "${TESSERACT_BUILD_PATH}"
      "${CMAKE_COMMAND}" --build .

    RESULT_VARIABLE BUILDING_RETURN
  )

  if(NOT 0 EQUAL ${BUILDING_RETURN})
    message(FATAL_ERROR "Tesseract CMake building terminated with failure. Return code: " ${BUILDING_RETURN})
  endif()

  # Installing Tesseract project
  execute_process(
    COMMAND
      "${CMAKE_COMMAND}" -E chdir "${TESSERACT_BUILD_PATH}"
      sudo "${CMAKE_COMMAND}" --build . --target install

    RESULT_VARIABLE INSTALLING_RETURN
  )

  if(NOT 0 EQUAL ${INSTALLING_RETURN})
    message(FATAL_ERROR "Tesseract CMake installing terminated with failure. Return code: " ${INSTALLING_RETURN})
  endif()
else()
  ## Building Tesseract via Automake

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
endif()
