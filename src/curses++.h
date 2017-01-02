#pragma once

#define NCURSES_WIDECHAR 1

#include <ncursesw/ncurses.h>
#include <ncursesw/cursesapp.h>
#include <ncursesw/cursesf.h>
#include <ncursesw/cursesm.h>
#include <ncursesw/cursesp.h>
#include <ncursesw/cursesw.h>
#include <string>

template<typename T>
constexpr T CTRL( T x ) {
	return x & 0x1f;
}

class ExitItem : public NCursesMenuItem {
public:
	ExitItem() :
		NCursesMenuItem( "Exit" ) {}

	virtual bool action() {
		return true;
	}
};

class BaseMenuItem : public NCursesMenuItem {
private:
	static inline const char* convertString( const std::string& text ) {
		char* str = new char[text.size() + 1];
		text.copy( str, text.size() );

		return str;
	}

public:
	BaseMenuItem( const std::string& text ) :
		NCursesMenuItem( convertString( text ) ) {}

	BaseMenuItem( const char* text ) :
		BaseMenuItem( std::string( text ) ) {}
};

class PassiveItem : public BaseMenuItem {
public:
	PassiveItem( const std::string& text ) :
		BaseMenuItem( text ) {
		options_off( O_SELECTABLE );
	}

	PassiveItem( const char* text ) :
		PassiveItem( std::string( text ) ) {}
};
