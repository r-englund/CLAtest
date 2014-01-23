/**********************************************************************
 * Copyright (C) 2014 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#include <inviwo/core/util/filesystem.h>
#include <inviwo/core/common/inviwoapplication.h>

// For directory exists
#include <sys/types.h>
#include <sys/stat.h> 

// For working directory
#include <stdio.h> // FILENAME_MAX
#ifdef WIN32
#include <windows.h> 
#include <tchar.h>
#else 
#include <unistd.h>
#endif
#include <stdio.h>  // defines FILENAME_MAX

namespace inviwo {

namespace filesystem {

std::string getWorkingDirectory() {
    char workingDir[FILENAME_MAX];
#ifdef WIN32
    if (!GetCurrentDirectoryA(sizeof(workingDir), workingDir)) {
        return "";
    } 
#else 
    if (!getcwd(workingDir, sizeof(workingDir))) {
        return "";
    } 
#endif
    return std::string(workingDir);

}


bool directoryExists( const std::string& path ) {
    struct stat info;
    if (stat(path.c_str(), &info)) {
        return false; // Cannot access
    } else if (info.st_mode & S_IFDIR) {
        return true;
    } else {
        return false;
    }
}


std::string getParentFolderPath(const std::string& basePath, const std::string& parentFolder) {
    size_t pos = basePath.length();
    std::string fileDirectory = basePath;
    do {
        fileDirectory = fileDirectory.substr(0, pos);
        std::string moduleDirectory = fileDirectory + "/" + parentFolder;
        bool exists = directoryExists(moduleDirectory);
        if (exists) {
            return fileDirectory + "/";
        }

    } while ((pos = fileDirectory.find_last_of("\\/")) != std::string::npos); 
    return basePath;
}

std::string findBasePath() {
    // Search for directory containing data folder to find application basepath. 
    // Working directory will be used if data folder is not found in parent directories.
    std::string basePath = inviwo::filesystem::getParentFolderPath(inviwo::filesystem::getWorkingDirectory(), "data");

    //If we did not find "data" in basepath, check CMake source path.
    if(!directoryExists(basePath + "/data") && directoryExists(IVW_TRUNK + "/data"))
        basePath = IVW_TRUNK;

    return basePath;
}

}

}