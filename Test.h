#ifndef NCURSES_CPLUS_INTERNAL_H
#define NCURSES_CPLUS_INTERNAL_H 1

#if USE_RCS_IDS
#define MODULE_ID(id) static const char Ident[] = id;
#else
#define MODULE_ID(id) /*nothing*/
#endif

#ifdef __MINGW32__
#include <nc_mingw.h>
#undef KEY_EVENT
#endif

#ifndef _QNX_SOURCE
#include <stdlib.h>
#include <string.h>
#endif

#ifndef CTRL
#define CTRL(x) ((x) & 0x1f)
#endif

#ifndef NULL
#define NULL 0
#endif

#ifndef STRING_HACKS_H
#define STRING_HACKS_H 1

/*
* $Id: nc_string.h,v 1.4 2013/12/15 01:09:19 tom Exp $
*
* String-hacks.  Use these macros to stifle warnings on (presumably) correct
* uses of strcat, strcpy and sprintf.
*
* By the way -
* A fundamental limitation of the interfaces (and frequent issue in bug
* reports using these functions) is that sizes are passed as unsigned values
* (with associated sign-extension problems), limiting their effectiveness
* when checking for buffer overflow.
*/

#ifdef __cplusplus
#define NCURSES_VOID		/* nothing */
#else
#define NCURSES_VOID (void)
#endif

#if USE_STRING_HACKS && HAVE_STRLCAT
#define _nc_STRCAT(d,s,n)	NCURSES_VOID strlcat((d),(s),NCURSES_CAST(size_t,n))
#else
#define _nc_STRCAT(d,s,n)	NCURSES_VOID strcat((d),(s))
#endif

#if USE_STRING_HACKS && HAVE_STRLCPY
#define _nc_STRCPY(d,s,n)	NCURSES_VOID strlcpy((d),(s),NCURSES_CAST(size_t,n))
#else
#define _nc_STRCPY(d,s,n)	NCURSES_VOID strcpy((d),(s))
#endif

#if USE_STRING_HACKS && HAVE_SNPRINTF
#define _nc_SPRINTF             NCURSES_VOID snprintf
#define _nc_SLIMIT(n)           NCURSES_CAST(size_t,n),
#else
#define _nc_SPRINTF             NCURSES_VOID sprintf
#define _nc_SLIMIT(n)		/* nothing */
#endif

#endif /* STRING_HACKS_H */

#endif /* NCURSES_CPLUS_INTERNAL_H */

#include "cursesapp.h"
#include "cursesm.h"
#include "cursesf.h"
#include "Logger.h"

#include <unistd.h>

//
// -------------------------------------------------------------------------
//
class SillyDemo {
public:
	void run( int sleeptime ) {

		NCursesPanel *mystd = new NCursesPanel();

		//  Make a few small demo panels

		NCursesPanel *u = new NCursesPanel( 8, 20, 12, 4 );
		NCursesPanel *v = new NCursesPanel( 8, 20, 10, 6 );
		NCursesPanel *w = new NCursesPanel( 8, 20, 8, 8 );
		NCursesPanel *x = new NCursesPanel( 8, 20, 6, 10 );
		NCursesPanel *y = new NCursesPanel( 8, 20, 4, 12 );
		NCursesPanel *z = new NCursesPanel( 8, 30, 2, 14 );

		//  Draw something on the main screen, so we can see what happens
		//  when panels get moved or deleted.

		mystd->box();
		mystd->move( mystd->height() / 2, 1 );
		mystd->hline( mystd->width() - 2 );
		mystd->move( 1, mystd->width() / 2 );
		mystd->vline( mystd->height() - 2 );
		mystd->addch( 0, mystd->width() / 2, ACS_TTEE );
		mystd->addch( mystd->height() - 1, mystd->width() / 2, ACS_BTEE );
		mystd->addch( mystd->height() / 2, 0, ACS_LTEE );
		mystd->addch( mystd->height() / 2, mystd->width() - 1, ACS_RTEE );
		mystd->addch( mystd->height() / 2, mystd->width() / 2, ACS_PLUS );

		//  Draw frames with titles around panels so that we can see where
		//  the panels are located.
		u->boldframe( "Win U" );
		v->frame( "Win V" );
		w->boldframe( "Win W" );
		x->frame( "Win X" );
		y->boldframe( "Win Y" );
		z->frame( "Win Z" );
		if ( NCursesApplication::getApplication()->useColors() ) {
			u->bkgd( ' ' | COLOR_PAIR( 1 ) );
			w->bkgd( ' ' | COLOR_PAIR( 1 ) );
			y->bkgd( ' ' | COLOR_PAIR( 1 ) );
			v->bkgd( ' ' | COLOR_PAIR( 2 ) );
			x->bkgd( ' ' | COLOR_PAIR( 2 ) );
			z->bkgd( ' ' | COLOR_PAIR( 2 ) );
		}

		//  A refresh to any valid panel updates all panels and refreshes
		//  the screen.  Using mystd is just convenient - We know it's always
		//  valid until the end of the program.

		mystd->refresh();
		sleep( sleeptime );

		//  Show what happens when panels are deleted and moved.

		sleep( sleeptime );
		delete u;
		mystd->refresh();

		sleep( sleeptime );
		delete z;
		mystd->refresh();

		sleep( sleeptime );
		delete v;
		mystd->refresh();

		// show how it looks when a panel moves
		sleep( sleeptime );
		y->mvwin( 5, 30 );
		mystd->refresh();

		sleep( sleeptime );
		delete y;
		mystd->refresh();

		// show how it looks when you raise a panel
		sleep( sleeptime );
		w->top();
		mystd->refresh();

		sleep( sleeptime );
		delete w;
		mystd->refresh();

		sleep( sleeptime );
		delete x;

		mystd->clear();
		mystd->refresh();

		//  Don't forget to clean up the main screen.  Since this is the
		//  last thing using NCursesWindow, this has the effect of
		//  shutting down ncurses and restoring the terminal state.

		sleep( sleeptime );
		delete mystd;
	}
};

class UserData {
private:
	int u;
public:
	UserData( int x ) : u( x ) {}
	int sleeptime() const {
		return u;
	}
};
//
// -------------------------------------------------------------------------
//
template<class T> class MyAction : public NCursesUserItem<T> {
public:
	MyAction( const char* p_name,
			  const T* p_UserData )
		: NCursesUserItem<T>( p_name, static_cast<const char*>(0), p_UserData ) {}

	virtual ~MyAction() {}

	bool action() {
		SillyDemo a;
		a.run( NCursesUserItem<T>::UserData()->sleeptime() );
		return FALSE;
	}
};

template class MyAction<UserData>;
template class NCURSES_IMPEXP NCursesUserItem<UserData>;

class QuitItem : public NCursesMenuItem {
public:
	QuitItem() : NCursesMenuItem( "Quit" ) {}

	bool action() {
		return TRUE;
	}
};
//
// -------------------------------------------------------------------------
//
class Label : public NCursesFormField {
public:
	Label( const char* title,
		   int row, int col )
		: NCursesFormField( 1, static_cast<int>(::strlen( title )), row, col ) {
		set_value( title );
		options_off( O_EDIT | O_ACTIVE );
	}
};
//
// -------------------------------------------------------------------------
//
class MyFieldType : public UserDefinedFieldType {
private:
	int chk;
protected:
	bool field_check( NCursesFormField& f ) {
		(void)f;
		return TRUE;
	}
	bool char_check( int c ) {
		return (c == chk ? TRUE : FALSE);
	}
public:
	MyFieldType( int x ) : chk( x ) {}
};
//
// -------------------------------------------------------------------------
//
class TestForm : public NCursesForm {
private:
	NCursesFormField** F;
	MyFieldType* mft;
	Integer_Field *ift;
	Enumeration_Field *eft;

	static const char *weekdays[];

public:
	TestForm()
		: NCursesForm( 13, 51, (lines() - 15) / 2 - 20, (cols() - 53) / 2 ),
		F( 0 ),
		mft( 0 ),
		ift( 0 ),
		eft( 0 ) {

		F = new NCursesFormField*[10];
		mft = new MyFieldType( 'X' );
		ift = new Integer_Field( 0, 1, 10 );
		eft = new Enumeration_Field( weekdays );

		F[0] = new Label( "Demo Entry Form", 0, 16 );
		F[1] = new Label( "Weekday Enum", 2, 1 );
		F[2] = new Label( "Number(1-10)", 2, 21 );
		F[3] = new Label( "Only 'X'", 2, 35 );
		F[4] = new Label( "Multiline Field (Dynamic and Scrollable)", 5, 1 );
		F[5] = new NCursesFormField( 1, 18, 3, 1 );
		F[6] = new NCursesFormField( 1, 12, 3, 21 );
		F[7] = new NCursesFormField( 1, 12, 3, 35 );
		F[8] = new NCursesFormField( 4, 46, 6, 1, 2 );
		F[9] = new NCursesFormField();

		InitForm( F, TRUE, TRUE );
		boldframe();

		F[5]->set_fieldtype( *eft );
		F[6]->set_fieldtype( *ift );

		F[7]->set_fieldtype( *mft );
		F[7]->set_maximum_growth( 20 ); // max. 20 characters
		F[7]->options_off( O_STATIC );  // make field dynamic

		F[8]->set_maximum_growth( 10 ); // max. 10 lines
		F[8]->options_off( O_STATIC );  // make field dynamic
	}

	TestForm& operator=( const TestForm& rhs ) {
		if ( this != &rhs ) {
			*this = rhs;
		}
		return *this;
	}

	TestForm( const TestForm& rhs )
		: NCursesForm( rhs ), F( 0 ), mft( 0 ), ift( 0 ), eft( 0 ) {}

	~TestForm() {
		delete mft;
		delete ift;
		delete eft;
	}
};

const char* TestForm::weekdays[] = {
	"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
	"Friday", "Saturday", NULL };
//
// -------------------------------------------------------------------------
//
class FormAction : public NCursesMenuItem {
public:
	FormAction( const char *s ) : NCursesMenuItem( s ) {}

	bool action() {
		try {
			Soft_Label_Key_Set* S = new Soft_Label_Key_Set( Soft_Label_Key_Set::PC_Style_With_Index );

			for ( int i = 1; i <= S->labels(); i++ ) {
				char buf[8];
				assert( i < 100 );
				::_nc_SPRINTF( buf, _nc_SLIMIT( sizeof( buf ) ) "Frm%02d", i );
				(*S)[i] = buf;                                      // Text
				(*S)[i] = Soft_Label_Key_Set::Soft_Label_Key::Left; // Justification

			}

			NCursesApplication::getApplication()->push( *S );

			TestForm F;
			F();

			NCursesApplication::getApplication()->pop();
			delete S;

			for ( int i = 0; i < F.count(); i++ ) {
				if ( typeid(*F[i]) == typeid(NCursesFormField) )
					LOG( INFO ) << typeid(*F[i]).name() << ": " << F[i]->value();
			}
		} catch ( const NCursesException& e ) {
			LOG( ERROR ) << typeid(e).name() << ": " << e.message;
		}

		return FALSE;
	}
};
//
// -------------------------------------------------------------------------
//
class PadAction : public NCursesMenuItem {
public:
	PadAction( const char* s ) : NCursesMenuItem( s ) {}

	bool action() {
		const int GRIDSIZE = 3;
		const int PADSIZE = 200;
		unsigned gridcount = 0;

		NCursesPanel mystd;
		NCursesPanel P( mystd.lines() - 2, mystd.cols() - 2, 1, 1 );
		NCursesFramedPad FP( P, PADSIZE, PADSIZE );

		for ( int i = 0; i < PADSIZE; i++ ) {
			for ( int j = 0; j < PADSIZE; j++ ) {
				if ( i % GRIDSIZE == 0 && j % GRIDSIZE == 0 ) {
					if ( i == 0 || j == 0 )
						FP.addch( '+' );
					else
						FP.addch( static_cast<chtype>('A' + (gridcount++ % 26)) );
				} else if ( i % GRIDSIZE == 0 )
					FP.addch( '-' );
				else if ( j % GRIDSIZE == 0 )
					FP.addch( '|' );
				else
					FP.addch( ' ' );
			}
		}

		P.label( "Pad Demo", NULL );
		FP();
		P.clear();
		return FALSE;
	}
};

//
// -------------------------------------------------------------------------
//
class PassiveItem : public NCursesMenuItem {
public:
	PassiveItem( const char* text ) : NCursesMenuItem( text ) {
		options_off( O_SELECTABLE );
	}
};

//
// -------------------------------------------------------------------------
//
class ScanAction : public NCursesMenuItem {
public:
	ScanAction( const char* s ) : NCursesMenuItem( s ) {}

	bool action() {
		NCursesPanel *mystd = new NCursesPanel();

		NCursesPanel *w = new NCursesPanel( mystd->lines() - 2, mystd->cols() - 2, 1, 1 );
		w->box();
		w->refresh();

		NCursesPanel *s = new NCursesPanel( w->lines() - 6, w->cols() - 6, 3, 3 );
		s->scrollok( TRUE );
		::echo();

		s->printw( "Enter decimal integers.  The running total will be shown\n" );
		int nvalue = -1;
		int result = 0;
		while ( nvalue != 0 ) {
			nvalue = 0;
			s->scanw( "%d", &nvalue );
			if ( nvalue != 0 ) {
				s->printw( "%d: ", result += nvalue );
			}
			s->refresh();
		}
		s->printw( "\nPress any key to continue..." );
		s->getch();

		delete s;
		delete w;
		delete mystd;
		::noecho();
		return true;
	}
};

//
// -------------------------------------------------------------------------
//
class MyMenu : public NCursesMenu {
private:
	NCursesPanel* P;
	NCursesMenuItem** I;
	UserData *u;
#define n_items 7

public:
	MyMenu()
		: NCursesMenu( n_items + 10, 16, (lines() - 10) / 2, (cols() - 16) / 2 ),
		P( 0 ), I( 0 ), u( 0 ) {
		u = new UserData( 1 );
		I = new NCursesMenuItem*[1 + n_items];
		I[0] = new PassiveItem( "One" );
		I[1] = new PassiveItem( "Two" );
		I[2] = new MyAction<UserData>( "Silly", u );
		I[3] = new FormAction( "Form" );
		I[4] = new PadAction( "Pad" );
		I[5] = new ScanAction( "Scan" );
		I[6] = new QuitItem();
		I[7] = new NCursesMenuItem(); // Terminating empty item

		InitMenu( I, TRUE, TRUE );

		P = new NCursesPanel( 1, n_items, LINES - 1, 1 );
		frame( "Demo", "Silly" );
		P->show();
	}

	MyMenu& operator=( const MyMenu& rhs ) {
		if ( this != &rhs ) {
			*this = rhs;
		}
		return *this;
	}

	MyMenu( const MyMenu& rhs )
		: NCursesMenu( rhs ), P( 0 ), I( 0 ), u( 0 ) {}

	~MyMenu() {
		P->hide();
		delete P;
		delete u;
	}

	virtual void On_Menu_Init() {
		NCursesWindow W( ::stdscr );
		P->move( 0, 0 );
		P->clrtoeol();
		for ( int i = 1; i <= count(); i++ )
			P->addch( '0' + i );
		P->bkgd( W.getbkgd() );
		refresh();
	}

	virtual void On_Menu_Termination() {
		P->move( 0, 0 );
		P->clrtoeol();
		refresh();
	}

	virtual void On_Item_Init( NCursesMenuItem& item ) {
		P->move( 0, item.index() );
		P->attron( A_REVERSE );
		P->printw( "%1d", 1 + item.index() );
		P->attroff( A_REVERSE );
		refresh();
	}

	virtual void On_Item_Termination( NCursesMenuItem& item ) {
		P->move( 0, item.index() );
		P->attroff( A_REVERSE );
		P->printw( "%1d", 1 + item.index() );
		refresh();
	}
};
//
// -------------------------------------------------------------------------
//
class TestApplication : public NCursesApplication {
protected:
	int titlesize() const {
		return 30;
	}
	void title();
	Soft_Label_Key_Set::Label_Layout useSLKs() const {
		return Soft_Label_Key_Set::PC_Style_With_Index;
	}
	void init_labels( Soft_Label_Key_Set& S ) const;

public:
	TestApplication() : NCursesApplication( TRUE ) {}

	int run();
};

void TestApplication::init_labels( Soft_Label_Key_Set& S ) const {
	for ( int i = 1; i <= S.labels(); i++ ) {
		char buf[8];
		assert( i < 100 );
		::_nc_SPRINTF( buf, _nc_SLIMIT( sizeof( buf ) ) "Key%02d", i );
		S[i] = buf;                                      // Text
		S[i] = Soft_Label_Key_Set::Soft_Label_Key::Left; // Justification
	}
}

void TestApplication::title() {
	const char * const titleText = "Simple C++ Binding Demo";
	const int len = ::strlen( titleText );

	titleWindow->bkgd( screen_titles() );
	titleWindow->addstr( 0, (titleWindow->cols() - len) / 2, titleText );
	titleWindow->noutrefresh();
}


int TestApplication::run() {
	try {
		MyMenu M;
		M();
	} catch ( const NCursesException& e ) {
		LOG( ERROR ) << typeid(e).name() << ": " << e.message;
	}

	return 0;
}

//
// -------------------------------------------------------------------------
//
//static TestApplication *Demo = new TestApplication();