#pragma once

#include <boost/filesystem.hpp>
#include <fstream>
#include <iostream>
#include <libconfig.h++>
#include <map>
#include <pwd.h>
#include <string>
#include <typeindex>
#include <unistd.h>

#include "Server.h"

namespace config {
	namespace fs = boost::filesystem;

	static const fs::path confDir = ".mats/";
	static const fs::path mainConf = "main.cfg";
	static const fs::path serversConf = "servers.cfg";

	extern libconfig::Setting* servers;

	extern libconfig::Config config;
	extern libconfig::Config serversConfig;
	extern libconfig::Config globalConfig;

	fs::path getConfigDir();
	fs::path getGlobalConfigDir();
	fs::path getMainConf();
	fs::path getServersConf();
	fs::path getGlobalMainConf();

	void createFileIfNotExists( const fs::path& path );

	void loadClientConfigs();
	void verifyServersConfig();
	bool verifyServerBlock( libconfig::Setting& server );
	void verifyGlobalConfig();
	void safeServersConfig();

	template<typename T>
	extern void verifySetting( libconfig::Setting& root, const std::string& path, T defaultValue );

	namespace settings {
		typedef const char* label;

		namespace client {
		}

		namespace servers {
			static constexpr label accpetedEula = "acceptedEula";
			static constexpr label servers = "servers";

			namespace sever {
				static constexpr label severName = "severName";
				static constexpr label severPath = "severPath";
				static constexpr label jarPath = "jarPath";
				static constexpr label address = "address";
				static constexpr label port = "port";
				static constexpr label slots = "slots";
				static constexpr label maxRam = "maxRam";
				static constexpr label minRam = "minRam";
				static constexpr label startParams = "startParams";
				static constexpr label javaParams = "javaParams";
				static constexpr label autoRestart = "autoRestart";
				static constexpr label autoBackup = "autoBackup";
			}
		}

		namespace global {
			static constexpr label logPath = "logPath";
			static constexpr label daemonSocket = "daemonSocket";
		}
	}
}
