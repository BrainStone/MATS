#! /bin/bash

# Make sure the user did not forget to update the submodules and they are at the correct version
echo -e "\e[1mCloning all submodules\e[m"
git submodule update --init --recursive
git submodule sync --recursive

if [ $# -eq 0 ] || [ "_$1" != "_-s" ]
then
  # Install dependencies
  echo "Installing all dependencies"
  sudo apt-get install -y g++ libboost-all-dev libconfig++-dev libncursesw5-dev
fi

# Make and install the program
echo -e "\e[1mCompiling...\e[m"
make release
echo -e "\e[1mInstalling...\e[m"
sudo make install

echo -e "\n\e[1mDone!\e[m\n"
