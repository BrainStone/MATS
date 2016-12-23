#ifndef cursesPP_H
#define cursesPP_H

#include <curses.h>
#include <cursesapp.h>
#include <cursesf.h>
#include <cursesm.h>
#include <cursesp.h>
#include <cursesw.h>

template<typename T>
constexpr T CTRL( T x ) {
	return x & 0x1f;
}

#endif // #ifndef cursesPP_H
