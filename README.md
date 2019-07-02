# VHAT 2.0
Virtual HMI Automation Toolset.

## Prerequisites

### PC
Supported OS:
* Ubuntu 18.04 LTS
* Windows 10 (VHAT client only) - **TBD**

### LVDS board
1. LVDS board rev 0.9 or higher (aka _Luxoft baseboard v.1.0 ("green")_ or _Luxoft baseboard v.1.1 ("black")_ )

1. Board set up with latest Jetson TX1 update package from: https://github.ford.com/LVDS2Eth/Jetson_TX1_Update_Package

1. OpenCV installed from JetPack 3.3: https://github.ford.com/IVIToolsTeam/VHAT_2.0/wiki/Set-up-LVDS-board-for-the-ATE-Server

## Build and install

1. Clone the repository:

    ```
    git clone git@github.ford.com:IVIToolsTeam/VHAT_2.0.git
    ```

1. Run setup environment script:

    ```
    VHAT_2.0/infrastructure/setup-environment.sh
    ```

1. Run build script:

    ```
    cmake -P VHAT_2.0/infrastructure/build.cmake
    ```

By default, build root is set to _build-VHAT_2.0-Release_ and installation is done into _build-VHAT_2.0-Release/install_.

### Configure build

1. Syntax:

    ```
    cmake [-D<option>:<type>=<value>] -P build.cmake
    ```

1. Configurable options:
    - Build type (default: Release)
      ```
      CMAKE_BUILD_TYPE:STRING=[Debug|Release|RelWithDebInfo|MinSizeRel|…]
      ```
    - Directory to place build files (default: _VHAT_2.0/../build-VHAT_2.0-\<build type\>_):
      ```
      VHAT_BUILD_ROOT:PATH
      ```
    - Toggle VHAT install (default: _ON_):
      ```
      VHAT_INSTALL:BOOL
      ```
    - Install directory aka _\<install prefix>_ (default: _<build root\>/install_):
      ```
      CMAKE_INSTALL_PREFIX:PATH
      ```
    - Toggle ATE client build (default: _ON_):
      ```
      VHAT_BUILD_CLIENT:BOOL
      ```
    - Toggle ATE server build (default: _ON_):
      ```
      VHAT_BUILD_SERVER:BOOL
      ```
    - Toggle build of unit-tests (default: _ON_):
      ```
      VHAT_WITH_TESTS:BOOL
      ```
    - Tweak toolchain (default: _VHAT_2.0/infrastructure/cmake/toolchains/desktop.cmake_):
      ```
      CMAKE_TOOLCHAIN_FILE:FILEPATH
      ```

1. Examples:
    1. Build Debug:
        ```
        cmake -DCMAKE_BUILD_TYPE:STRING=Debug -P build.cmake
        ```
    1. Build only client and disable unit tests:
        ```
        cmake -DVHAT_BUILD_SERVER:BOOL=OFF -DVHAT_WITH_TESTS:BOOL=OFF -P build.cmake
        ```
    1. Build and install into _/usr/local_:
        ```
        sudo cmake -DCMAKE_INSTALL_PREFIX=/usr/local -P build.cmake
        ```

## Running the unit tests

1. Run all tests:
    ```
    cd build-VHAT_2.0-Release
    ctest
    ```
1. Run tests manually:
    ```
    build-VHAT_2.0-Release/ATE_server/src/vhat_server_test
    build-VHAT_2.0-Release/ATE_common/src/vhat_common_test
    ```

## Run VHAT server on LVDS board

1. Open server config for edit:
    ```
    <install prefix>/etc/vhat_server.ini
    ```

1. Set up video stream:
    ```
    [VIDEO_STREAM]
    FrameWidth = <TDK display resolution width>
    FrameHeight = <TDK display resolution height>
    ```

1. Select icon storage:
    ```
    [DB]
    Target = <TDK Sync version (Sync3 | Sync4)>
    ```

1. Save the _vhat_server.ini_ and run VHAT server:
    ```
    <install prefix>/bin/vhat_server
    ```

VHAT server need to be restarted in order to apply any config or icon storage changes.

## Run ATE server outside the LVDS board (debug purpose only)

1. Open server config for edit:
    ```
    <install prefix>/etc/vhat_server.ini
    ```

1. Set up video stream source:
    ```
    [VIDEO_STREAM]
    Source = RTSP
    Path = rtsp://127.0.0.1:8554/lvds
    ```

1. Either set up path to send touch events to TDK via VDP server:
    ```
    [INTERACTION]
    Type = VDP
    Address = <ip address of LVDS board running VDP server>
    Port = <port of VDP server on LVDS board>
    DisplayType = <type of HMI display as configured on TDK>
    ```

   Or just stub it:
    ```
    [INTERACTION]
    Type = Dummy
    ```

1. Select icon storage:
    ```
    [DB]
    Target = <TDK sync version (Sync3 | Sync4)>
    ```

1. Save the _vhat_server.ini_ and run VHAT server:
    ```
   <install prefix>/bin/vhat_server
    ```

For full video stream support you will also need to set up webrtc client which is not covered by this project.

## Use VHAT client on LVDS board

Import _vhat_client.so_ from _<install prefix\>/lib/python2.7/dist-packages_ into your Python 2.7 code.

If _\<install prefix>_ was set to a standard system resource directory (i.e. _/usr_ or _/usr/local_) then _vhat_client.so_ could be looked up from any point of a system.

Otherwise you'd like to add module location to _PYTHONPATH_ or look it up manually.

No additional configuration is needed.

## Use VHAT client outside LVDS board

1. Open client config for edit:
    ```
    <install prefix>/etc/vhat_client.ini
    ```
1. Set IP address of LVDS board running VHAT server:
    ```
    [BOARD]
    Address = <IP address>
    ```
1. Import _vhat_client.so_ from _<install prefix\>/lib/python2.7/dist-packages_ into your Python 2.7 code. Module look up rules are the same as for LVDS board case above.