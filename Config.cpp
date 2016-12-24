#include "Config.h"

namespace config {
	libconfig::Config config;
	libconfig::Config globalConfig;

	fs::path getConfigDir() {
		char* dirPointer;

		dirPointer = getenv( "MATS_HOME" );

		if ( dirPointer == NULL ) {
			dirPointer = getenv( "XDG_CONFIG_HOME" );

			if ( dirPointer == NULL ) {
				dirPointer = getenv( "HOME" );

				if ( dirPointer == NULL ) {
					dirPointer = getpwuid( getuid() )->pw_dir;
				}
			}

			return dirPointer / confDir;
		} else {
			return dirPointer;
		}
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

	void createFileIfNotExists( const fs::path& path ) {
		fs::create_directories( path.parent_path() );

		if ( !fs::exists( path ) )
			std::ofstream().open( path );
	}

	void loadConfigs() {
		if ( !fs::exists( getConfigDir() ) ) {
			fs::create_directories( getConfigDir() );
			fs::permissions( getConfigDir(), fs::perms::owner_all );
		}

		createFileIfNotExists( getMainConf() );

		config.readFile( getMainConf().string().c_str() );
		globalConfig.readFile( getGlobalMainConf().string().c_str() );
	}

	void safeConfig() {
		config.writeFile( getMainConf().string().c_str() );
	}

	void safeGlobalConfig() {
		config.writeFile( getGlobalMainConf().string().c_str() );
	}
}
