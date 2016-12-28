#include "Client.h"

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

	return 0;
}

bool Client::eulaCheck() {
	LOG( TRACE ) << "Entered EULA check";

	if ( !config::lookupWithDefault( "acceptedEula", false ) ) {
		LOG( INFO ) << "EULA not yet accepted! Asking user to accept it";

		DialogYesNo dialogAcceptedEula( "Do you agree to the Minecraft EULA? (https://account.mojang.com/documents/minecraft_eula)" );
		dialogAcceptedEula();

		LOG( DEBUG ) << "User answered: " << dialogAcceptedEula.getResult();

		if ( dialogAcceptedEula.getResult() == "Yes" ) {
			config::config.lookup( "acceptedEula" ) = true;

			config::safeConfig();
		} else {
			config::safeConfig();

			DialogOk dialogFailedEula( "You have to agree to the Minecraft EULA!" );
			dialogFailedEula();

			LOG( WARNING ) << "EULA check failed!";

			return false;
		}
	}

	LOG( TRACE ) << "EULA check succeeded";

	return true;
}
