#include "Client.h"

ServerItem::ServerItem( const std::string& name ) :
	BaseMenuItem( name ) {}

NewServerItem::NewServerItem() :
	BaseMenuItem( "Add Server" ) {}

ServerMenu::ServerMenu() :
	NCursesMenu( lines(), cols() / 3 ) {
	const size_t size = Server::size();
	size_t i = 0;
	items = new NCursesMenuItem*[size + 3];

	for ( const std::string& serverName : Server::getNames() ) {
		items[i++] = new ServerItem( serverName );
	}

	items[size] = new NewServerItem();
	items[size + 1] = new ExitItem();
	items[size + 2] = new NCursesMenuItem();

	InitMenu( items, false, true );
}

int Client::titlesize() const {
	return 1;
}

void Client::title() {
	const std::string titleText = "MATS - Minecraft Administrative Tools for Servers - v" VERSION_STRING_SHORT;

	titleWindow->bkgd( screen_titles() );
	titleWindow->addstr( 0, (titleWindow->cols() - titleText.size()) / 2, titleText.c_str() );
	titleWindow->noutrefresh();
}

Client::Client() : NCursesApplication( true ) {}

int Client::run() {
	LOG( TRACE ) << "Entered Client application";

	if ( !eulaCheck() )
		return 1;

	ServerMenu menu;
	menu();

	return 0;
}

bool Client::eulaCheck() {
	LOG( TRACE ) << "Entered EULA check";

	libconfig::Setting* settingAcceptedEula = &config::serversConfig.lookup( config::settings::servers::accpetedEula );

	if ( !*settingAcceptedEula ) {
		LOG( INFO ) << "EULA not yet accepted! Asking user to accept it";

		DialogYesNo dialogAcceptedEula( "Do you agree to the Minecraft EULA? (https://account.mojang.com/documents/minecraft_eula)" );
		dialogAcceptedEula();

		LOG( DEBUG ) << "User answered: " << dialogAcceptedEula.getResult();

		if ( dialogAcceptedEula.getResult() == "Yes" ) {
			*settingAcceptedEula = true;

			config::safeServersConfig();
		} else {
			config::safeServersConfig();

			DialogOk dialogFailedEula( "You have to agree to the Minecraft EULA!" );
			dialogFailedEula();

			LOG( WARNING ) << "EULA check failed!";

			return false;
		}
	}

	LOG( TRACE ) << "EULA check succeeded";

	return true;
}
