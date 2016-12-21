#ifndef Logger_H
#define Logger_H

// Compile options for easylogging++
//#define ELPP_UNICODE
#define ELIPP_THREAD_SAFE
#define ELPP_DISABLE_PERFORMANCE_TRACKING
#define ELPP_STACKTRACE_ON_CRASH
#define ELPP_NO_DEFAULT_LOG_FILE
#define ELPP_STL_LOGGING

// See https://github.com/easylogging/easyloggingpp for Doc
#include "easylogging++.h"

inline void initLogger( bool debug, int verbosity ) {
	if ( debug ) {
		el::Loggers::configureFromGlobal( "config/logger_debug.conf" );
	} else {
		el::Loggers::configureFromGlobal( "config/logger_default.conf" );
	}

	// Set options
	el::Loggers::addFlag( el::LoggingFlag::NewLineForContainer );
	el::Loggers::addFlag( el::LoggingFlag::LogDetailedCrashReason );
	el::Loggers::addFlag( el::LoggingFlag::DisableApplicationAbortOnFatalLog );
	el::Loggers::addFlag( el::LoggingFlag::ColoredTerminalOutput );
	el::Loggers::setVerboseLevel( verbosity );

	el::Helpers::validateFileRolling( el::Loggers::getLogger("default"), el::Level::Global );
}

#endif // #ifndef Logger_H
