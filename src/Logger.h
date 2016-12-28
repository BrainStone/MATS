#ifndef Logger_H
#define Logger_H

#ifndef VERSION_STRING
#	define VERSION_STRING "UNKNOWN"
#endif

#ifndef VERSION_STRING_SHORT
#	define VERSION_STRING_SHORT "UNKNOWN"
#endif

#ifndef BUILD_TIME
#	define BUILD_TIME 0L
#endif

// Compile options for easylogging++
#define ELPP_DISABLE_PERFORMANCE_TRACKING
#define ELPP_NO_DEFAULT_LOG_FILE
#define ELPP_STACKTRACE_ON_CRASH
#define ELPP_STL_LOGGING
#define ELPP_THREAD_SAFE
#define ELPP_UNICODE

#include <string>
#include <time.h>

#include "Config.h"
// See https://github.com/easylogging/easyloggingpp for Doc
#include "../easyloggingpp/src/easylogging++.h"

void initLogger( bool debug, int verbosity, bool deamon = false );
std::string unixToHumanReadable( long timestamp );
std::string buildTime();

#endif // #ifndef Logger_H
