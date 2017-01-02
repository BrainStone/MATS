#pragma once

#include <string>

#include "Config.h"
#include "curses++.h"
#include "Dialog.h"
#include "Logger.h"

class ServerItem : public BaseMenuItem {
public:
	ServerItem(const std::string& name);
};

class NewServerItem : public BaseMenuItem {
public:
	NewServerItem();
};

class ServerMenu : public NCursesMenu {
private:
	NCursesMenuItem** items;

public:
	ServerMenu();
};

class Client : public NCursesApplication {
protected:
	virtual int titlesize() const;
	virtual void title();

public:
	Client();

	virtual int run();

private:
	bool eulaCheck();
};
