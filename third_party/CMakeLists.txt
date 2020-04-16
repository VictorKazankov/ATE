cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

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
add_library(boost_asio STATIC boost_asio.cpp)
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
      _WINSOCK_DEPRECATED_NO_WARNINGS
  )
endif(MSVC)

target_compile_definitions(boost_asio
  PUBLIC
    BOOST_ASIO_DISABLE_BOOST_ARRAY
    BOOST_ASIO_DISABLE_BOOST_ASSERT
    BOOST_ASIO_DISABLE_BOOST_BIND
    BOOST_ASIO_DISABLE_BOOST_CHRONO
    BOOST_ASIO_DISABLE_BOOST_DATE_TIME
    BOOST_ASIO_DISABLE_BOOST_REGEX
    BOOST_ASIO_DISABLE_BOOST_THROW_EXCEPTION
    BOOST_ASIO_NO_DEPRECATED
    BOOST_ASIO_SEPARATE_COMPILATION
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

### inih
### https://github.com/benhoyt/inih
add_library(inihcpp STATIC
  inih/cpp/INIReader.cpp
  inih/ini.c
)

target_include_directories(inihcpp
  INTERFACE
    inih/cpp
    inih
)

# Libs linked to ate_common are de-facto shared object and must be compiled with -fPIC
set_target_properties(boost_asio inihcpp
  PROPERTIES
    POSITION_INDEPENDENT_CODE ON
)

if(ATE_BUILD_CLIENT)
  ### Pybind11
  ### https://pybind11.readthedocs.io/en/stable/index.html
  set(PYBIND11_PYTHON_VERSION 2.7 CACHE STRING "Pybind11 Python version")
  add_subdirectory(pybind11)

  if(WIN32)
    # Use pybind11 tools to get site-packages path
    include(pybind11/tools/FindPythonLibsNew.cmake)
    set(ATE_PYTHON_SITEPACKAGES_DIR ${PYTHON_SITE_PACKAGES} CACHE PATH "Python site-packages dir")
    message(STATUS "Python site-packages dir: " ${ATE_PYTHON_SITEPACKAGES_DIR})
  endif()
endif(ATE_BUILD_CLIENT)

### spdlog
### https://github.com/gabime/spdlog
add_subdirectory(spdlog EXCLUDE_FROM_ALL)
