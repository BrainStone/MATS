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
	if ( !config::lookupWithDefault( "acceptedEula", false ) ) {
		DialogYesNo dialogAcceptedEula( "Do you agree to the Minecraft EULA? (https://account.mojang.com/documents/minecraft_eula)" );
		dialogAcceptedEula();

		if ( dialogAcceptedEula.getResult() == "Yes" ) {
			config::config.lookup( "acceptedEula" ) = true;

			config::safeConfig();
		} else {
			config::safeConfig();

			DialogOk dialogFailedEula( "You have to agree to the Minecraft EULA!" );
			dialogFailedEula();

			return 1;
		}
	}

	return 0;
}
