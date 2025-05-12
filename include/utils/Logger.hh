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
                    std::ostringstream oss;
                    oss << message;
                    std::cout << oss.str();
                }
                
                return *this;
            }

            Logger& operator<< (std::ostream& (*pf) (std::ostream&)) {
                if (threshold_ <= verbosity_) {
                    std::cout << pf;
                }
                return *this;
            }

        private:
            std::string logName_;
            int threshold_;
            int verbosity_;
    };

    class LoggerHolder {
        public:
        
            static LoggerHolder* getInstance() {
                if (instance_ == nullptr) {
                    instance_ = new LoggerHolder();
                }
                return instance_;
            }
            
            void SetVerbosity(int verbosity);

            // The loggers
            utils::Logger InfoLogger;
            utils::Logger DebugLogger;

        private:
            // Private constructor to prevent direct instantiation
            LoggerHolder()
                : InfoLogger("Info",1),
                DebugLogger("Debug",2),
                verbosity_(0)
                {}

            // Private static instance
            static utils::LoggerHolder* instance_;

            //tree
            int verbosity_;
    };
}

#endif // LOGGERER_HH