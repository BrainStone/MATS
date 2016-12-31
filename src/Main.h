#pragma once

#include <boost/program_options.hpp>
#include <ctime>
#include <sstream>
#include <unistd.h>

#include "Client.h"
#include "Config.h"
#include "Logger.h"

namespace po = boost::program_options;

int main( int argc, char* argv[] );
