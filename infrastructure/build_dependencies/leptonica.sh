# Go to the source root directory
cd "$(dirname "$0")/../.."

# Toolchain from the VHAT
readonly TOOLCHAIN_ABSOLUTE_PATH="$(pwd -P)/infrastructure/cmake/toolchains/desktop.cmake"

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

cmake ../$LEPTONICA_SOURCE_DIR_NAME -GNinja \
  -DCMAKE_CXX_STANDARD:STRING=11 \
  -DCMAKE_CXX_STANDARD_REQUIRED:BOOL=ON \
  -DCMAKE_INSTALL_PREFIX:PATH=/opt/Leptonica \
  -DCMAKE_TOOLCHAIN_FILE:FILEPTAH="$TOOLCHAIN_ABSOLUTE_PATH"

cmake --build .
sudo cmake --build . --target install
