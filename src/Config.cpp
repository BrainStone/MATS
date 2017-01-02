#include "Config.h"

namespace config {
	const std::map<std::type_index, libconfig::Setting::Type> typeMapping( {
		{ typeid(bool), libconfig::Setting::TypeBoolean },
		{ typeid(int), libconfig::Setting::TypeInt },
		{ typeid(long long), libconfig::Setting::TypeInt64 },
		{ typeid(float), libconfig::Setting::TypeFloat },
		{ typeid(const char*), libconfig::Setting::TypeString },
		{ typeid(std::string), libconfig::Setting::TypeString }
	} );

	libconfig::Setting* servers;

	libconfig::Config config;
	libconfig::Config serversConfig;
	libconfig::Config globalConfig;

	fs::path getConfigDir() {
		char* dirPointer;

		dirPointer = getenv( "MATS_HOME" );

		if ( dirPointer == nullptr ) {
			dirPointer = getenv( "XDG_CONFIG_HOME" );

			if ( dirPointer == nullptr ) {
				dirPointer = getenv( "HOME" );

				if ( dirPointer == nullptr ) {
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
			std::ofstream().open( path.string() );
	}

	void loadClientConfigs() {
		try {
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
			verifyGlobalConfig();

			servers = &serversConfig.lookup( settings::servers::servers );

			Server::initalizeServers();
		} catch ( const libconfig::ParseException& e ) {
			std::cerr << "Error reading config!" << std::endl;
			std::cerr << e.getError() << " in file \"" << e.getFile() << "\" on line " << e.getFile();

			std::exit( 1 );
		} catch ( const libconfig::SettingNotFoundException& e ) {
			std::cerr << "Error reading config!" << std::endl;
			std::cerr << "Could not find setting: \"" << e.getPath() << '"' << std::endl;

			std::exit( 1 );
		} catch ( const libconfig::SettingTypeException& e ) {
			std::cerr << "Error reading config!" << std::endl;
			std::cerr << "Wrong setting type for: \"" << e.getPath() << '"' << std::endl;

			std::exit( 1 );
		}
	}

	void verifyServersConfig() {
		libconfig::Setting& root = serversConfig.getRoot();

		verifySetting( root, settings::servers::accpetedEula, false );

		if ( !serversConfig.exists( settings::servers::servers ) )
			root.add( settings::servers::servers, libconfig::Setting::TypeList );

		libconfig::Setting& servers = serversConfig.lookup( settings::servers::servers );

		for ( int i = servers.getLength() - 1; i >= 0; i-- )
			if ( !verifyServerBlock( servers[i] ) )
				servers.remove( i );

		safeServersConfig();
	}

	bool verifyServerBlock( libconfig::Setting& server ) {
		if ( !server.isGroup() )
			return false;

		verifySetting( server, settings::servers::sever::serverName, "Server" );
		verifySetting( server, settings::servers::sever::serverPath, "~/server" );
		verifySetting( server, settings::servers::sever::jarPath, "minecraft.jar" );
		verifySetting( server, settings::servers::sever::address, "" );
		verifySetting( server, settings::servers::sever::port, 25565 );
		verifySetting( server, settings::servers::sever::slots, 20 );
		verifySetting( server, settings::servers::sever::maxRam, 1024 );
		verifySetting( server, settings::servers::sever::minRam, 0 );
		verifySetting( server, settings::servers::sever::startParams, "" );
		verifySetting( server, settings::servers::sever::javaParams, "" );
		verifySetting( server, settings::servers::sever::autoRestart, 0 );
		verifySetting( server, settings::servers::sever::autoBackup, 0 );

		return true;
	}

	void verifyGlobalConfig() {
		libconfig::Setting& root = globalConfig.getRoot();

		verifySetting( root, settings::global::logPath, "/var/log/mats" );
		verifySetting( root, settings::global::daemonSocket, "unix:/var/run/mats.sock" );
	}

	void safeServersConfig() {
		serversConfig.writeFile( getServersConf().string().c_str() );
	}

	template<typename T>
	void verifySetting( libconfig::Setting& root, const std::string& path, T defaultValue ) {
		libconfig::Setting::Type type = typeMapping.at( typeid(T) );

		if ( !root.exists( path ) ) {
			root.add( path, type ) = defaultValue;
		} else if ( root.lookup( path ).getType() != type ) {
			root.remove( path );
			root.add( path, type ) = defaultValue;
		}
	}

	libconfig::Setting& getServer( size_t index ) {
		return (*servers)[index];
	}

	libconfig::Setting& addServer() {
		libconfig::Setting& server = servers->add( libconfig::Setting::TypeGroup );
		verifyServerBlock( server );

		return server;
	}

	void removeServer( size_t index ) {
		servers->remove( index );
	}

	template void verifySetting<bool>( libconfig::Setting& root, const std::string& path, bool defaultValue );
	template void verifySetting<int>( libconfig::Setting& root, const std::string& path, int defaultValue );
	template void verifySetting<long long>( libconfig::Setting& root, const std::string& path, long long defaultValue );
	template void verifySetting<float>( libconfig::Setting& root, const std::string& path, float defaultValue );
	template void verifySetting<const char*>( libconfig::Setting& root, const std::string& path, const char* defaultValue );
	template void verifySetting<std::string>( libconfig::Setting& root, const std::string& path, std::string defaultValue );
}
