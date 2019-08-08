# Go to the source root directory
cd "$(dirname "$0")/../.."

readonly ROOT_DIR=$(pwd -P)

# Toolchain from the VHAT
readonly TOOLCHAIN_ABSOLUTE_PATH="$ROOT_DIR/infrastructure/cmake/toolchains/desktop.cmake"

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
  -DCMAKE_INSTALL_PREFIX:PATH=/usr/local \
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

# This is approximate dependency list for the enabled OpenCV modules. This is actual only for Ubuntu18:
UBUNTU18_DEPENDS="libtbb2, zlib1g"  # core
UBUNTU18_DEPENDS+=", libavcodec57 | libavcodec-extra57, libavformat57, libavutil55, libswscale4, libopenexr22, libgphoto2-6, libgphoto2-port12" # videio
UBUNTU18_DEPENDS+=", libgdal20, libgdcm2.8, libilmbase12, libjpeg8, libpng16-16, libtiff5, libwebp6" # imgcodecs and imgproc
UBUNTU18_DEPENDS+=", libgdk-pixbuf2.0-0, libglib2.0-0, libgtk-3-0, libcairo2" # highgui

UBUNTU18_CONFLICTS="libopencv-calib3d3.2, libopencv-contrib3.2, libopencv-core3.2, libopencv-features2d3.2, libopencv-flann3.2, libopencv-highgui3.2"
UBUNTU18_CONFLICTS+=", libopencv-imgcodecs3.2, libopencv-imgproc3.2, libopencv-ml3.2, libopencv-objdetect3.2, libopencv-photo3.2"
UBUNTU18_CONFLICTS+=", libopencv-shape3.2, libopencv-stitching3.2, libopencv-superres3.2, libopencv-video3.2, libopencv-videoio3.2"

m4 \
  -DSOURCE_DIR="../${OPENCV_SOURCE_DIR_NAME}" \
  -DNAME="libopencv" \
  -DPACKAGING_PREFIX="/usr/local" \
  -DARCH=$(dpkg-architecture -q DEB_HOST_ARCH) \
  -DDEPENDS="$UBUNTU18_DEPENDS" \
  -DCONFLICTS="$UBUNTU18_CONFLICTS" \
  -DDESCRIPTION="OpenCV libs for VHAT needs" \
  -DVERSION="${VHAT_OPENCV_VERSION}" \
  "$ROOT_DIR/infrastructure/build_dependencies/CPackConfig.m4" > CPackConfig.cmake

sudo cpack .
mv -vf libopencv*.deb ../
