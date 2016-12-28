#! /bin/bash

# Make sure the user did not forget to update the submodules
echo "Cloning all submodules"
git submodule update --init --recursive

if [ $# -eq 0 ] || [ "_$1" != "_-s" ]
then
  # Install dependencies
  echo "Installing all dependencies"
  sudo apt-get install -y g++ libboost-all-dev libconfig++-dev libncursesw5-dev
fi

# Make and install the program
echo "Compiling..."
make
echo "Installing..."
sudo make install

echo -e "\nDone!"
