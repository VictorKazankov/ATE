## Example for usage:
## ./setup-environment.sh

## Update repositories
sudo apt update

## Install packages
readonly VHAT_DEV_TOOLS="g++-5 ninja-build cmake clang-format pkg-config"
readonly VHAT_USE_LIBS="libssl-dev libjsoncpp-dev"
readonly IMAGE_FORMAT_LIBS="libpng-dev libjpeg-dev libtiff-dev"
readonly OPENCV_USE_LIBS="libavcodec-dev libavformat-dev libswscale-dev libgtk2.0-dev"
readonly TESSERACT_DEV_TOOLS="autoconf automake libtool"

# List of the all necessary packages
readonly INSTALL_PACKAGES="$VHAT_DEV_TOOLS $VHAT_USE_LIBS $IMAGE_FORMAT_LIBS $OPENCV_USE_LIBS $TESSERACT_DEV_TOOLS"

sudo apt install -y $INSTALL_PACKAGES

## Go to the source root directory
cd "$(dirname "$0")/.."

## Download submodules
git submodule init
git submodule update

## Copy Visual Studio Code tasks
mkdir -pv .vscode
cp infrastructure/ide_integration/vscode-linux-tasks.json .vscode/tasks.json

# Build the required libraries
cd infrastructure/build_dependencies
sh ./opencv.sh
sh ./leptonica.sh
sh ./tesseract.sh
