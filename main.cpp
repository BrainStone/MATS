#include <boost/program_options.hpp>
#include <sstream>
#include <unistd.h>

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
		("debug,d", "Use debug logger format")
		("verbose,v", po::value<int>( &verbosity )->default_value( 0 )->implicit_value( 9 ), "Sets the verbosity level (0-9). Sets the level to 9 if no level is specified. Verbosity level is 0 if this option is not specified")
		;

	po::variables_map vm;
	po::store( po::parse_command_line( argc, argv, desc ), vm );
	po::notify( vm );

	if ( vm.count( "help" ) ) {
		std::cout << desc << "\n";
		return 1;
	}

	debug = vm.count( "debug" );
	initLogger( debug, verbosity );

	daemon( 0, 0 );

	std::stringstream sstream;
	sstream << '\t';

	for ( int i = 0; i < argc; i++ ) {
		sstream << ' ' << argv[i];
	}

	LOG( DEBUG ) << "Starting Programm with parameters: ";
	LOG( DEBUG ) << sstream.str();
	LOG( DEBUG ) << "Verbostiy level: " << verbosity;

	if ( debug ) {
		LOG( INFO ) << "Using debug";
	}

	sleep( 10 );

	LOG( INFO ) << "Test after 10 seconds";

	sleep( 10 );

	LOG( INFO ) << "10 more seconds and the end".
}
