## Example for usage:
## ./setup-environment.sh

## Update repositories
sudo apt update

## Install packages
readonly VHAT_DEV_TOOLS="g++-5 ninja-build cmake clang-format"
readonly VHAT_USE_LIBS="libssl-dev libjsoncpp-dev"
readonly OPENCV_DEV_TOOLS="pkg-config ccache"
readonly OPENCV_USE_LIBS="libavcodec-dev libavformat-dev libswscale-dev libpng-dev libgtk2.0-dev"

# List of the all necessary packages
readonly INSTALL_PACKAGES="$VHAT_DEV_TOOLS $VHAT_USE_LIBS $OPENCV_DEV_TOOLS $OPENCV_USE_LIBS"

sudo apt install -y $INSTALL_PACKAGES

## Go to the source root directory
cd "$(dirname "$0")/.."

## Download submodules
git submodule init
git submodule update

## Copy Visual Studio Code tasks
mkdir -pv .vscode
cp infrastructure/ide_integration/vscode-linux-tasks.json .vscode/tasks.json

## OpenCV build

# Toolchain from the VHAT
readonly TOOLCHAIN_ABSOLUTE_PATH="$(pwd -P)/infrastructure/cmake/toolchains/desktop.cmake"

# OpenCV version number is saved in file
readonly OPENCV_VERSION=`cat infrastructure/config/opencv-version`

# Create a root directory for OpenCV sources, builds and installed files
mkdir -pv ../OpenCV
cd ../OpenCV

# Download archive if necessary and extract it
readonly OPENCV_ARCHIVE_FILENAME=$OPENCV_VERSION.tar.gz
wget -nc "https://github.com/opencv/opencv/archive/$OPENCV_ARCHIVE_FILENAME"
tar --skip-old-files -xvf $OPENCV_ARCHIVE_FILENAME

# Create directory for build files
readonly OPENCV_SOURCE_DIR_NAME=opencv-$OPENCV_VERSION

mkdir -pv build-$OPENCV_SOURCE_DIR_NAME
cd build-$OPENCV_SOURCE_DIR_NAME

readonly OPENCV_BUILD_ABSOLUTE_DIR=`pwd -P`

# OpenCV 3.3.1 doesn't build in C++14 mode, so used C++11 mode

cmake ../$OPENCV_SOURCE_DIR_NAME -GNinja \
  -DCMAKE_CXX_STANDARD:STRING=11 \
  -DCMAKE_CXX_STANDARD_REQUIRED:BOOL=ON \
  -DCMAKE_INSTALL_PREFIX:PATH=/opt/OpenCV \
  -DCMAKE_TOOLCHAIN_FILE:FILEPTAH="$TOOLCHAIN_ABSOLUTE_PATH" \
  -DBUILD_DOCS:BOOL=OFF \
  -DBUILD_PERF_TESTS:BOOL=OFF \
  -DBUILD_TESTS:BOOL=OFF \
  -DBUILD_opencv_apps:BOOL=OFF \
  -DBUILD_opencv_calib3d:BOOL=ON \
  -DBUILD_opencv_core:BOOL=ON \
  -DBUILD_opencv_dnn:BOOL=ON \
  -DBUILD_opencv_features2d:BOOL=ON \
  -DBUILD_opencv_flann:BOOL=ON \
  -DBUILD_opencv_highgui:BOOL=ON \
  -DBUILD_opencv_imgcodecs:BOOL=ON \
  -DBUILD_opencv_imgproc:BOOL=ON \
  -DBUILD_opencv_js:BOOL=OFF \
  -DBUILD_opencv_ml:BOOL=ON \
  -DBUILD_opencv_objdetect:BOOL=ON \
  -DBUILD_opencv_photo:BOOL=ON \
  -DBUILD_opencv_python3:BOOL=OFF \
  -DBUILD_opencv_shape:BOOL=OFF \
  -DBUILD_opencv_stitching:BOOL=ON \
  -DBUILD_opencv_superres:BOOL=OFF \
  -DBUILD_opencv_ts:BOOL=OFF \
  -DBUILD_opencv_video:BOOL=ON \
  -DBUILD_opencv_videoio:BOOL=ON \
  -DBUILD_opencv_videostab:BOOL=OFF \
  -DBUILD_opencv_world:BOOL=OFF \
  -DWITH_PNG:BOOL=ON

cmake --build .
sudo cmake --build . --target install
