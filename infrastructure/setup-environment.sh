# Example for usage:
# ./setup-environment.sh

cd "$(dirname "$0")/.."

# Download submodules
git submodule init
git submodule update

# Update repositories
sudo apt update

# Install development tools
sudo apt install -y g++-5 ninja-build cmake clang-format

# Install the libraries with headers needed for the VHAT
sudo apt install -y libssl-dev libjsoncpp-dev

# Copy Visual Studio Code tasks
mkdir -pv .vscode
cp infrastructure/ide_integration/vscode-linux-tasks.json .vscode/tasks.json
