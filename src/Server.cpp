#include "Server.h"

Server::serversMapType Server::servers;

std::string Server::getNextServerName() {
	serversMapType::iterator end = servers.end();
	const std::string baseName = "Server";

	if ( servers.find( baseName ) == end ) {
		return baseName;
	} else {
		size_t i = 0;
		std::string name;

		do {
			name = baseName + std::to_string( ++i );
		} while ( servers.find( name ) != end );

		return name;
	}
}

void Server::initalizeServers() {
	if ( !servers.empty() )
		throw std::runtime_error( "Server list already initialized!" );

	std::string name;
	serversMapType::iterator end = servers.end();

	for ( libconfig::Setting& server : *config::servers ) {
		name = server.lookup( config::settings::servers::sever::serverName ).operator std::string();

		if ( servers.find( name ) != end )
			throw std::runtime_error( "Duplicate server \"" + name + "\"!" );

		servers.insert( serversMapType::value_type( name, server ) );
	}
}

Server& Server::getServer( const std::string& name ) {
	return servers.at( name );
}

Server& Server::addServer() {
	libconfig::Setting& newServer = config::servers->add( libconfig::Setting::TypeGroup );
	std::string newName = getNextServerName();

	config::verifyServerBlock( newServer );
	newServer.lookup( config::settings::servers::sever::serverName ) = newName;

	config::safeServersConfig();

	return servers.insert( serversMapType::value_type( newName, newServer ) ).first->second;
}

void Server::removeServer( const std::string& name ) {
	config::servers->remove( servers.at( name ).setting.getIndex() );

	config::safeServersConfig();
}

size_t Server::size() {
	return servers.size();
}

std::vector<std::string> Server::getNames() {
	std::vector<std::string> names;
	names.reserve( size() );

	for ( const serversMapType::value_type& server : servers ) {
		names.push_back( server.first );
	}

	LOG( INFO ) << servers;
	LOG( INFO ) << names;

	return names;
}

Server::Server( libconfig::Setting& setting ) :
	setting( setting ) {}

const std::string& Server::getName() const {
	return setting.lookup( config::settings::servers::sever::serverName );
}
