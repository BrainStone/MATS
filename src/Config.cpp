#include "Config.h"

namespace config {
	const std::map<std::type_index, libconfig::Setting::Type> typeMapping( {
		{ typeid(bool), libconfig::Setting::TypeBoolean },
		{ typeid(int), libconfig::Setting::TypeInt },
		{ typeid(long long), libconfig::Setting::TypeInt64 },
		{ typeid(float), libconfig::Setting::TypeFloat },
		{ typeid(char*), libconfig::Setting::TypeString },
		{ typeid(std::string), libconfig::Setting::TypeString }
	} );

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

	template<typename T>
	T lookupWithDefault( const std::string& path, T defaultValue ) {
		if ( !config.lookupValue( path, defaultValue ) ) {
			config.getRoot().add( path, typeMapping.at( typeid(T) ) ) = defaultValue;
		}

		return defaultValue;
	}

	template bool lookupWithDefault<bool>( const std::string& path, bool defaultValue );
	template int lookupWithDefault<int>( const std::string& path, int defaultValue );
	template long long lookupWithDefault<long long>( const std::string& path, long long defaultValue );
	template float lookupWithDefault<float>( const std::string& path, float defaultValue );
	template const char* lookupWithDefault<const char*>( const std::string& path, const char* defaultValue );
	template std::string lookupWithDefault<std::string>( const std::string& path, std::string defaultValue );
}
