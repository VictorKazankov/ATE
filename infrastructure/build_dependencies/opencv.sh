# Go to the source root directory
cd "$(dirname "$0")/../.."

# Toolchain from the VHAT
readonly TOOLCHAIN_ABSOLUTE_PATH="$(pwd -P)/infrastructure/cmake/toolchains/desktop.cmake"

# OpenCV version number is saved in the file
. infrastructure/build_dependencies/version.sh

# Create a root directory for OpenCV sources and build files
mkdir -pv ../third_party/OpenCV
cd ../third_party/OpenCV

# Download archive if necessary and extract it
readonly OPENCV_ARCHIVE_FILENAME="$VHAT_OPENCV_VERSION.tar.gz"
wget -nc "https://github.com/opencv/opencv/archive/$OPENCV_ARCHIVE_FILENAME"
tar --skip-old-files -xvf "$OPENCV_ARCHIVE_FILENAME"

# Create directory for build files
readonly OPENCV_SOURCE_DIR_NAME="opencv-$VHAT_OPENCV_VERSION"

mkdir -pv "build-$OPENCV_SOURCE_DIR_NAME"
cd "build-$OPENCV_SOURCE_DIR_NAME"

cmake ../$OPENCV_SOURCE_DIR_NAME -GNinja \
  -DCMAKE_INSTALL_PREFIX:PATH=/opt/OpenCV \
  -DCMAKE_TOOLCHAIN_FILE:FILEPTAH="$TOOLCHAIN_ABSOLUTE_PATH" \
  -DENABLE_CXX11:BOOL=ON \
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
  -DBUILD_JPEG:BOOL=OFF \
  -DWITH_JPEG:BOOL=ON \
  -DBUILD_PNG:BOOL=OFF \
  -DWITH_PNG:BOOL=ON \
  -DBUILD_TIFF:BOOL=OFF \
  -DWITH_TIFF:BOOL=ON

cmake --build .
sudo cmake --build . --target install
