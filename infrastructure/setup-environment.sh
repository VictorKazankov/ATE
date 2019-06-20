## Example for usage:
## ./setup-environment.sh
##
## The target platform is determined automatically based on a CPU architecture:
## if running on aarch64 then it is LVDS board and this script must not install OpenCV.
## For other platforms, OpenCV is built and installed.

readonly LVDS_TARGET="lvds"
readonly DEV_TARGET="dev"

uname -p | grep -x -q aarch64 && readonly TARGET=$LVDS_TARGET || readonly TARGET=$DEV_TARGET

if [ ! $TARGET ]
then
  echo "Failed to detect target platfrom. Terminating."
  exit 1
fi

echo "Setting up VHAT build environment for: $TARGET"

## Update repositories
sudo apt update

## Install packages
readonly VHAT_DEV_TOOLS="wget g++-5 ninja-build cmake clang-format autoconf automake libtool pkg-config"
readonly VHAT_USE_LIBS="libssl-dev python2.7-dev"
readonly IMAGE_FORMAT_LIBS="zlib1g-dev libpng-dev libjpeg-dev libtiff-dev"

if [ $TARGET != $LVDS_TARGET ]
then
  readonly GSTREAMER_LIBS="libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev"
  readonly OPENCV_USE_LIBS="libavcodec-dev libavformat-dev libswscale-dev libgtk2.0-dev"
fi

readonly TESSERACT_DEV_TOOLS="autoconf automake libtool"

# List of the all necessary packages
readonly INSTALL_PACKAGES="$VHAT_DEV_TOOLS $VHAT_USE_LIBS $IMAGE_FORMAT_LIBS $OPENCV_USE_LIBS $TESSERACT_DEV_TOOLS $GSTREAMER_LIBS"

sudo apt install -y $INSTALL_PACKAGES

## Go to the source root directory
cd "$(dirname "$0")/.."

## Download submodules
git submodule init
git submodule update

if [ $TARGET != $LVDS_TARGET ]
then
  ## Copy Visual Studio Code tasks
  mkdir -pv .vscode
  cp infrastructure/ide_integration/vscode-linux-tasks.json .vscode/tasks.json
fi

# Build the required libraries
cd infrastructure/build_dependencies
if [ $TARGET != $LVDS_TARGET ] ; then sh ./opencv.sh; fi
sh ./leptonica.sh
sh ./tesseract-trained-data.sh
cmake -P tesseract.cmake

if [ $TARGET == $LVDS_TARGET ]
then
  echo -e "\nOpenCV is assumed to be installed from JetPack 3.3."
  echo "Please see installation instructions: https://github.ford.com/IVIToolsTeam/VHAT_2.0/wiki/Set-up-LVDS-board-for-the-ATE-Server"
fi
