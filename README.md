# ATE
Automated Test Environment.

Contents
--------
1.  [Prerequisites](#prerequisites)
    1. [PC](#pc)
    2. [LVDS board](#lvds-board)
2.  [Build](#build)
    1. [Prepare environment](#prepare-environment)
    2. [Generate a build system](#generate-a-build-system)
    3. [Build a project](#build-a-project) 
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

## Prerequisites

### PC
Supported OS:
* Ubuntu 18.04 LTS
* Windows 10 (ATE client only) - **TBD**

### LVDS board
1. LVDS board rev 0.9 or higher (aka _Luxoft baseboard v.1.0 ("green")_ or _Luxoft baseboard v.1.1 ("black")_ )

1. Board set up with latest Jetson TX1 update package from: https://github.ford.com/LVDS2Eth/Jetson_TX1_Update_Package

1. OpenCV installed from JetPack 3.3: https://github.ford.com/VHAT/ATE/wiki/Set-up-LVDS-board-for-the-ATE-Server

Build
-----

Building this project, consists of three stages:

1. ### Prepare environment

    - Clone the repository:
    
        ```shell
        git clone git@github.ford.com:VHAT/ATE.git
        ```
    - Run setup environment script:
    
        ```shell
        ATE/infrastructure/setup-environment.sh
        ```

2. ### Generate a build system
    
    ```shell
    cmake path/to/sources [options ...]
    ``` 

    [More about options](#options).

3. ### Build a project
    
    ```shell
    cmake --build path/to/build/directory [--target target]
    ```
    
    [More about targets](#targets).

   
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
