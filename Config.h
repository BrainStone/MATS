#ifndef Config_H
#define Config_H

#include <experimental/filesystem>
#include <fstream>
#include <libconfig.h++>
#include <pwd.h>
#include <string>
#include <unistd.h>

namespace config {
	namespace fs = std::experimental::filesystem;

	static const fs::path confDir = ".mats/";
	static const fs::path mainConf = "main.cfg";
	static const fs::path serversConf = "servers.cfg";

	extern libconfig::Config config;
	extern libconfig::Config globalConfig;

	fs::path getConfigDir();
	fs::path getGlobalConfigDir();
	fs::path getMainConf();
	fs::path getServersConf();
	fs::path getGlobalMainConf();

	void createFileIfNotExists( const fs::path& path );

	void loadConfigs();
	void safeConfig();
	void safeGlobalConfig();
}

#endif // #ifndef Config_H
