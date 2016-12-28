#include <boost/program_options.hpp>
#include <ctime>
#include <sstream>
#include <unistd.h>

#include "Client.h"
#include "Config.h"
#include "Test.h"
#include "Logger.h"

namespace po = boost::program_options;

// Initialize Easylogging++
INITIALIZE_EASYLOGGINGPP;

int main( int argc, char* argv[] ) {
	bool debug;
	int verbosity;

	po::options_description desc( "Allowed options" );
	desc.add_options()
		("help,h", "Displays this help page")
		("version,V", "Displays the version")
		("debug,d", "Use debug logger format")
		("verbose,v", po::value<int>( &verbosity )->default_value( 0 )->implicit_value( 9 ), "Sets the verbosity level (0-9). Sets the level to 9 if no level is specified. Verbosity level is 0 if this option is not specified")
		;

	po::variables_map vm;
	po::store( po::parse_command_line( argc, argv, desc ), vm );
	po::notify( vm );

	if ( vm.count( "help" ) ) {
		std::cout << desc << std::endl;

		return 0;
	} else if ( vm.count( "version" ) ) {
		std::cout << "MATS - Minecraft Administrative Tools for Servers" << "\n"
			<< "Version: v" VERSION_STRING "\nBuilt: " << buildTime() << std::endl;

		return 0;
	}

	config::loadConfigs();

	debug = vm.count( "debug" );
	initLogger( debug, verbosity );

	// daemon( 0, 0 );

	std::stringstream sstream;
	sstream << '\t';

	for ( int i = 0; i < argc; i++ ) {
		sstream << ' ' << argv[i];
	}

	LOG( INFO ) << "Starting \"MATS - Minecraft Administrative Tools for Servers\" version:";
	LOG( INFO ) << "\tv" VERSION_STRING;
	LOG( INFO ) << "Built:";
	LOG( INFO ) << '\t' << buildTime();
	LOG( DEBUG ) << "Parameters:";
	LOG( DEBUG ) << sstream.str();
	LOG( DEBUG ) << "Verbostiy level: " << verbosity;

	if ( debug ) {
		LOG( INFO ) << "Using debug";
	}

	try {
		Client client;
		client();
	} catch ( const NCursesException& e ) {
		LOG( ERROR ) << typeid(e).name() << ": " << e.message;
	}

	return 0;
}