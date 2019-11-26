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

#------------------------------------------ cmake includes ------------------------------------------#
# cmake functions
include(${PROJECT_SOURCE_DIR}/infrastructure/cmake/functions/functions.cmake)

# Install into GNU standard dirs
# Must be included after project() to set up all the variables correctly
include(GNUInstallDirs)

option(ATE_BUILD_CLIENT "Build ATE client" ON)
option(ATE_BUILD_SERVER "Build ATE server" ON)
option(ATE_WITH_TESTS "Build ATE with tests" ON)
option(ATE_COVERAGE "Enable test coverage" OFF)

# The icon storage path is universal for whole VDP project and must never change
set(ATE_WRITABLE_DATA_PREFIX "/var/lib/vdp/vhat" CACHE STRING "Prefix for ATE writable data directories" )
set(VHAT_ICON_STORAGE_PREFIX "${ATE_WRITABLE_DATA_PREFIX}/icon_storage" CACHE STRING "Prefix for ATE icon storage")

# All configs must be placed into <install prefix>/etc/vdp
if(${CMAKE_INSTALL_PREFIX} STREQUAL "/usr")
  set(ATE_INSTALL_SYSCONFDIR /${CMAKE_INSTALL_SYSCONFDIR}/vdp)
else()
  set(ATE_INSTALL_SYSCONFDIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/vdp)
endif()
message(STATUS "ATE_INSTALL_SYSCONFDIR: " ${ATE_INSTALL_SYSCONFDIR})

# All libs must ignore Debian specific /lib/<multi-arch> policy and simply install into <install prefix>/lib
# Since there are several libraries installed, group them into <install prefix>/lib/ate
set(ATE_INSTALL_LIBPREFIX lib)
set(ATE_INSTALL_LIBDIR ${ATE_INSTALL_LIBPREFIX}/vhat)

file(WRITE "${CMAKE_BINARY_DIR}/ate_version" "${PROJECT_VERSION_FULL}\n")
install(FILES "${CMAKE_BINARY_DIR}/ate_version" DESTINATION ${ATE_INSTALL_SYSCONFDIR})

message(STATUS ${PROJECT_NAME} " v" ${PROJECT_VERSION_FULL})
message(STATUS "ATE_BUILD_CLIENT: " ${ATE_BUILD_CLIENT})
message(STATUS "ATE_BUILD_SERVER: " ${ATE_BUILD_SERVER})
message(STATUS "ATE_WITH_TESTS: " ${ATE_WITH_TESTS})
message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})
message(STATUS "CMAKE_TOOLCHAIN_FILE: " ${CMAKE_TOOLCHAIN_FILE})
message(STATUS "ATE_CREATE_DEBIAN_PACKAGE: " ${ATE_CREATE_DEBIAN_PACKAGE})
message(STATUS "ATE_INSTALL_SERVER_SERVICE: " ${ATE_INSTALL_SERVER_SERVICE})
message(STATUS "ATE_WRITABLE_DATA_PREFIX: " ${ATE_WRITABLE_DATA_PREFIX})
message(STATUS "VHAT_INSTALL_ICON_STORAGE: " ${VHAT_INSTALL_ICON_STORAGE})
message(STATUS "VHAT_ICON_STORAGE_PREFIX: " ${VHAT_ICON_STORAGE_PREFIX})

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
if(ATE_WITH_TESTS)
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
endif(ATE_WITH_TESTS)

if(ATE_CREATE_DEBIAN_PACKAGE)
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

  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libssl1.0.0, vdp-libdbmanager (>= ${PROJECT_VERSION}),\
                                    vdp-librecognition (>= ${PROJECT_VERSION}), vdp-mcu-utils,\
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
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${PROJECT_VERSION_FULL}_${CPACK_DEBIAN_ARCHITECTURE}")

  set(POSTINST_FILE "${CMAKE_CURRENT_BINARY_DIR}/postinst")
  set(PRERM_FILE "${CMAKE_CURRENT_BINARY_DIR}/prerm")

  file(WRITE ${POSTINST_FILE} "#!/bin/bash\n")
  file(WRITE ${PRERM_FILE} "#!/bin/bash\n")

  if (ATE_INSTALL_SERVER_SERVICE)
    file(APPEND ${POSTINST_FILE} "
      systemctl enable vdp_ate_server
      systemctl start vdp_ate_server
    ")

    file(APPEND ${PRERM_FILE} "
      systemctl stop vdp_ate_server
      systemctl disable vdp_ate_server
    ")
  endif(ATE_INSTALL_SERVER_SERVICE)

  if (VHAT_INSTALL_ICON_STORAGE)
    #TODO: change the user and group names according to VDP policies
    file(APPEND ${POSTINST_FILE} "
      USER=nvidia
      GROUP=nvidia
      PERMISSIONS=\"a+rw\"
      echo \"Changing owner of ${VHAT_ICON_STORAGE_PREFIX} to \${USER}:\${GROUP}...\"
      if chown --recursive \${USER}:\${GROUP} ${VHAT_ICON_STORAGE_PREFIX}
      then
        echo \"Ok\"
      else
        echo \"error: failed to set owner of ${VHAT_ICON_STORAGE_PREFIX} to \${USER}:\${GROUP}\"
      fi

      echo \"Changing permissions of ${VHAT_ICON_STORAGE_PREFIX} to \${PERMISSIONS}...\"
      if chmod --recursive \${PERMISSIONS} ${VHAT_ICON_STORAGE_PREFIX}
      then
        echo \"Ok\"
      else
        echo \"error: failed to set permissions of ${VHAT_ICON_STORAGE_PREFIX} to \${PERMISSIONS}\"
      fi
    ")
  endif(VHAT_INSTALL_ICON_STORAGE)

  set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA ${POSTINST_FILE} ${PRERM_FILE})

  include(CPack)
endif(ATE_CREATE_DEBIAN_PACKAGE)
