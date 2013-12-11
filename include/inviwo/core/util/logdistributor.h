/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_LOGGER_H
#define IVW_LOGGER_H

#pragma warning (disable : 4231)

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <sstream>
#include <typeinfo>
#include <inviwo/core/util/stringconversion.h>

namespace inviwo {

IVW_CORE_API enum LogLevel {
    Info,
    Warn,
    Error
};

#define LogInfo(message) \
    {   std::ostringstream stream__; stream__ << message; \
    inviwo::LogCentral::instance()->log(parseTypeIdName(std::string(typeid(this).name())), inviwo::Info, __FILE__, __FUNCTION__, __LINE__, stream__.str());}
#define LogWarn(message) \
    {   std::ostringstream stream__; stream__ << message; \
    inviwo::LogCentral::instance()->log(parseTypeIdName(std::string(typeid(this).name())), inviwo::Warn, __FILE__, __FUNCTION__, __LINE__, stream__.str());}
#define LogError(message) \
    {   std::ostringstream stream__; stream__ << message; \
    inviwo::LogCentral::instance()->log(parseTypeIdName(std::string(typeid(this).name())), inviwo::Error, __FILE__, __FUNCTION__, __LINE__, stream__.str());}

#define LogInfoCustom(source, message) \
    {   std::ostringstream stream__; stream__ << message; \
    inviwo::LogCentral::instance()->log(source, inviwo::Info, __FILE__, __FUNCTION__, __LINE__, stream__.str());}
#define LogWarnCustom(source, message) \
    {   std::ostringstream stream__; stream__ << message; \
    inviwo::LogCentral::instance()->log(source, inviwo::Warn, __FILE__, __FUNCTION__, __LINE__, stream__.str());}
#define LogErrorCustom(source, message) \
    {   std::ostringstream stream__; stream__ << message; \
    inviwo::LogCentral::instance()->log(source, inviwo::Error, __FILE__, __FUNCTION__, __LINE__, stream__.str());}

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
    void unregisterLogger(Logger* logger);
    void log(std::string logSource, unsigned int logLevel, const char* fileName, const char* functionName, int lineNumber, std::string logMsg);

private:
    unsigned int logLevel_;
    std::vector<Logger*>* loggers_;

    static LogCentral* instance_;
};

} // namespace

#endif // IVW_LOGGER_H
