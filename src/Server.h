#pragma once

#include <libconfig.h++>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

#include "Config.h"

class Server {
private:
	typedef std::map<std::string, Server> serversMapType;

	static serversMapType servers;

	static std::string getNextServerName();

public:
	static void initalizeServers();

	static Server& getServer( const std::string& name );
	static Server& addServer();
	static void removeServer( const std::string& name );

	static size_t size();
	static std::vector<std::string> getNames();

private:
	libconfig::Setting& setting;

public:
	Server( libconfig::Setting& setting );

	const std::string& getName() const;
};
