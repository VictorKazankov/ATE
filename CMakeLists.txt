cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

project(VHAT
  VERSION 0.0.1.0
)

set(CMAKE_CXX_STANDARD 14 CACHE STRING
  "Default value for CXX_STANDARD property of targets")

set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL
  "Default value for CXX_STANDARD_REQUIRED property of targets")

option(VHAT_BUILD_CLIENT "Build VHAT client" ON)
option(VHAT_BUILD_SERVER "Build VHAT server" ON)
option(VHAT_WITH_TESTS "Build VHAT with tests" ON)

message(STATUS ${PROJECT_NAME} " v" ${PROJECT_VERSION})
message(STATUS "VHAT_BUILD_CLIENT: " ${VHAT_BUILD_CLIENT})
message(STATUS "VHAT_BUILD_SERVER: " ${VHAT_BUILD_SERVER})
message(STATUS "VHAT_WITH_TESTS: " ${VHAT_WITH_TESTS})
message(STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_INSTALL_PREFIX: " ${CMAKE_INSTALL_PREFIX})
message(STATUS "CMAKE_TOOLCHAIN_FILE: " ${CMAKE_TOOLCHAIN_FILE})
message(STATUS "VHAT_CREATE_DEBIAN_PACKAGE: " ${VHAT_CREATE_DEBIAN_PACKAGE})
message(STATUS "VHAT_INSTALL_SERVER_SERVICE: " ${VHAT_INSTALL_SERVER_SERVICE})

add_subdirectory(third_party)
add_subdirectory(ATE_common/src)

if(VHAT_BUILD_SERVER)
  add_subdirectory(ATE_server/src)
endif(VHAT_BUILD_SERVER)

if(VHAT_BUILD_CLIENT)
  add_subdirectory(ATE_client/src)
endif(VHAT_BUILD_CLIENT)

if(VHAT_WITH_TESTS)
  # Enable testing via CTest
  # https://cmake.org/cmake/help/latest/manual/ctest.1.html
  include(CTest)
  enable_testing()

  # ATE tests directories
  add_subdirectory(ATE_common/test)

  if(VHAT_BUILD_SERVER)
    add_subdirectory(ATE_server/test)
  endif(VHAT_BUILD_SERVER)
endif(VHAT_WITH_TESTS)

if(VHAT_CREATE_DEBIAN_PACKAGE)
  # Unfortunately Debian package cannot be packed with an arbitrary install prefix.
  # As the binary depends on an absolute path stored in CMAKE_INSTALL_PREFIX var.
  set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
  set(CPACK_GENERATOR "DEB")
  set(CPACK_PACKAGE_CONTACT "Luxoft")
  set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
  set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
  set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})

  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libssl1.0.0, python2.7")
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

  set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}_${CPACK_DEBIAN_ARCHITECTURE}")

  if (VHAT_INSTALL_SERVER_SERVICE)
    set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA
      "${CMAKE_CURRENT_SOURCE_DIR}/ATE_server/service/postinst"
      "${CMAKE_CURRENT_SOURCE_DIR}/ATE_server/service/prerm"
    )
  endif(VHAT_INSTALL_SERVER_SERVICE)

  include(CPack)
endif(VHAT_CREATE_DEBIAN_PACKAGE)
