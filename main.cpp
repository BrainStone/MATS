#include <boost/program_options.hpp>
#include <sstream>

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
		("verbose,v", po::value<int>( &verbosity )->default_value( 0 )->implicit_value( 9 ), "Sets the verbosity level (0-9). Sets the level to 9 if no level is specified. Verbosity level is 0 if this option is not specified")
		("port,p", po::value<short>( &port ), "Sets the port. Default is the config value")
		("ip,i", po::value<string>( &ip ), "Sets the address tio listen to. Default is the config value")
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
}
