#! /bin/bash -e

# Change directory to directory of the script so that make and git commands work
cd "$(dirname "$0")"

# Make sure the user did not forget to update the submodules and they are at the correct version
echo -e "\e[1mCloning all submodules\n----------------------\e[m"
git submodule update --init --recursive
git submodule sync --recursive

if [ $# -eq 0 ] || [ "_$1" != "_-s" ]
then
  # Install dependencies
  echo -e "\n\e[1mInstalling all required dependencies\n------------------------------------\e[m"
  sudo apt-get install -y g++ make libboost-program-options-dev libboost-filesystem-dev libconfig++-dev libncurses5-dev libncursesw5-dev
fi

# Make and install the program
echo -e "\n\e[1mCompiling\n---------\e[m"
make release
echo -e "\n\e[1mInstalling\n----------\e[m"
sudo make install

echo -e "\n\n\e[1mDone!\n=====\e[m\n"
