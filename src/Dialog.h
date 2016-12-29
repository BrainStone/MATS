#pragma once

#include <algorithm>
#include <string>
#include <vector>

#include "curses++.h"

class Dialog : public NCursesMenu {
private:
	std::string title;
	std::string result;
	NCursesPanel mainPanel;
	std::vector<NCursesPanel> itemPanels;
	NCursesMenuItem** menuItems;

	static void centerString( NCursesPanel& panel, int row, const std::string& text );

	class DialogItem : public NCursesMenuItem {
	private:
		std::string text;
		Dialog& dialog;

	public:
		DialogItem( const std::string& text, Dialog& dialog );

		virtual bool action();
		const std::string& getText() const;
	};

public:
	Dialog( const std::string& title, const std::vector<std::string>& options, const std::string& abortAnswer = "abort" );

	virtual void On_Menu_Init();
	virtual void On_Menu_Termination();
	virtual void On_Item_Init( NCursesMenuItem& item );
	virtual void On_Item_Termination( NCursesMenuItem& item );
	virtual int virtualize( int c );

	const std::string& getResult() const;
};

class DialogOk : public Dialog {
public:
	DialogOk( const std::string& text );
};

class DialogYesNo : public Dialog {
public:
	DialogYesNo( const std::string& text );
};
