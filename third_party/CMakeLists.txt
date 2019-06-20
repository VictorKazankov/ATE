cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

include(GNUInstallDirs)

### Boost libraries
### Add new libraries from https://github.com/boostorg
### as git submodule
### into ./boost/libs directory

## Boost Config
## https://github.com/boostorg/config
add_subdirectory(boost/libs/config)

## Boost Predef
## https://github.com/boostorg/predef
add_subdirectory(boost/libs/predef)

## Boost Winapi
## https://github.com/boostorg/winapi
add_subdirectory(boost/libs/winapi)

## Boost System
## https://github.com/boostorg/system
add_subdirectory(boost/libs/system)

## Boost.Asio
## https://think-async.com
add_library(boost_asio SHARED boost_asio.cpp)
add_library(Boost::asio ALIAS boost_asio)

if(MSVC)
  # When BOOST_SYSTEM_USE_UTF8 is defined, on Windows
  # the library returns UTF-8 messages using code page CP_UTF8
  # instead of the default CP_ACP.
  # This macro has no effect on POSIX.
  # https://www.boost.org/doc/libs/1_70_0/libs/system/doc/html/system.html#ref_macros
  # https://docs.microsoft.com/en-us/windows/desktop/api/stringapiset/nf-stringapiset-widechartomultibyte

  # BOOST_USE_WINDOWS_H macro is part of Boost.Config.
  # If defined, Boost.WinAPI will include
  # <windows.h> and any other headers from Windows SDK as needed.
  # Otherwise Boost.WinAPI will provide its own declarations of the SDK components as needed
  # without including SDK headers.
  # https://www.boost.org/doc/libs/1_70_0/libs/winapi/doc/html/winapi/config.html

  target_compile_definitions(boost_asio
    PUBLIC
      BOOST_SYSTEM_USE_UTF8
      _WIN32_WINDOWS=_WIN32_WINNT_WIN7
  )
endif(MSVC)

# BOOST_ASIO_DYN_LINK necessary for separate compilling
# https://www.boost.org/doc/libs/1_70_0/doc/html/boost_asio/using.html#boost_asio.using.optional_separate_compilation
target_compile_definitions(boost_asio
  PUBLIC
    BOOST_ASIO_DISABLE_BOOST_ARRAY
    BOOST_ASIO_DISABLE_BOOST_ASSERT
    BOOST_ASIO_DISABLE_BOOST_BIND
    BOOST_ASIO_DISABLE_BOOST_CHRONO
    BOOST_ASIO_DISABLE_BOOST_DATE_TIME
    BOOST_ASIO_DISABLE_BOOST_REGEX
    BOOST_ASIO_DISABLE_BOOST_THROW_EXCEPTION
    BOOST_ASIO_DYN_LINK
    BOOST_ASIO_NO_DEPRECATED
)

target_include_directories(boost_asio
  PUBLIC
    boost/libs/asio/include
)

# This module determines the thread library of the system.
# https://cmake.org/cmake/help/latest/module/FindThreads.html
find_package(Threads REQUIRED)

target_link_libraries(boost_asio
  PUBLIC
    Boost::system
    Threads::Threads
)

if(VHAT_BUILD_SERVER)
  # Find the OpenSSL encryption library.
  # https://cmake.org/cmake/help/latest/module/FindOpenSSL.html
  # https://www.openssl.org
  # Debian/Ubuntu package name: libssl-dev

  find_package(OpenSSL REQUIRED)

  add_library(boost_asio_ssl SHARED boost_asio_ssl.cpp)
 
  target_include_directories(boost_asio_ssl
    PUBLIC
      boost/libs/asio/include
      "${OPENSSL_INCLUDE_DIR}"
  )

  target_link_libraries(boost_asio_ssl
    PUBLIC
      Boost::asio
    PRIVATE
      OpenSSL::Crypto
      OpenSSL::SSL
  )
endif()

### inih
### https://github.com/benhoyt/inih
add_library(inihcpp STATIC
  inih/cpp/INIReader.cpp
  inih/ini.c
)

# inihcpp is intended to be linked into the shared lib.
# Thus it requires -fPIC compilation option.
# https://stackoverflow.com/questions/38296756/what-is-the-idiomatic-way-in-cmake-to-add-the-fpic-compiler-option
set_property(TARGET inihcpp PROPERTY POSITION_INDEPENDENT_CODE ON)

target_include_directories(inihcpp
  INTERFACE
    inih/cpp
    inih
)

if(VHAT_BUILD_CLIENT)
  ### Pybind11
  ### https://pybind11.readthedocs.io/en/stable/index.html
  set(PYBIND11_PYTHON_VERSION 2.7 CACHE STRING "Pybind11 Python version")
  add_subdirectory(pybind11)
endif(VHAT_BUILD_CLIENT)

# JsonCpp
# https://github.com/open-source-parsers/jsoncpp
#Disable build and running unit test for jsoncpp
set(JSONCPP_WITH_TESTS OFF CACHE BOOL "Disable unit test build")
set(JSONCPP_WITH_POST_BUILD_UNITTEST OFF CACHE BOOL "Disable run jsoncpp unit test")
set(JSONCPP_WITH_PKGCONFIG_SUPPORT OFF CACHE BOOL "Disable cmake configure json cmake")
add_subdirectory(jsoncpp EXCLUDE_FROM_ALL)
set_property(TARGET jsoncpp_lib_static PROPERTY POSITION_INDEPENDENT_CODE ON)

### spdlog
### https://github.com/gabime/spdlog
add_subdirectory(spdlog EXCLUDE_FROM_ALL)

if(VHAT_WITH_TESTS)
  ### Google Test
  ### https://github.com/google/googletest

  ## For now, we don't use Google Mock
  set(BUILD_GMOCK OFF CACHE BOOL
    "Builds the Google Mock subproject")

  set(INSTALL_GTEST OFF CACHE BOOL
    "Enable installation of googletest. Projects embedding googletest may want to turn this OFF")

  # BUILD_SHARED_LIBS=ON necessary for building shared gtest library
  set(BUILD_SHARED_LIBS ON)
  add_subdirectory(googletest)

endif(VHAT_WITH_TESTS)

if(UNIX)
  install(TARGETS
    boost_asio
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
elseif(MSVC)
  install(TARGETS
    boost_asio
    RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
endif()

if(VHAT_BUILD_SERVER)
  install(TARGETS
    boost_asio_ssl
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )
endif()
