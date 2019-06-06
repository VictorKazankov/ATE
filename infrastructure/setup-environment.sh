## Example for usage:
## ./setup-environment.sh [lvds]

readonly LVDS_TARGET="lvds"
readonly DEV_TARGET="dev"

if [ $1 ]
then
  if [ $1 == $LVDS_TARGET ]
  then
    readonly TARGET=$LVDS_TARGET
  else
    echo "Unknown target: $1"
    echo "Run with option 'lvds' for build on LVDS board or without any args for dev setup environment."
    exit 1
  fi
else
  readonly TARGET=$DEV_TARGET
fi

echo "Setting up VHAT build environment for: $TARGET"

## Update repositories
sudo apt update

## Install packages
readonly VHAT_DEV_TOOLS="wget g++-5 ninja-build cmake clang-format autoconf automake libtool pkg-config"
readonly VHAT_USE_LIBS="libssl-dev libjsoncpp-dev"
readonly IMAGE_FORMAT_LIBS="zlib1g-dev libpng-dev libjpeg-dev libtiff-dev"
if [ $TARGET != $LVDS_TARGET ]; then readonly OPENCV_USE_LIBS="libavcodec-dev libavformat-dev libswscale-dev libgtk2.0-dev"; fi
readonly TESSERACT_DEV_TOOLS="autoconf automake libtool"

# List of the all necessary packages
readonly INSTALL_PACKAGES="$VHAT_DEV_TOOLS $VHAT_USE_LIBS $IMAGE_FORMAT_LIBS $OPENCV_USE_LIBS $TESSERACT_DEV_TOOLS"

sudo apt install -y $INSTALL_PACKAGES

## Go to the source root directory
cd "$(dirname "$0")/.."

if [ $TARGET != $LVDS_TARGET ]
then
  ## Download submodules
  git submodule init
  git submodule update

  ## Copy Visual Studio Code tasks
  mkdir -pv .vscode
  cp infrastructure/ide_integration/vscode-linux-tasks.json .vscode/tasks.json
else
  echo -e "\nWARNING: due to some limitations, git submodules were not updated automatically."
  echo "Please ensure you have manually init and updated submodules by running commands:"
  echo "  git submodule init"
  echo "  git submodule update"
  read -n 1 -s -r -p "Press any key to continue..."
fi

# Build the required libraries
cd infrastructure/build_dependencies
if [ $TARGET != $LVDS_TARGET ]
then
  sh ./opencv.sh
else
  echo -e "\nWARNING: OpenCV is assumed to be installed from JetPack 3.3."
  echo "Please see installation instructions: https://github.ford.com/IVIToolsTeam/VHAT_2.0/wiki/Set-up-LVDS-board-for-the-ATE-Server"
  read -n 1 -s -r -p "Press any key to continue..."
fi
sh ./leptonica.sh
sh ./tesseract-trained-data.sh
cmake -P tesseract.cmake
