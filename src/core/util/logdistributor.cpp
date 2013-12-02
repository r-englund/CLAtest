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

#include <inviwo/core/util/logdistributor.h>

namespace inviwo {

LogCentral* LogCentral::instance_ = 0;

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


LogCentral::LogCentral() {
    loggers_ = new std::vector<Logger*>();
}
LogCentral::~LogCentral() {
    delete loggers_;
}

LogCentral* LogCentral::instance() {
    if (!instance_) {
        instance_ = new LogCentral();
        instance_->setLogLevel(Info);
        //instance_->registerLogger(new ConsoleLogger());
    }
    return instance_;
}

void LogCentral::registerLogger(Logger* logger) {
    loggers_->push_back(logger);
}

void LogCentral::log(std::string logSource, unsigned int logLevel, const char* fileName, const char* functionName, int lineNumber, std::string logMsg) {
    if (logLevel >= logLevel_)
        for (size_t i=0; i<loggers_->size(); i++)
            loggers_->at(i)->log(logSource, logLevel, fileName, functionName, lineNumber, logMsg);
}

} // namespace