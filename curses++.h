#ifndef cursesPP_H
#define cursesPP_H

#define NCURSES_WIDECHAR 1

#include <ncursesw/ncurses.h>
#include <ncursesw/cursesapp.h>
#include <ncursesw/cursesf.h>
#include <ncursesw/cursesm.h>
#include <ncursesw/cursesp.h>
#include <ncursesw/cursesw.h>

template<typename T>
constexpr T CTRL( T x ) {
	return x & 0x1f;
}

#endif // #ifndef cursesPP_H
