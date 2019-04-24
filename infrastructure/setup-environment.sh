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

# Build the required libraries
cd infrastructure/build_dependencies
sh ./opencv.sh
