# Example for usage:
# ./build.sh [Debug|Release|RelWithDebInfo|MinSizeRel|…]
# TODO: add the command-line argument parsing

cd "$(dirname "$0")/.."
readonly SOURCE_ABSOLUTE_DIR=`pwd -P`

# Script that contains versions of the external libraries
. infrastructure/build_dependencies/version.sh

if [ "$1" ]
then
  readonly BUILD_TYPE="$1"
else
  readonly BUILD_TYPE=Debug
fi

readonly SOURCE_DIR_NAME=`basename $SOURCE_ABSOLUTE_DIR`
readonly BUILD_DIR_NAME="build-$SOURCE_DIR_NAME-$BUILD_TYPE"

readonly TOOLCHAIN_ABSOLUTE_PATH="$SOURCE_ABSOLUTE_DIR/infrastructure/cmake/toolchains/desktop.cmake"

mkdir -pv "../$BUILD_DIR_NAME"
cd "../$BUILD_DIR_NAME"

cmake "$SOURCE_ABSOLUTE_DIR" -GNinja \
  -DCMAKE_BUILD_TYPE:STRING="$BUILD_TYPE" \
  -DCMAKE_TOOLCHAIN_FILE:FILEPATH="$TOOLCHAIN_ABSOLUTE_PATH" \
  -DVHAT_OPENCV_VERSION:STRING="$VHAT_OPENCV_VERSION"

cmake --build .
