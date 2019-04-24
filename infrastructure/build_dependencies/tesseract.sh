# Go to the source root directory
cd "$(dirname "$0")/../.."

# Toolchain from the VHAT
readonly TOOLCHAIN_ABSOLUTE_PATH="$(pwd -P)/infrastructure/cmake/toolchains/desktop.cmake"

# Tesseract version number is saved in the file
. infrastructure/build_dependencies/version.sh

# Create a root directory for Tesseract sources and build files
mkdir -pv ../third_party/Tesseract
cd ../third_party/Tesseract

# Download archive if necessary and extract it
readonly TESSERACT_ARCHIVE_FILENAME="$VHAT_TESSERACT_VERSION.tar.gz"
wget -nc "https://github.com/tesseract-ocr/tesseract/archive/$TESSERACT_ARCHIVE_FILENAME"
tar --skip-old-files -xvf "$TESSERACT_ARCHIVE_FILENAME"

# Create directory for build files
readonly TESSERACT_SOURCE_DIR_NAME="tesseract-$VHAT_TESSERACT_VERSION"

mkdir -pv "build-$TESSERACT_SOURCE_DIR_NAME"
cd "build-$TESSERACT_SOURCE_DIR_NAME"

# Definition of the Leptonica_DIR is a workaround for usage find_package(Leptonica)
# instead of pkg_check_modules(Leptonica)
# It's necessary because pkg_check_modules doesn't search modules in the /opt directory
# in default mode instead of find_package

cmake ../$TESSERACT_SOURCE_DIR_NAME -GNinja \
  -DCMAKE_INSTALL_PREFIX:PATH=/opt/Tesseract \
  -DCMAKE_TOOLCHAIN_FILE:FILEPTAH="$TOOLCHAIN_ABSOLUTE_PATH" \
  -DLeptonica_DIR:PATH=/opt/Leptonica

cmake --build .
sudo cmake --build . --target install
