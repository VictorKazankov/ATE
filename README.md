# ATE
Automated Test Environment.

## Prerequisites

### PC
Supported OS:
* Ubuntu 18.04 LTS
* Windows 10 (ATE client only) - **TBD**

### LVDS board
1. LVDS board rev 0.9 or higher (aka _Luxoft baseboard v.1.0 ("green")_ or _Luxoft baseboard v.1.1 ("black")_ )

1. Board set up with latest Jetson TX1 update package from: https://github.ford.com/LVDS2Eth/Jetson_TX1_Update_Package

1. OpenCV installed from JetPack 3.3: https://github.ford.com/VHAT/ATE/wiki/Set-up-LVDS-board-for-the-ATE-Server

## Build and install

1. Clone the repository:

    ```
    git clone git@github.ford.com:VHAT/ATE.git
    ```

1. Run setup environment script:

    ```
    ATE/infrastructure/setup-environment.sh
    ```

1. Run build script:

    ```
    cmake -P ATE/infrastructure/build.cmake
    ```

By default, build root is set to _build-ATE_2.0-Release_ and installation is done into _build-ATE_2.0-Release/install_.

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
    - Directory to place build files (default: _ATE/../build-ATE-\<build type\>_):
      ```
      VHAT_BUILD_ROOT:PATH
      ```
    - Toggle VHAT install (default: _ON_):
      ```
      VHAT_INSTALL:BOOL
      ```
    - Toggle VHAT server service install (default: _OFF_):
      ```
      VHAT_INSTALL_SERVER_SERVICE:BOOL
      ```
    - Install directory aka _\<install prefix>_ (default: _<build root\>/install_):
      ```
      CMAKE_INSTALL_PREFIX:PATH
      ```
    - Toggle VHAT deb package generation from install (default: _OFF_):
      ```
      VHAT_CREATE_DEBIAN_PACKAGE:BOOL
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
    - Tweak toolchain (default: _ATE/infrastructure/cmake/toolchains/desktop.cmake_):
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
### System install and deploy package

1. Set the install prefix to system dir and enable server service install. _sudo_ permissions are required to modify system dirs:
    ```
    sudo cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DVHAT_INSTALL_SERVER_SERVICE=ON -P infrastructure/build.cmake
    ```
1. The deploy package is _VHAT-X.Y.Z.deb_ located in _build-ATE-Release_ build root folder.
1. You can use _dpkg_ package manager to install this package. This command will also replace the previous installation of VHAT:
    ```
    sudo dpkg -i VHAT-X.Y.Z.deb
    ```

## Running the unit tests

1. Run all tests:
    ```
    cd build-ATE-Release
    ctest
    ```
1. Run tests manually:
    ```
    build-ATE-Release/ATE_server/src/vhat_server_test
    build-ATE-Release/ATE_common/src/vhat_common_test
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
    Target = <TDK Sync version (sync3 | sync4)>
    Build = <TDK Sync software build version>
    CollectionMode = <Current Sync display theme(day_mode | night_mode)>
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
    Target = <TDK sync version (sync3 | sync4)>
    Build = <Sync software build version>
    CollectionMode = <Current Sync display theme(day_mode | night_mode)>
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
