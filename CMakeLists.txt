cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(${CMAKE_SOURCE_DIR}/infrastructure/cmake/functions/git.cmake)

git_get_version(VERSION_MAJOR VERSION_MINOR VERSION_PATCH VERSION_SUFFIX)

project(vdp-ate
  VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
)

set(PROJECT_VERSION_FULL "${PROJECT_VERSION}${VERSION_SUFFIX}")

set(CMAKE_CXX_STANDARD 14 CACHE STRING
  "Default value for CXX_STANDARD property of targets")

set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL
  "Default value for CXX_STANDARD_REQUIRED property of targets")

# Default build type is release
set(CMAKE_BUILD_TYPE Release)

#------------------------------------------ cmake includes ------------------------------------------#
# cmake functions
include(${PROJECT_SOURCE_DIR}/infrastructure/cmake/functions/functions.cmake)

# Install into GNU standard dirs
# Must be included after project() to set up all the variables correctly
include(GNUInstallDirs)

# Custom location for find_package to look up ThirdParty dependencies on Win32
if(WIN32)
  list(APPEND CMAKE_PREFIX_PATH "$ENV{USERPROFILE}/VHAT/ThirdParty")
endif()

# Config
option(ATE_BUILD_CLIENT "Build ATE client" ON)
conditional_option(ATE_BUILD_SERVER "Build ATE server" NOT WIN32)
option(ATE_TESTING "Build ATE with tests" ON)
option(ATE_COVERAGE "Enable test coverage" OFF)
option(ATE_INSTALL_SERVER_SERVICE "Install ATE server service" OFF)
option(ATE_PACKAGE "Create Debian package" OFF)

# Platform specific parameters
if(UNIX)
  set(ATE_WRITABLE_DATA_PREFIX "/var/lib/vdp/vhat" CACHE PATH "Prefix for ATE writable data directories" )

  # All configs must be placed into <install prefix>/etc/vdp
  if(${CMAKE_INSTALL_PREFIX} STREQUAL "/usr")
    set(ATE_INSTALL_SYSCONFDIR "/${CMAKE_INSTALL_SYSCONFDIR}/vdp" CACHE PATH "ATE config dir")
  else()
    set(ATE_INSTALL_SYSCONFDIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/vdp" CACHE PATH "ATE config dir")
  endif()

  # All libs must ignore Debian specific /lib/<multi-arch> policy and simply install into <install prefix>/lib
  # Since there are several libraries installed, group them into <install prefix>/lib/ate
  set(ATE_INSTALL_LIBPREFIX lib CACHE PATH "ATE install prefix for libs")
  set(ATE_INSTALL_LIBDIR "${ATE_INSTALL_LIBPREFIX}/vhat" CACHE PATH "ATE install dir for libs")
elseif(WIN32)
  if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(ATE_INSTALL_SYSCONFDIR "$ENV{ALLUSERSPROFILE}/VDP" CACHE PATH "ATE config dir")
  else()
    set(ATE_INSTALL_SYSCONFDIR "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/vdp" CACHE PATH "ATE config dir")
  endif()
endif()

file(WRITE "${CMAKE_BINARY_DIR}/ate_version" "${PROJECT_VERSION_FULL}\n")
install(FILES "${CMAKE_BINARY_DIR}/ate_version" DESTINATION ${ATE_INSTALL_SYSCONFDIR})

message(STATUS ${PROJECT_NAME} " v" ${PROJECT_VERSION_FULL})
message(STATUS "ATE_BUILD_CLIENT: " ${ATE_BUILD_CLIENT})
message(STATUS "ATE_BUILD_SERVER: " ${ATE_BUILD_SERVER})
message(STATUS "ATE_TESTING: " ${ATE_TESTING})
message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})
message(STATUS "CMAKE_TOOLCHAIN_FILE: " ${CMAKE_TOOLCHAIN_FILE})
message(STATUS "ATE_INSTALL_SYSCONFDIR: " ${ATE_INSTALL_SYSCONFDIR})
message(STATUS "ATE_PACKAGE: " ${ATE_PACKAGE})
message(STATUS "ATE_INSTALL_SERVER_SERVICE: " ${ATE_INSTALL_SERVER_SERVICE})
message(STATUS "ATE_WRITABLE_DATA_PREFIX: " ${ATE_WRITABLE_DATA_PREFIX})

add_subdirectory(third_party)
add_subdirectory(ATE_common/src)

if(ATE_BUILD_SERVER)
  add_subdirectory(ATE_server/src)
endif(ATE_BUILD_SERVER)

if(ATE_BUILD_CLIENT)
  add_subdirectory(ATE_client/src)
endif(ATE_BUILD_CLIENT)

#######################################################
#                   Test coverage
#######################################################
find_program(GCOVR_EXECUTABLE gcovr)

if (ATE_COVERAGE AND GCOVR_EXECUTABLE)
  add_custom_target(coverage
    COMMAND
      ${GCOVR_EXECUTABLE}
      --root=${PROJECT_SOURCE_DIR}/
      --filter=${PROJECT_SOURCE_DIR}/ATE_common/include
      --filter=${PROJECT_SOURCE_DIR}/ATE_common/src
      --filter=${PROJECT_SOURCE_DIR}/ATE_server/include
      --filter=${PROJECT_SOURCE_DIR}/ATE_server/src
      --filter=${PROJECT_SOURCE_DIR}/ATE_client/include
      --filter=${PROJECT_SOURCE_DIR}/ATE_client/src
      --object-directory=${PROJECT_BINARY_DIR}
      --print-summary
    DEPENDS
      check
  )

  add_custom_target(coverage_html
    COMMAND
      ${GCOVR_EXECUTABLE}
      --root=${PROJECT_SOURCE_DIR}/
      --filter=${PROJECT_SOURCE_DIR}/ATE_common/include
      --filter=${PROJECT_SOURCE_DIR}/ATE_common/src
      --filter=${PROJECT_SOURCE_DIR}/ATE_server/include
      --filter=${PROJECT_SOURCE_DIR}/ATE_server/src
      --filter=${PROJECT_SOURCE_DIR}/ATE_client/include
      --filter=${PROJECT_SOURCE_DIR}/ATE_client/src
      --object-directory=${PROJECT_BINARY_DIR}
      --html
      --sort-percentage
      --output=${PROJECT_BINARY_DIR}/index.html
    DEPENDS
      check
  )
elseif (ATE_COVERAGE AND NOT GCOVR_EXECUTABLE)
  set(ATE_COVERAGE OFF)
  message(WARNING "For test coverage need gcovr")
endif ()

#------------------------------------------ unit tests ------------------------------------------#
if(ATE_TESTING)
  find_package(GTest REQUIRED)
  FindGmock()

  # Enable testing via CTest
  # https://cmake.org/cmake/help/latest/manual/ctest.1.html
  include(CTest)
  enable_testing()

  # ATE tests directories
  add_subdirectory(ATE_common/test)

  if(ATE_BUILD_SERVER)
    add_subdirectory(ATE_server/test)
  endif(ATE_BUILD_SERVER)
  
  if(ATE_BUILD_CLIENT)
    add_subdirectory(ATE_client/test)
  endif(ATE_BUILD_CLIENT)

  # run all tests
  add_custom_target(check COMMAND ctest)
endif(ATE_TESTING)

if(ATE_PACKAGE)
  # Unfortunately Debian package cannot be packed with an arbitrary install prefix.
  # As the binary depends on an absolute path stored in CMAKE_INSTALL_PREFIX var.
  set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CPACK_GENERATOR "DEB")
  set(CPACK_PACKAGE_CONTACT "Luxoft")
  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}${VERSION_SUFFIX}")

  set(CPACK_DEBIAN_PACKAGE_CONFLICTS "vdp-vhat, vhat")
  set(CPACK_DEBIAN_PACKAGE_REPLACES "vdp-vhat, vhat")
  set(CPACK_DEBIAN_PACKAGE_PROVIDES "vdp-vhat (=${PROJECT_VERSION_FULL}), vhat (=${PROJECT_VERSION_FULL})")

  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libssl1.0.0, vdp-libdbmanager,\
                                    vdp-librecognition, vdp-mcu-utils,\
                                    vdp-gstreamer, vdp-media-server")
  set(CPACK_DEBIAN_PACKAGE_SUGGESTS "python2.7")
  set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
  set(CPACK_DEBIAN_PACKAGE_SECTION "unknown")
  set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Virtual HMI Automation Toolset")

  if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "aarch64")
    set(CPACK_DEBIAN_ARCHITECTURE "arm64")
  elseif(${CMAKE_SYSTEM_PROCESSOR} MATCHES "x86_64")
    set(CPACK_DEBIAN_ARCHITECTURE "amd64")
  else()
    message(SEND_ERROR "Unsupported architecture for Debian package")
  endif()

  set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}_${PROJECT_VERSION_FULL}_${CPACK_DEBIAN_ARCHITECTURE}")

  set(LEGACY_ICON_STORAGE "/var/lib/vdp/vhat/icon_storage")
  set(LEGACY_ICON_STORAGE_BAK "/var/lib/vdp/vhat/.icon_storage.bak")

  configure_file("${CMAKE_SOURCE_DIR}/data/debian/preinst" "${CMAKE_CURRENT_BINARY_DIR}/debian/preinst" @ONLY)
  configure_file("${CMAKE_SOURCE_DIR}/data/debian/postinst" "${CMAKE_CURRENT_BINARY_DIR}/debian/postinst" @ONLY)
  configure_file("${CMAKE_SOURCE_DIR}/data/debian/prerm" "${CMAKE_CURRENT_BINARY_DIR}/debian/prerm" @ONLY)
  
  set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
    "${CMAKE_CURRENT_BINARY_DIR}/debian/preinst"
    "${CMAKE_CURRENT_BINARY_DIR}/debian/postinst"
    "${CMAKE_CURRENT_BINARY_DIR}/debian/prerm"
  )

  include(CPack)
endif(ATE_PACKAGE)
