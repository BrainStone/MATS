#ifndef Client_H
#define Client_H

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
};

#endif // #ifndef Client_H
