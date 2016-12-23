#include "Config.h"

namespace config {
	libconfig::Config config;
	libconfig::Config globalConfig;

	fs::path getConfigDir() {
		char* dirPointer;
		fs::path dir;

		dirPointer = getenv( "XDG_CONFIG_HOME" );

		if ( dirPointer == NULL ) {
			dirPointer = getenv( "HOME" );

			if ( dirPointer == NULL ) {
				dir = getpwuid( getuid() )->pw_dir;
			} else {
				dir = dirPointer;
			}
		} else {
			dir = dirPointer;
		}

		return dir / confDir;
	}

	fs::path getGlobalConfigDir() {
		return "/etc/mats";
	}

	fs::path getMainConf() {
		return getConfigDir() / mainConf;
	}

	fs::path getServersConf() {
		return getConfigDir() / serversConf;
	}

	fs::path getGlobalMainConf() {
		return getGlobalConfigDir() / mainConf;
	}

	void loadConfigs() {
		config.readFile( pathToCString( getMainConf() ) );
		globalConfig.readFile( pathToCString( getGlobalMainConf() ) );
	}

	void safeConfig() {
		config.writeFile( pathToCString( getMainConf() ) );
	}

	void safeGlobalConfig() {
		config.writeFile( pathToCString( getGlobalMainConf()) );
	}

	const char* pathToCString( const fs::path& path ) {
		return path.string().c_str();
	}
}