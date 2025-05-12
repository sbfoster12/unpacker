#include "Logger.hh"

using namespace utils;

//LOGGER

Logger::Logger(std::string logName, int threshold)
    : logName_(logName),
    threshold_(threshold),
    verbosity_(0) {}

void Logger::SetVerbosity(int verbosity) { verbosity_ = verbosity; }

//LOGGERHOLDER

// Define the static member variable instance_
LoggerHolder* LoggerHolder::instance_ = nullptr;

void LoggerHolder::SetVerbosity(int verbosity) {
    verbosity_ = verbosity;
    this->InfoLogger.SetVerbosity(verbosity);
    this->DebugLogger.SetVerbosity(verbosity);
}

