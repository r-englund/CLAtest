#ifndef IVW_DATETIME_H
#define IVW_DATETIME_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>

namespace inviwo {

static const std::string currentDateTime() {
    time_t currentDateTime = time(0);
    struct tm dateTimeStruct;
    char dateTimeBuffer[20];
    dateTimeStruct = *localtime(&currentDateTime);
    strftime(dateTimeBuffer, sizeof(dateTimeBuffer), "%Y%m%d_%H%M%S", &dateTimeStruct);
    return dateTimeBuffer;
}

} // namespace

#endif // IVW_DATETIME_H
