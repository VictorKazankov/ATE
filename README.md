# ATE
Automated Test Environment.

Contents
--------
1. [Prepare build environment](#prepare-build-environment)
    * [Linux (VDP board or Desktop Ubuntu 18.04)](#linux-vdp-board-or-desktop-ubuntu-1804)
    * [Windows 10 PC](#windows-10-pc)
1. [Build and install](#build-and-install)
    * [Linux (VDP board or Desktop Ubuntu 18.04)](#linux-vdp-board-or-desktop-ubuntu-1804-1)
    * [Windows 10 PC](#windows-10-pc-1)
3.  [Options](#options)
    1. [ATE_BUILD_CLIENT](#ATE_BUILD_CLIENT-default-on)
    2. [ATE_BUILD_SERVER](#ATE_BUILD_SERVER-default-on)
    3. [ATE_COVERAGE](#ATE_COVERAGE-default-off)
    4. [ATE_TESTING](#ATE_TESTING-default-on)
    5. [ATE_PACKAGE](#ATE_PACKAGE-default-off)
    6. [ATE_INSTALL_SERVER_SERVICE](#ATE_INSTALL_SERVER_SERVICE-default-off)
    7. [CMAKE_TOOLCHAIN_FILE](#CMAKE_TOOLCHAIN_FILE)
4.  [Targets](#targets)
    1.  [default](#default)
    2.  [check](#check)
    3.  [test](#test)
    4.  [coverage](#coverage)
    5.  [coverage-html](#coverage-html)
    6.  [package](#package)    
    7.  [install](#install)    
5.  [Examples](#examples)
6.  [Usage](#usage)
7.  [Build dependencies](#build-dependencies)

## Prepare build environment

### Linux (VDP board or Desktop Ubuntu 18.04)

1. Install tools and dependencies:
    * #### Desktop Ubuntu 18.04:
        1. Run:
            ```shell
            sudo apt update
            sudo apt install git cmake libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev libdbus-glib-1-dev libglib2.0-dev python2.7-dev
            ```
    * #### VDP board TX1:
        1. Add entries to _/etc/apt/sources.list_:
            ```
            deb http://ports.ubuntu.com/ubuntu-ports/ xenial main restricted universe multiverse
            deb http://ports.ubuntu.com/ubuntu-ports/ xenial-updates main restricted universe multiverse
            ```
        1. Run:
            ```shell
            sudo apt update
            sudo apt install git cmake libdbus-glib-1-dev libglib2.0-dev python2.7-dev
            ```

1. Clone, build and install VHAT dependencies:
    * [ThirdParty](https://github.ford.com/VHAT/ThirdParty)
    * [DBManagerLib](https://github.ford.com/VHAT/DBManagerLib)
    * [RecognitionLib](https://github.ford.com/VHAT/RecognitionLib)

1. Clone repository and init submodules:
    ```shell
    git clone git@github.ford.com:VHAT/ATE.git
    git -C ATE submodule update --init
    ```

### Windows 10 PC

1. Download and install Python 2.7 with latest bugfix https://www.python.org/download/releases/2.7/
1. Download and install latest CMake https://cmake.org/download/
1. Download and install latest git https://git-scm.com/downloads/
1. Download and install Visual Studio 2015 or later https://visualstudio.microsoft.com/downloads/.
1. In Visual Studio Installer select _Desktop development with C++_ and install latest _MSVC build tools_ and _Windows 10 SDK_.
1. Clone, build and install [ThirdParty](https://github.ford.com/VHAT/ThirdParty).

## Build and install

### Linux (VDP board or Desktop Ubuntu 18.04)

1. Create build dir and generate build tool files there:
    ```shell
    mkdir build && cd build
    cmake ../ATE
    ```

    * To configure build check [options section](#options) .

1. Build:
    ```shell
    cmake --build .
    ```
    
    * To choose build target check [targets section](#targets).

1. Install:
    ```shell
    sudo cmake --build . --target install
    ```

    * To choose install directory check [install section](#install).

### Windows 10 PC

Only ATE client is supported on Windows platform.

1. Create build dir and generate build tool files there:

    ```
    md build
    cd build
    cmake ..\ATE
    ```

1. Build
    * Debug:
        ```
        cmake --build . --config Debug
        ```

    * Release:
        ```
        cmake --build . --config Release
        ```

1. Install
    * Debug:
        ```
        cmake --build . --target install --config Debug
        ```
    * Release:
        ```
        cmake --build . --target install --config Release
        ```

Options
-------

- #### ATE_BUILD_CLIENT (default: ON)

    ```shell
    cmake path/to/sources -DATE_BUILD_CLIENT=ON [other options ...]
    ```

- #### ATE_BUILD_SERVER (default: ON)

    ```shell
    cmake path/to/sources -DATE_BUILD_SERVER=ON [other options ...]
    ```

- #### ATE_COVERAGE (default: OFF)

    ```shell
    cmake path/to/sources -DATE_COVERAGE=ON [other options ...]
    ```

    Turns on the [`coverage`](#coverage) target which performs code coverage measurement.

- #### ATE_TESTING (default: ON)

    ```shell
    cmake path/to/sources -DATE_TESTING=OFF [other options ...]
    ```
    
    Provides the ability to turn off unit testing and hence the [`check`](#check) target. As a result, the code coverage measurement is also turned off (see [Code coverage](#DATE_TESTING)).

- #### ATE_PACKAGE (default: OFF)

    ```shell
    cmake path/to/sources -DCMAKE_INSTALL_PREFIX=/usr -DATE_PACKAGE=ON [other options ...]
    ```

    Turns on the [`package`](#package) target which allow create deb package.

    It is highly recommended to create Debian package that would intall into _/usr_. For now, packaging prefix is strongly tied to the install prefix.

- #### ATE_INSTALL_SERVER_SERVICE (default: OFF)

    ```shell
    cmake path/to/sources -DATE_INSTALL_SERVER_SERVICE=ON [other options ...]
    ```

- #### CMAKE_TOOLCHAIN_FILE

    ```shell
    cmake path/to/sources -DCMAKE_TOOLCHAIN_FILE=path/to/toolchain/file [other options ...]
    ```

Targets
-------

- #### Default

    ```shell
    cmake --build path/to/build/directory
    cmake --build path/to/build/directory --target all
    ```
    
    If a target is not specified (which is equivalent to the `all` target), it builds everything possible including unit tests and also calls the [`check`](#check) target.

- #### check

    ```shell
    cmake --build path/to/build/directory --target check
    ```
    
    Launches built (and builds if not yet) unit tests. Enabled by default.
    
    * On Windows must specify `--config Debug` or `--config Release` option. e.g.:

        ```shell
        cmake --build . --config Debug --target check
        ```

    See also [`test`](#test).

- #### test

    ```shell
    cmake --build path/to/build/directory --target test
    ```
    
    Launches built (and builds if not yet) unit tests. Enabled by default.
    
    See also [`check`](#check).

- #### coverage

    ```shell
    cmake --build path/to/build/directory --target coverage
    ```
    
    Analyzes run unit tests (and runs is not yet) using [gcovr](https://gcovr.com).
    
    Target is only available if [`ATE_COVERAGE`](#ATE_COVERAGE) option is on.
    
    See also [`check`](#check) and ['test'](#test).

- #### coverage-html

    ```shell
    cmake --build path/to/build/directory --target coverage_html
    ```
    Generate html (index.html) report in binary directory.
    
    Analyzes run unit tests (and runs is not yet) using [gcovr](https://gcovr.com).
    
    Target is only available if [`ATE_COVERAGE`](#ATE_COVERAGE) option is on.
    
    See also [`check`](#check) and ['test'](#test).

- #### package

    ```shell
    cmake path/to/sources -DATE_PACKAGE=ON
    cmake --build path/to/build/directory --target package
    ```
    
    Creating package for install.
    
    Target is only available if [`ATE_PACKAGE`](#ATE_PACKAGE) option is on.

- #### install

    ```shell
    cmake path/to/sources -DCMAKE_INSTALL_PREFIX=/usr
    cmake --build path/to/build/directory --target install
    ```
    
    Installing ATE to ```CMAKE_INSTALL_PREFIX``` folder.
    
    ##### The correct installation method is to install the package!
    
Examples
--------

 - #### Build Debug:
        
    ```shell
    cmake path/to/sources -DCMAKE_BUILD_TYPE=Debug 
    cmake --build <path/to/build/directory>
    ```
   
 - #### Building the project in the debug mode and measure a test coverage

    ```shell
    cmake path/to/sources -DCMAKE_BUILD_TYPE=Debug -DATE_COVERAGE=ON
    cmake --build path/to/build/directory --target coverage
    ```      
    
- #### Build only client and disable unit tests:
        
    ```
    cmake path/to/sources -DATE_BUILD_SERVER=OFF -DATE_TESTING=OFF
    cmake --build path/to/build/directory>
    ```

- #### Build project with ninja generator:
        
    ```
    cmake path/to/sources -GNinja
    cmake --build path/to/build/directory>
    ```
        
- #### Build and install into _/usr_:
    
  ```
    cmake path/to/sources -DCMAKE_INSTALL_PREFIX=/usr
    sudo cmake --build path/to/build/directory --target install
    ```
  
  ##### The correct installation method is to install the package!
  
- #### Building the project in the release mode with creating a package (see [target package](#package)).

    ```shell
    cmake path/to/sources -DCMAKE_BUILD_TYPE=Release -DATE_PACKAGE=ON -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_PREFIX_PATH=path/to/installed/dependencies
    cmake --build path/to/build/directory --target package
    ```

- #### System install and deploy package

    1. Enable server service install, install icon storage and create package :
        
       ```shell
        cmake path/to/sources -DCMAKE_INSTALL_PREFIX=/usr -DATE_INSTALL_SERVER_SERVICE=ON -DATE_PACKAGE=ON
        cmake --build path/to/build/directory --target package
        ```
      
    2. You can use _dpkg_ package manager to install this package. This command will also replace the previous installation of ATE:
    
        ```
        sudo dpkg -i ate-X.Y.Z.deb
        ```

- #### Running the unit tests
    
    ```shell
     cmake path/to/sources -DATE_TESTING=ON
     cmake --build path/to/build/directory --target test
    ```
    
    or
     
    ```shell
     cmake path/to/sources -DATE_TESTING=ON
     cmake --build path/to/build/directory --target check
    ```

Usage
-----  

### Run ATE server on LVDS board

1. Open server config for edit:
    ```
    <install prefix>/etc/ate_server.ini
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

1. Save the _ate_server.ini_ and run ATE server:
    ```
    <install prefix>/bin/ate_server
    ```

ATE server need to be restarted in order to apply any config or icon storage changes.

### Run ATE server outside the LVDS board (debug purpose only)

1. Open server config for edit:
    ```
    <install prefix>/etc/ate_server.ini
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

1. Save the _ate_server.ini_ and run ATE server:
    ```
   <install prefix>/bin/ate_server
    ```

For full video stream support you will also need to set up webrtc client which is not covered by this project.

### Use ATE client on LVDS board

Import _vhat_client.so_ from _<install prefix\>/lib/python2.7/dist-packages_ into your Python 2.7 code.

If _\<install prefix>_ was set to a standard system resource directory (i.e. _/usr_ or _/usr/local_) then _vhat_client.so_ could be looked up from any point of a system.

Otherwise you'd like to add module location to _PYTHONPATH_ or look it up manually.

No additional configuration is needed.

### Use ATE client outside LVDS board

1. Open client config for edit:
    ```
    <install prefix>/etc/ate_client.ini
    ```
1. Set IP address of LVDS board running ATE server:
    ```
    [BOARD]
    Address = <IP address>
    ```
1. Import _vhat_client.so_ from _<install prefix\>/lib/python2.7/dist-packages_ into your Python 2.7 code. Module look up rules are the same as for LVDS board case above.


Build dependencies
------------------

#### Linux

1. Build tools and libs provided by Ubuntu dev packages are automatically installed by [setup-environment.sh](infrastructure/setup-environment.sh) script.
2. [DBManagerLib](https://github.ford.com/VHAT/DBManagerLib)
3. [RecognitionLib](https://github.ford.com/VHAT/RecognitionLib)
4. [JsonCpp](https://github.ford.com/VHAT/ThirdParty)
5. [GoogleTest](https://github.ford.com/VHAT/ThirdParty)
6. (LVDS board only) [GStreamer](https://github.ford.com/LVDS2Eth/Jetson_Gstreamer14)

#### Windows

TBD
