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

	libconfig::Setting* servers;

	libconfig::Config config;
	libconfig::Config serversConfig;
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

	void loadClientConfigs() {
		if ( !fs::exists( getConfigDir() ) ) {
			fs::create_directories( getConfigDir() );
			fs::permissions( getConfigDir(), fs::perms::owner_all );
		}

		createFileIfNotExists( getMainConf() );
		createFileIfNotExists( getServersConf() );

		config.readFile( getMainConf().string().c_str() );
		serversConfig.readFile( getServersConf().string().c_str() );
		globalConfig.readFile( getGlobalMainConf().string().c_str() );

		verifyServersConfig();

		servers = &serversConfig.lookup( "Servers" );
	}

	void verifyServersConfig() {
		libconfig::Setting* root( &serversConfig.getRoot() );

		verifySetting( root, "acceptedEula", false );

		if ( !serversConfig.exists( "Servers" ) )
			root->add( "Servers", libconfig::Setting::TypeArray );

		safeServersConfig();
	}

	void safeServersConfig() {
		serversConfig.writeFile( getServersConf().string().c_str() );
	}

	template<typename T>
	void verifySetting( libconfig::Setting* root, const std::string& path, T defaultValue ) {
		libconfig::Setting::Type type = typeMapping.at( typeid(T) );

		if ( !root->exists( path ) ) {
			root->add( path, type ) = defaultValue;
		} else if ( root->lookup( path ).getType() != type ) {
			root->remove( path );
			root->add( path, type ) = defaultValue;
		}
	}

	template<typename T>
	T lookupWithDefault( const libconfig::Config& config, const std::string& path, T defaultValue ) {
		if ( !config.lookupValue( path, defaultValue ) ) {
			config.getRoot().add( path, typeMapping.at( typeid(T) ) ) = defaultValue;
		}

		return defaultValue;
	}

	template void verifySetting<bool>( libconfig::Setting* root, const std::string& path, bool defaultValue );
	template void verifySetting<int>( libconfig::Setting* root, const std::string& path, int defaultValue );
	template void verifySetting<long long>( libconfig::Setting* root, const std::string& path, long long defaultValue );
	template void verifySetting<float>( libconfig::Setting* root, const std::string& path, float defaultValue );
	template void verifySetting<const char*>( libconfig::Setting* root, const std::string& path, const char* defaultValue );
	template void verifySetting<std::string>( libconfig::Setting* root, const std::string& path, std::string defaultValue );

	template bool lookupWithDefault<bool>( const libconfig::Config& config, const std::string& path, bool defaultValue );
	template int lookupWithDefault<int>( const libconfig::Config& config, const std::string& path, int defaultValue );
	template long long lookupWithDefault<long long>( const libconfig::Config& config, const std::string& path, long long defaultValue );
	template float lookupWithDefault<float>( const libconfig::Config& config, const std::string& path, float defaultValue );
	template const char* lookupWithDefault<const char*>( const libconfig::Config& config, const std::string& path, const char* defaultValue );
	template std::string lookupWithDefault<std::string>( const libconfig::Config& config, const std::string& path, std::string defaultValue );
}
