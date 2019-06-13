# VHAT 2.0
Virtual HMI Automation Toolset.

## Prerequisites

1. Supported OS:
    - Ubuntu 18.04
    - Ubuntu 16.04 - LVDS board only
    - Windows 10 - ATE client only - TBD

1. (LVDS board only) Install OpenCV from JetPack 3.3: https://github.ford.com/IVIToolsTeam/VHAT_2.0/wiki/Set-up-LVDS-board-for-the-ATE-Server

## Build

1. Clone the repository:

    ```
    git@github.ford.com:IVIToolsTeam/VHAT_2.0.git
    ```

1. Run setup environment script:

    ```
    VHAT_2.0/infrastructure/setup-environment.sh
    ```

1. Run build script:

    ```
    cmake -P VHAT_2.0/infrastructure/build.cmake
    ```

1. By default, build root is set to _build-VHAT_2.0-Release_ directory.

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
    - Directory to place build artifact (default: VHAT_2.0/../build-VHAT_2.0-\<build type\>):
      ```
      VHAT_BUILD_ROOT:PATH
      ```
    - Toggle ATE client build (default: ON):
      ```
      VHAT_BUILD_CLIENT:BOOL
      ```
    - Toggle ATE server build (default: ON):
      ```
      VHAT_BUILD_SERVER:BOOL
      ```
    - Toggle build of unit-tests (default: ON):
      ```
      VHAT_WITH_TESTS:BOOL
      ```
    - Tweak toolchain (default: VHAT_2.0/infrastructure/cmake/toolchains/desktop.cmake):
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

## Run ATE server on LVDS board

1. Open server config for edit:
    ```
    build-VHAT_2.0-Release/ATE_server/data/config.ini
    ```

1. Set up video stream source:
    ```
    [VIDEO_STREAM]
    Source = V4L2
    Device = /dev/video0
    FrameWidth = <resolution width as configured on TDK>
    FrameHeight = <resolution heigh as configured on TDK>
    ```

1. Set up path to send touch events to TDK:
    ```
    [INTERACTION]
    Type = SPI
    DeviceAddress = /dev/spidev3.0
    ```

1. Set up icon storage:
    ```
    [DB]
    Path = <path to icon storage relative to data/ directory>
    CollectionMode = <name of HMI theme to be used>
    ```

    A file named _\<HMI theme name\>.json_ must be present under icon storage path.

1. Save the _config.ini_ and run ATE server:
    ```
    build-VHAT_2.0-Release/ATE_server/src/vhat_server
    ```

1. In case some config options are needed to be changed, ATE server must be restarted.

## Run ATE server outside the LVDS board

1. Open server config for edit:
    ```
    build-VHAT_2.0-Release/ATE_server/data/config.ini
    ```

1. Set up video stream source:
    ```
    [VIDEO_STREAM]
    Source = RTSP
    RtspStream=<address of rtsp stream>
    ```

1. Set up path to send touch events to TDK:
    ```
    [INTERACTION]
    Type = VDP
    Address = <ip address of LVDS board running VDP server>
    Port = <port of VDP server on LVDS board>
    DisplayType = <type of HMI display as configured on TDK>
    ```

1. Set up icon storage:
    ```
    [DB]
    Path = <path to icon storage relative to data/ directory>
    CollectionMode = <name of HMI theme to be used>
    ```

    A file named _\<HMI theme name\>.json_ must be present under icon storage path.

1. Save the _config.ini_ and run ATE server:
    ```
    build-VHAT_2.0-Release/ATE_server/src/vhat_server
    ```

1. In case some config options are needed to be changed, ATE server must be restarted.

## Use ATE client

* Import _vhat_client.so_ (located at _\<build root\>/ATE_client/src_) into your Python 2.7 code.
