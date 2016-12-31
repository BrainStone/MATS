# MATS
## Minecraft Administrative Tools for Servers

This software is a management system for Minecraft servers on linux systems. It has mainly been designed for servers which run many Minecraft servers, like
hosters or large server networks but can also be used for a few or even a single Minecraft server.

This project is developed using the C++11 standard, Boost and ncurses. So be sure to have them available when compiling (see install instrcutions below). The
software is developed and tested on several Debian 7 and 8 systems. It will likley work on other Linux distributions too however some adjustments might be
needed and furthermore I cannot gurantee that it will work at all. Should you have issues that can be resolved on my side feel free to open an Issue or a Pull
Request.

## Installing

There are no precompiled binaries available so you have to compile it yourself. But however you install this software, if you have done everything correctly you
will end up with a program binary called `mats` (located under `/usr/local/bin` which usually is in `PATH`) and the global config installed in `/etc/mats`.  
Depending on how confident you feel with your skills on the command line follow whichever guide appeals the most to you.

### Simple guide

    # Clone the repo. (We are using other repos in it. So clone them too)
    git clone --recursive https://github.com/BrainStone/MATS.git
    cd MATS
    
    # Make and install the program
    ./install.sh

`./install.sh` can be called with the parameter `-s` (for skip) to skip trying to install the dependencies.  
*(Also make sure you have `sudo` and `git` installed! Everything else you need will be installed automatically!)*

### Advanced guide

So if you are reading this I assume you are familiar with the standard command line tools and your operating system in general.

First of all, this repo has submodules so be sure to clone it recursively (with the `--recursive` flag). Should you by accident have forgotten to do this just
run the following two commands and you are good to go!

    git submodule update --init --recursive
    git submodule sync --recursive

After you cloned the repo verify that you have all the necessary tools, libararies and packages installed (See command below for package list). Simply run this
command if you are not sure:

    sudo apt-get install -y g++ make libboost-program-options-dev libconfig++-dev libncurses5-dev libncursesw5-dev

After that it's just a matter of compiling. Nothing out of the ordinary. Just run:

    make
    sudo make install

If you want to customize the install location of the binary edit the `Makefile` on lines 27 and 29 (defaults to `/usr/local/bin`).  
If you want to customize the location of the global config run the following command **BEFORE** you compile the program. (Or compile it again afterwards) Be
sure to escape all `/` like this: `\\/`. Also keep the all the `"`s. They are important!

    sed -i 's/"\\/etc\\/mats"/"your\\/path"/g' src/Config.cpp
