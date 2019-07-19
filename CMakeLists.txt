cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(GNUInstallDirs)

include(${CMAKE_SOURCE_DIR}/infrastructure/cmake/functions/git.cmake)

git_get_commit_hash(GIT_COMMIT_HASH)
message(STATUS "On a git commit: " ${GIT_COMMIT_HASH})

git_get_latest_tag(GIT_TAG GIT_TAG_VERSION_MAJOR GIT_TAG_VERSION_MINOR)
message(STATUS "Most recent visible tag: " ${GIT_TAG})

git_get_number_of_commits_since_tag(GIT_COMMITS_SINCE_TAG ${GIT_TAG})
message(STATUS "Number of commits since ${GIT_TAG}: " ${GIT_COMMITS_SINCE_TAG})

if(${GIT_COMMITS_SINCE_TAG} EQUAL 0)
  set(VERSION_MINOR ${GIT_TAG_VERSION_MINOR})
  set(VERSION_PATCH 0)
else()
  math(EXPR VERSION_MINOR "${GIT_TAG_VERSION_MINOR} + 1")
  set(VERSION_PATCH ${GIT_COMMITS_SINCE_TAG})
endif()

project(VHAT
  VERSION ${GIT_TAG_VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
)

file(WRITE "${CMAKE_BINARY_DIR}/vhat_version" "${PROJECT_VERSION}\n")
install(FILES "${CMAKE_BINARY_DIR}/vhat_version" DESTINATION ${CMAKE_INSTALL_SYSCONFDIR})

set(CMAKE_CXX_STANDARD 14 CACHE STRING
  "Default value for CXX_STANDARD property of targets")

set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE BOOL
  "Default value for CXX_STANDARD_REQUIRED property of targets")

#------------------------------------------ cmake includes ------------------------------------------#
# cmake functions
include(${PROJECT_SOURCE_DIR}/infrastructure/cmake/functions/functions.cmake)

#------------------------------------------ compiler flags ------------------------------------------#
if(UNIX)
  add_cxx_compiler_flag(-Wall)
  add_cxx_compiler_flag(-Wextra)
  add_cxx_compiler_flag(-pedantic)
  add_cxx_compiler_flag(-Werror)
endif(UNIX)

# BUILD_SHARED_LIBS=ON necessary for building shared gtest library
option(BUILD_SHARED_LIBS "Build shared libs by default" ON)

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

#------------------------------------------ unit tests ------------------------------------------#
if(VHAT_WITH_TESTS)
  find_package(GTest REQUIRED)

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
