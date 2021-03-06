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
readonly VHAT_DEV_TOOLS="wget g++-5 ninja-build cmake clang-format autoconf automake libtool pkg-config gcovr"
readonly VHAT_USE_LIBS="libssl-dev python2.7-dev"
readonly DBUS_LIBS="libdbus-glib-1-dev libglib2.0-dev"
readonly MYSQL_SERVER_LIBS="mysql-server"
readonly MYSQL_CLIENT_LIBS="libmariadb-dev"

## For non-interactive mysql installation
echo "mysql-server mysql-server/root_password password OhMyGodIamaRoot" | sudo debconf-set-selections
echo "mysql-server mysql-server/root_password_again password OhMyGodIamaRoot" | sudo debconf-set-selections

if [ $TARGET != $LVDS_TARGET ]
then
  readonly GSTREAMER_LIBS="libgstreamer1.0-dev libgstreamer-plugins-base1.0-dev"
fi

# List of the all necessary packages
readonly INSTALL_PACKAGES="$VHAT_DEV_TOOLS $VHAT_USE_LIBS $GSTREAMER_LIBS $DBUS_LIBS $MYSQL_SERVER_LIBS"

sudo apt install -y $INSTALL_PACKAGES

## MariaDB connector for MySQL server
# all actions related to MariaDB should be done ONLY after mysql-server was installed, cause it affects it
sudo apt install -y software-properties-common
sudo apt-key adv --fetch-keys 'http://mariadb.org/mariadb_release_signing_key.asc'
sudo add-apt-repository 'deb [arch=arm64] http://mirrors.nav.ro/mariadb/repo/10.4/ubuntu xenial main'
sudo apt update

sudo apt install -y $MYSQL_CLIENT_LIBS

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
