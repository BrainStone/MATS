#include "Main.h"

int main( int argc, char* argv[] ) {
	bool debug;
	int verbosity;

	po::options_description desc( "Options" );
	desc.add_options()
		("debug", "Increase logging verbosity to be able to trace the program. Use this option when reporting bugs.\n(WARNING: This can really spam your logs. So don't use it unless you have to.)")
		("verbose,v", po::value<int>( &verbosity )->default_value( 0 )->implicit_value( 9 ), "Sets the verbosity level (0-9). Sets the level to 9 if no level is specified. Verbosity level is 0 if this option is not specified")
		("help,h", "Displays this help page")
		("version,V", "Displays the version and build time")
		;

	po::variables_map vm;
	po::store( po::parse_command_line( argc, argv, desc ), vm );
	po::notify( vm );

	if ( vm.count( "help" ) ) {
		std::cout << "MATS - Minecraft Administrative Tools for Servers\nVersion: v" VERSION_STRING << std::endl;
		std::cout << "Usage:\n\tmats [options]\n" << std::endl;
		std::cout << desc << std::flush;

		return 0;
	} else if ( vm.count( "version" ) ) {
		std::cout << "MATS - Minecraft Administrative Tools for Servers\nVersion: v" VERSION_STRING "\nBuilt: " __DATE_TIME__ << std::endl;

		return 0;
	}

	config::loadClientConfigs();

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
	LOG( INFO ) << "\t" __DATE_TIME__;
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

		return 1;
	}

	return 0;
}
