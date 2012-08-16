#ifndef IVW_LOGGER_H
#define IVW_LOGGER_H

#include "inviwo/core/inviwo.h"

namespace inviwo {

enum LogLevel {
    Info,
    Warn,
    Error
};

#define LogInfo(message) \
    LogCentral::instance()->log(logSource_, Info, __FILE__, __FUNCTION__, __LINE__, (message));
#define LogWarn(message) \
    LogCentral::instance()->log(logSource_, Warn, __FILE__, __FUNCTION__, __LINE__, (message));
#define LogError(message) \
    LogCentral::instance()->log(logSource_, Error, __FILE__, __FUNCTION__, __LINE__, (message));

class Logger {

public:
    Logger() {};
    ~Logger() {};

    virtual void log(std::string logSource, unsigned int logLevel, const char* fileName, const char* functionName, int lineNumber, std::string logMsg) = 0;
};

class ConsoleLogger : public Logger{

public:
    ConsoleLogger();
    ~ConsoleLogger();

    virtual void log(std::string logSource, unsigned int logLevel, const char* fileName, const char* functionName, int lineNumber, std::string logMsg);
};

class LogCentral {

public:
    LogCentral();
    virtual ~LogCentral();

    static LogCentral* instance();

    void setLogLevel(unsigned int logLevel) { logLevel_ = logLevel; }
    unsigned int getLogLevel() { return logLevel_; }

    void registerLogger(Logger* logger);
    void log(std::string logSource, unsigned int logLevel, const char* fileName, const char* functionName, int lineNumber, std::string logMsg);

private:
    unsigned int logLevel_;
    std::vector<Logger*> loggers_;

    static LogCentral* instance_;
};

} // namespace

#endif // IVW_LOGGER_H
