#ifndef Logger_H
#define Logger_H

// Compile options for easylogging++
//#define ELPP_UNICODE
#define ELIPP_THREAD_SAFE
#define ELPP_DISABLE_PERFORMANCE_TRACKING
#define ELPP_STACKTRACE_ON_CRASH
#define ELPP_NO_DEFAULT_LOG_FILE
#define ELPP_STL_LOGGING

#include <string>

// See https://github.com/easylogging/easyloggingpp for Doc
#include "easylogging++.h"

inline void initLogger( bool debug, int verbosity ) {
	const std::string formatDefault = "%datetime [%level %vlevel] %msg";
	const std::string formatVerbose = "%datetime [%level %vlevel] %msg";
	const std::string formatDetailed = "%datetime [%level] %fbase: %line [%func]: %msg";

	el::Configurations defaultConf;
	defaultConf.setToDefault();

	defaultConf.setGlobally( el::ConfigurationType::Format, formatDefault );
	defaultConf.set( el::Level::Verbose, el::ConfigurationType::Format, formatVerbose );
	defaultConf.set( el::Level::Debug, el::ConfigurationType::Format, formatDetailed );
	defaultConf.set( el::Level::Warning, el::ConfigurationType::Format, formatDetailed );
	defaultConf.set( el::Level::Error, el::ConfigurationType::Format, formatDetailed );
	defaultConf.set( el::Level::Fatal, el::ConfigurationType::Format, formatDetailed );

	defaultConf.setGlobally( el::ConfigurationType::Filename, "log/log.log" );

	defaultConf.setGlobally( el::ConfigurationType::Enabled, true );
	defaultConf.set( el::Level::Trace, el::ConfigurationType::Enabled, debug );
	defaultConf.setGlobally( el::ConfigurationType::ToFile, true );
	defaultConf.setGlobally( el::ConfigurationType::ToStandardOutput, false );
	defaultConf.setGlobally( el::ConfigurationType::MillisecondsWidth, debug ? 6 : 3 );
	defaultConf.setGlobally( el::ConfigurationType::PerformanceTracking, debug );
	defaultConf.setGlobally( el::ConfigurationType::MaxLogFileSize, debug ? 1 * 1024 * 1024 * 1024 : 128 * 1024 * 1024 );

	// Set options
	el::Loggers::reconfigureLogger( "default", defaultConf );
	el::Loggers::addFlag( el::LoggingFlag::NewLineForContainer );
	el::Loggers::addFlag( el::LoggingFlag::LogDetailedCrashReason );
	el::Loggers::addFlag( el::LoggingFlag::DisableApplicationAbortOnFatalLog );
	el::Loggers::addFlag( el::LoggingFlag::ColoredTerminalOutput );
	el::Loggers::setVerboseLevel( verbosity );

	el::Helpers::validateFileRolling( el::Loggers::getLogger( "default" ), el::Level::Global );
}

#endif // #ifndef Logger_H
