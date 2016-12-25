# MATS
## Minecraft Administrative Tools for Servers

This software is a management system for Minecraft servers on linux systems. It has mainly been designed for servers which run many Minecraft servers, like
hosters or large server networks but can also be used for a few or even a single Minecraft server.

This project is developed using the C++14 standard, Boost and ncurses. So be sure to have them available when compiling (see install instrcutions below). The
software is developed and tested on several Debian 7 and 8 systems. It will likley work on other Linux distributions too however some adjustments might be
needed and furthermore I cannot gurantee that it will work at all. Should you have issues that can be resolved on my side feel free to open an Issue or a Pull
Request.

## Installing

There are no precompiled binaries available so you have to compile it yourself. Just follow these install instructions:

    $ # Install dependencies (Skip this if you have a C++14 compatible compiler, boost, config++ and ncurses installed for compilation)
    $ sudo apt-get install g++ libboost-all-dev libconfig++-dev libncursesw5-dev
    $ git clone https://github.com/BrainStone/MATS
    $ cd MATS
    $ make
    $ sudo make install

After that you should be able to call the program `mats`.
