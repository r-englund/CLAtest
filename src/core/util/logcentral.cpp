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

#include <inviwo/core/util/logcentral.h>

namespace inviwo {

ConsoleLogger::ConsoleLogger() : Logger() {}
ConsoleLogger::~ConsoleLogger() {}

void ConsoleLogger::log(std::string logSource, unsigned int logLevel, const char* fileName,
    const char* functionName, int lineNumber, std::string logMsg) {
        IVW_UNUSED_PARAM(fileName);
        IVW_UNUSED_PARAM(logLevel);
        IVW_UNUSED_PARAM(functionName);
        IVW_UNUSED_PARAM(lineNumber);
        std::cout << "(" << logSource << ") " << logMsg << std::endl;
}

FileLogger::FileLogger(std::string logPath)
    : Logger()
    , fileStream_(logPath.append("/inviwo-log.html").c_str())
{
    fileStream_ << "<p><font size='+1'>Inviwo (V " << IVW_VERSION << ") Log File</font></p><br>" << std::endl;
    fileStream_ << "<p>" << std::endl;
}

FileLogger::~FileLogger() {
    fileStream_ << "</p>" << std::endl;
    fileStream_.close();
}

void FileLogger::log(std::string logSource, unsigned int logLevel, const char* fileName,
    const char* functionName, int lineNumber, std::string logMsg) {
    IVW_UNUSED_PARAM(fileName);
    IVW_UNUSED_PARAM(logLevel);
    IVW_UNUSED_PARAM(functionName);
    IVW_UNUSED_PARAM(lineNumber);
    switch (logLevel) {
        case inviwo::Info:
            fileStream_ << "<font color='#000000'>Info: ";
            break;
        case inviwo::Warn:
            fileStream_ << "<font color='#FFFF00'>Warn: ";
            break;
        case inviwo::Error:
            fileStream_ << "<font color='#FF0000'>Error: ";
            break;
    }
    fileStream_ << "(" << logSource << ") " << logMsg;
    fileStream_ << "</font><br>" << std::endl;
}

LogCentral::LogCentral() : logLevel_(Info) {
    loggers_ = new std::vector<Logger*>();
}
LogCentral::~LogCentral() {
    delete loggers_;
}

LogCentral* LogCentral::instance() {
    static LogCentral instance_;
    return &instance_;
}

void LogCentral::registerLogger(Logger* logger) {
    loggers_->push_back(logger);
}

void LogCentral::unregisterLogger(Logger* logger) {
    std::vector<Logger*>::iterator it = find(loggers_->begin(), loggers_->end(), logger);
    if (it != loggers_->end()) {
        loggers_->erase(it);
    }
}

void LogCentral::log(std::string logSource, unsigned int logLevel, const char* fileName, const char* functionName, int lineNumber, std::string logMsg) {
    if (logLevel >= logLevel_)
        for (size_t i=0; i<loggers_->size(); i++)
            loggers_->at(i)->log(logSource, logLevel, fileName, functionName, lineNumber, logMsg);
}

} // namespace