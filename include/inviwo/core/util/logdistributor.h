#ifndef IVW_LOGGER_H
#define IVW_LOGGER_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/inviwo.h"
#include <sstream>

namespace inviwo {

IVW_CORE_API enum LogLevel {
    Info,
    Warn,
    Error
};

#define LogInfo(message) \
    {   std::ostringstream stream; stream << message; \
    IVW_CORE_API inviwo::LogCentral::instance()->log(logSource_, inviwo::Info, __FILE__, __FUNCTION__, __LINE__, stream.str());}
#define LogWarn(message) \
    {   std::ostringstream stream; stream << message; \
    IVW_CORE_API inviwo::LogCentral::instance()->log(logSource_, inviwo::Warn, __FILE__, __FUNCTION__, __LINE__, stream.str());}
#define LogError(message) \
    {   std::ostringstream stream; stream << message; \
    inviwo::LogCentral::instance()->log(logSource_, inviwo::Error, __FILE__, __FUNCTION__, __LINE__, stream.str());}

#define LogInfoS(source, message) \
    {   std::ostringstream stream; stream << message; \
    IVW_CORE_API inviwo::LogCentral::instance()->log(source, inviwo::Info, __FILE__, __FUNCTION__, __LINE__, stream.str());}
#define LogWarnS(source, message) \
    {   std::ostringstream stream; stream << message; \
    IVW_CORE_API inviwo::LogCentral::instance()->log(source, inviwo::Warn, __FILE__, __FUNCTION__, __LINE__, stream.str());}
#define LogErrorS(source, message) \
    {   std::ostringstream stream; stream << message; \
    inviwo::LogCentral::instance()->log(source, inviwo::Error, __FILE__, __FUNCTION__, __LINE__, stream.str());}

class IVW_CORE_API Logger {

public:
    Logger() {};
    ~Logger() {};

    virtual void log(std::string logSource, unsigned int logLevel, const char* fileName, const char* functionName, int lineNumber, std::string logMsg) = 0;
};

class IVW_CORE_API ConsoleLogger : public Logger{

public:
    ConsoleLogger();
    ~ConsoleLogger();

    virtual void log(std::string logSource, unsigned int logLevel, const char* fileName, const char* functionName, int lineNumber, std::string logMsg);
};

class IVW_CORE_API LogCentral {

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
