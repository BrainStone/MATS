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
	ExitItem();

	virtual bool action();
};

class BaseMenuItem : public NCursesMenuItem {
private:
	static const char* convertString( const std::string& text );

public:
	BaseMenuItem( const std::string& text );
	BaseMenuItem( const char* text );
};

class PassiveItem : public BaseMenuItem {
private:
	void makeUnselectable();

public:
	PassiveItem( const std::string& text );
	PassiveItem( const char* text );
};
