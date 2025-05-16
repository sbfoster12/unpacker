#ifndef LOGGER_HH
#define LOGGER_HH

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace utils {

    class Logger {
        public:
            //Constructor
            Logger(std::string logName, int threshold);

            //Destructor
            ~Logger() {};

            void SetVerbosity(int verbosity);

            template <typename T>
            Logger& operator<<(const T& message) {
                if (threshold_ <= verbosity_) {
                    stream_ << message;  // Keep formatting flags
                }
                return *this;
            }

            Logger& operator<<(std::ostream& (*pf)(std::ostream&)) {
                if (threshold_ <= verbosity_) {
                    pf(stream_);  // Apply the manipulator (like std::hex, std::endl)
                    if (pf == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
                        flush();
                    }
                }
                return *this;
            }

            void flush() {
                std::cout << logName_ << ": " << stream_.str();
                stream_.str(""); // Clear the buffer
                stream_.clear(); // Reset any error flags
            }

        private:
            std::string logName_;
            int threshold_;
            int verbosity_;
            std::ostringstream stream_;

    };

    class LoggerHolder {
        public:
        
        static LoggerHolder& getInstance() {
            static LoggerHolder instance_; 
            return instance_;
        }
            
            void SetVerbosity(int verbosity);

            // The loggers
            utils::Logger InfoLogger;
            utils::Logger DebugLogger;
            utils::Logger WarningLogger;

        private:
            // Private constructor to prevent direct instantiation
            LoggerHolder()
                : InfoLogger("Info",1),
                DebugLogger("Debug",2),
                WarningLogger("Warning",0),
                verbosity_(0)
                {}

            // Private static instance
            static utils::LoggerHolder instance_;

            //tree
            int verbosity_;
    };
}

#endif // LOGGERER_HH