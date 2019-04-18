## Example for usage:
## ./setup-environment.sh

cd "$(dirname "$0")/.."

## Download submodules
git submodule init
git submodule update

## Update repositories
sudo apt update

## Install development tools
sudo apt install -y g++-5 ninja-build cmake clang-format ccache

## Install the libraries with headers needed for the VHAT
sudo apt install -y libssl-dev libjsoncpp-dev

## Copy Visual Studio Code tasks
mkdir -pv .vscode
cp infrastructure/ide_integration/vscode-linux-tasks.json .vscode/tasks.json

## OpenCV build

# Install OpenCV dependencies
sudo apt install -y libavcodec-dev libavformat-dev libswscale-dev libpng-dev

# Toolchain from the VHAT
readonly TOOLCHAIN_ABSOLUTE_PATH="$(pwd -P)/infrastructure/cmake/toolchains/desktop.cmake"

# OpenCV version number is saved in file
readonly OPENCV_VERSION=`cat infrastructure/config/opencv-version`

# Create a root directory for OpenCV sources, builds and installed files
mkdir -pv ../OpenCV
cd ../OpenCV

# Prefix for installation files
readonly OPENCV_INSTALL_ABSOLUTE_DIR=`pwd -P`

# Download archive if necessary and extract it
readonly OPENCV_ARCHIVE_FILENAME=$OPENCV_VERSION.tar.gz
wget -nc "https://github.com/opencv/opencv/archive/$OPENCV_ARCHIVE_FILENAME"
tar --skip-old-files -xvf $OPENCV_ARCHIVE_FILENAME

# Create directory for build files
readonly OPENCV_SOURCE_DIR_NAME=opencv-$OPENCV_VERSION

mkdir -pv build-$OPENCV_SOURCE_DIR_NAME
cd build-$OPENCV_SOURCE_DIR_NAME

readonly OPENCV_BUILD_ABSOLUTE_DIR=`pwd -P`

cmake ../$OPENCV_SOURCE_DIR_NAME -GNinja \
  -DCMAKE_INSTALL_PREFIX:PATH="$OPENCV_INSTALL_ABSOLUTE_DIR" \
  -DCMAKE_TOOLCHAIN_FILE:FILEPTAH="$TOOLCHAIN_ABSOLUTE_PATH" \
  -DBUILD_DOCS:BOOL=OFF \
  -DBUILD_PERF_TESTS:BOOL=OFF \
  -DBUILD_TESTS:BOOL=OFF \
  -DBUILD_opencv_apps:BOOL=OFF \
  -DBUILD_opencv_calib3d:BOOL=ON \
  -DBUILD_opencv_core:BOOL=ON \
  -DBUILD_opencv_dnn:BOOL=OFF \
  -DBUILD_opencv_features2d:BOOL=ON \
  -DBUILD_opencv_flann:BOOL=OFF \
  -DBUILD_opencv_highgui:BOOL=OFF \
  -DBUILD_opencv_imgcodecs:BOOL=ON \
  -DBUILD_opencv_imgproc:BOOL=ON \
  -DBUILD_opencv_js:BOOL=OFF \
  -DBUILD_opencv_ml:BOOL=OFF \
  -DBUILD_opencv_objdetect:BOOL=OFF \
  -DBUILD_opencv_photo:BOOL=OFF \
  -DBUILD_opencv_python3:BOOL=OFF \
  -DBUILD_opencv_shape:BOOL=OFF \
  -DBUILD_opencv_stitching:BOOL=OFF \
  -DBUILD_opencv_superres:BOOL=OFF \
  -DBUILD_opencv_ts:BOOL=OFF \
  -DBUILD_opencv_video:BOOL=OFF \
  -DBUILD_opencv_videoio:BOOL=ON \
  -DBUILD_opencv_videostab:BOOL=OFF \
  -DBUILD_opencv_world:BOOL=OFF \
  -DWITH_PNG:BOOL=ON

cmake --build . --target install
