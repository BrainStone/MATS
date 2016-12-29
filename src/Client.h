#pragma once

#include "Config.h"
#include "curses++.h"
#include "Dialog.h"
#include "Logger.h"

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
