#include "Dialog.h"

void Dialog::centerString( NCursesPanel& panel, int row, const std::string& text ) {
	const size_t size = text.size();

	panel.addstr( row, std::max<int>( (panel.maxx() - size + 1) / 2, 0 ), text.c_str(), size );
}

constexpr chtype Dialog::getActiveColor() {
	return COLOR_PAIR( 6 );
}

constexpr chtype Dialog::getInactiveColor() {
	return COLOR_PAIR( 2 );
}

Dialog::DialogItem::DialogItem( const std::string& text, Dialog& dialog ) :
	NCursesMenuItem( text.c_str() ),
	text( text ),
	dialog( dialog ) {}

bool Dialog::DialogItem::action() {
	dialog.result = text;

	return true;
}

const std::string& Dialog::DialogItem::getText() const {
	return text;
}

Dialog::Dialog( const std::string& title, const std::vector<std::string>& options, const std::string& abortAnswer ) :
	NCursesMenu( 1, 1, cols() / 2, lines() / 2 ),
	title( title ),
	result( abortAnswer ),
	mainPanel( 1, 1 ) {
	const size_t count = options.size();
	size_t i = 0;
	size_t maxOptionLength = 0;
	size_t optionsWidth;
	size_t width;
	size_t pos;
	menuItems = new NCursesMenuItem*[count + 1];
	itemPanels.reserve( count );

	for ( const std::string& option : options ) {
		menuItems[i++] = new DialogItem( option, *this );
		maxOptionLength = std::max( maxOptionLength, option.size() );
	}

	// Terminating empty item
	menuItems[i] = new NCursesMenuItem();
	InitMenu( &menuItems[0], false, true );

	optionsWidth = ((maxOptionLength + 6) * count);
	width = std::max( title.size(), optionsWidth ) + 4;

	//mainPanel.
	mainPanel.wresize( 9, width );
	mainPanel.mvwin( (lines() - 9) / 2, (cols() - width) / 2 );
	centerString( mainPanel, 2, title );
	mainPanel.boldframe();

	pos = (cols() - optionsWidth) / 2 + 1;

	for ( const std::string& option : options ) {
		NCursesPanel* tmpPanel = new NCursesPanel( 3, maxOptionLength + 4, (lines() - 9) / 2 + 4, pos );

		centerString( *tmpPanel, 1, option );
		tmpPanel->frame();
		tmpPanel->bkgd( getInactiveColor() );
		tmpPanel->overlay( mainPanel );

		itemPanels.push_back( *tmpPanel );

		pos += maxOptionLength + 6;
	}
}

void Dialog::On_Menu_Init() {
	mainPanel.show();

	for ( NCursesPanel& panel : itemPanels ) {
		panel.show();
	}

	mainPanel.refresh();
}

void Dialog::On_Menu_Termination() {
	mainPanel.hide();

	for ( NCursesPanel& panel : itemPanels ) {
		panel.hide();
	}
}

void Dialog::On_Item_Init( NCursesMenuItem& item ) {
	itemPanels.at( item.index() ).bkgd( getActiveColor() );
	mainPanel.refresh();
}

void Dialog::On_Item_Termination( NCursesMenuItem& item ) {
	itemPanels.at( item.index() ).bkgd( getInactiveColor() );
	mainPanel.refresh();
}

static constexpr int CMD_QUIT = MAX_COMMAND + 1;
static constexpr int CMD_ACTION = MAX_COMMAND + 2;

int Dialog::virtualize( int c ) {
	switch ( c ) {
		// eXit
	case CTRL( 'X' ):
		return(CMD_QUIT);
		// Next
	case KEY_RIGHT:
	case KEY_DOWN:
	case CTRL( 'N' ):
		return(REQ_NEXT_ITEM);
		// Previous
	case KEY_LEFT:
	case KEY_UP:
	case CTRL( 'P' ):
		return(REQ_PREV_ITEM);
		// Enter
	case CTRL( 'J' ):
	case CTRL( 'M' ):
		return(CMD_ACTION);

	default:
		return c;
	}
}

const std::string& Dialog::getResult() const {
	return result;
}

DialogOk::DialogOk( const std::string& text ) :
	Dialog( text, { "Ok" }, "Ok" ) {}

DialogYesNo::DialogYesNo( const std::string& text ) :
	Dialog( text, { "Yes", "No" }, "No" ) {}
