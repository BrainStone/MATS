#include "curses++.h"

ExitItem::ExitItem() :
	NCursesMenuItem( "Exit" ) {}

bool ExitItem::action() {
	return true;
}

const char* BaseMenuItem::convertString( const std::string& text ) {
	char* str = new char[text.size() + 1];
	text.copy( str, text.size() );

	return str;
}

BaseMenuItem::BaseMenuItem( const std::string& text ) :
	BaseMenuItem( convertString( text ) ) {}

BaseMenuItem::BaseMenuItem( const char* text ) :
	NCursesMenuItem( text ) {}

void PassiveItem::makeUnselectable() {
	options_off( O_SELECTABLE );
}

PassiveItem::PassiveItem( const std::string& text ) :
	BaseMenuItem( text ) {
	makeUnselectable();
}

PassiveItem::PassiveItem( const char* text ) :
	BaseMenuItem( text ) {
	makeUnselectable();
}
