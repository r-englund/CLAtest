/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Daniel Jönsson
 *
 *********************************************************************************/

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

    if (!GetCurrentDirectoryA(sizeof(workingDir), workingDir))
        return "";

#else

    if (!getcwd(workingDir, sizeof(workingDir)))
        return "";

#endif
    return std::string(workingDir);
}


bool directoryExists(const std::string& path) {
    struct stat info;

    if (stat(path.c_str(), &info)) {
        return false; // Cannot access
    } else if (info.st_mode & S_IFDIR)
        return true;
    else
        return false;
}


std::string getParentFolderPath(const std::string& basePath, const std::string& parentFolder) {
    size_t pos = basePath.length();
    std::string fileDirectory = basePath;

    do {
        fileDirectory = fileDirectory.substr(0, pos);
        std::string moduleDirectory = fileDirectory + "/" + parentFolder;
        bool exists = directoryExists(moduleDirectory);

        if (exists)
            return fileDirectory + "/";
    } while ((pos = fileDirectory.find_last_of("\\/")) != std::string::npos);

    return basePath;
}

std::string findBasePath() {
    // Search for directory containing data folder to find application basepath.
    // Working directory will be used if data folder is not found in parent directories.
    std::string basePath = inviwo::filesystem::getParentFolderPath(inviwo::filesystem::getWorkingDirectory(), "data");

    //If we did not find "data" in basepath, check CMake source path.
    if (!directoryExists(basePath + "/modules") && directoryExists(IVW_TRUNK + "/modules"))
        basePath = IVW_TRUNK;

    return basePath;
}

}

}