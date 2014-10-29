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
 * Contact: Daniel Jönsson
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
#include <direct.h>
#include <Shlobj.h>
#elif defined(__APPLE__)
#include <CoreServices/CoreServices.h>
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


bool fileExists(const std::string& filePath) {
    // http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    struct stat buffer;
    return (stat(filePath.c_str(), &buffer) == 0);
}


bool directoryExists(const std::string& path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0 && (buffer.st_mode & S_IFDIR));
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

void createDirectoryRecursivly(std::string path) {
    replaceInString(path,"\\","/");
    std::vector<std::string> v = splitString(path,'/');

    std::string pathPart;
#ifdef _WIN32
    pathPart += v.front();
    v.erase(v.begin());
#endif

    while(!v.empty()) {
        pathPart += "/" + v.front();
        v.erase(v.begin());
#ifdef _WIN32 
        _mkdir(pathPart.c_str());
#elif defined(__unix__) 
        mkdir(pathPart.c_str(),0755);
#elif defined(__APPLE__)
        mkdir(pathPart.c_str(),0755);
#else
        LogWarnCustom("","createDirectoryRecursivly is not implemented for current system");
#endif
    }
}

// ---------- Helper function to retrieve inviwo settings folder -----------//
#ifdef _WIN32
static std::string helperSHGetKnownFolderPath(const KNOWNFOLDERID &id){
    PWSTR path;
    HRESULT hr = SHGetKnownFolderPath(id,0,NULL,&path);
    std::string s = "";
    if(SUCCEEDED(hr)){
        char ch[1024];
        static const char DefChar = ' ';
        WideCharToMultiByte(CP_ACP,0,path,-1, ch,1024,&DefChar, NULL);
        s =  std::string(ch);
    }else{
        LogErrorCustom("URLParser::getUserSettingsPath","SHGetKnownFolderPath failed to get settings folder");
    }

    CoTaskMemFree(path);
    return s;
}
#endif 

std::string getInviwoUserSettingsPath() {
    std::stringstream ss;
#ifdef _WIN32 
    ss << helperSHGetKnownFolderPath(FOLDERID_RoamingAppData);
    ss << "/Inviwo/";
#elif defined(__unix__) 
    ss << std::getenv("HOME");
    ss << "/.inviwo/";
#elif defined(__APPLE__)
    // Taken from:
    // http://stackoverflow.com/questions/5123361/finding-library-application-support-from-c?rq=1
    // A depricated solution, but a solution...

    FSRef ref;
    OSType folderType = kApplicationSupportFolderType;
    int MAX_PATH = 512;
    char path[PATH_MAX];

    STARTCLANGIGNORE("-Wdeprecated-declarations")
    FSFindFolder( kUserDomain, folderType, kCreateFolder, &ref );
    FSRefMakePath( &ref, (UInt8*)&path, MAX_PATH );
    ENDCLANGIGNORE

    ss << path << "/org.inviwo.network-editor/";
    
#else
    LogWarnCustom("","Get User Setting Path is not implemented for current system");
#endif
    return ss.str();
}

} // end namespace filesystem

} // end namespace inviwo