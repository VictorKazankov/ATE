# Go to the source root directory
cd "$(dirname "$0")/../.."

readonly ROOT_DIR=$(pwd -P)

# Toolchain from the VHAT
readonly TOOLCHAIN_ABSOLUTE_PATH="$ROOT_DIR/infrastructure/cmake/toolchains/desktop.cmake"

# Leptonica version number is saved in the file
. infrastructure/build_dependencies/version.sh

# Create a root directory for Leptonica sources and build files
mkdir -pv ../third_party/Leptonica
cd ../third_party/Leptonica

# Download archive if necessary and extract it
readonly LEPTONICA_ARCHIVE_FILENAME="$VHAT_LEPTONICA_VERSION.tar.gz"
wget -nc "https://github.com/DanBloomberg/leptonica/archive/$LEPTONICA_ARCHIVE_FILENAME"
tar --skip-old-files -xvf "$LEPTONICA_ARCHIVE_FILENAME"

# Create directory for build files
readonly LEPTONICA_SOURCE_DIR_NAME="leptonica-$VHAT_LEPTONICA_VERSION"

mkdir -pv "build-$LEPTONICA_SOURCE_DIR_NAME"
cd "build-$LEPTONICA_SOURCE_DIR_NAME"

readonly LEPTONICA_INSTALL_PREFIX=/usr/local

cmake ../$LEPTONICA_SOURCE_DIR_NAME -GNinja \
  -DCMAKE_CXX_STANDARD:STRING=11 \
  -DCMAKE_CXX_STANDARD_REQUIRED:BOOL=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX:PATH="$LEPTONICA_INSTALL_PREFIX" \
  -DCMAKE_TOOLCHAIN_FILE:FILEPTAH="$TOOLCHAIN_ABSOLUTE_PATH"

cmake --build .
sudo cmake --build . --target install

m4 \
  -DSOURCE_DIR="../${LEPTONICA_SOURCE_DIR_NAME}" \
  -DNAME="libleptonica" \
  -DPACKAGING_PREFIX=${LEPTONICA_INSTALL_PREFIX} \
  -DARCH=$(dpkg-architecture -q DEB_HOST_ARCH) \
  -DDEPENDS="libjpeg8, libopenjp2-7, libpng12-0 | libpng16-16, libtiff5, libwebp5 | libwebp6, zlib1g" \
  -DCONFLICTS="liblept5, libleptonica-dev" \
  -DDESCRIPTION="Leptonica lib for VHAT needs" \
  -DVERSION="${VHAT_LEPTONICA_VERSION}" \
  "$ROOT_DIR/infrastructure/build_dependencies/CPackConfig.m4" > CPackConfig.cmake

sudo cpack .
mv -vf libleptonica*.deb ../
