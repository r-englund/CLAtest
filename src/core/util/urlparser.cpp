/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Contact: Rickard Englund
 *
 *********************************************************************************/

#include <inviwo/core/util/urlparser.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <sys/stat.h>


#ifdef _WIN32 
#include <direct.h>
#include <Shlobj.h>
#elif defined(__unix__) 
#include <sys/types.h>
#endif

#ifdef __APPLE__
#include <CoreServices/CoreServices.h>
#endif

namespace inviwo {

std::string URLParser::addBasePath(const std::string url) {
    return InviwoApplication::getPtr()->getBasePath()+url;
}

std::string URLParser::getFileDirectory(const std::string& url) {
    size_t pos = url.find_last_of("\\/") + 1;
    std::string fileDirectory = url.substr(0, pos);
    return fileDirectory;
}

std::string URLParser::getFileNameWithExtension(const std::string& url) {
    size_t pos = url.find_last_of("\\/") + 1;
    std::string fileNameWithExtension = url.substr(pos, url.length());
    return fileNameWithExtension;
}

std::string URLParser::getFileNameWithoutExtension(const std::string& url) {
    std::string fileNameWithExtension = getFileNameWithExtension(url);
    size_t pos = fileNameWithExtension.find_last_of(".");
    std::string fileNameWithoutExtension = fileNameWithExtension.substr(0, pos);
    return fileNameWithoutExtension;
}

std::string URLParser::getFileExtension(const std::string& url) {
    std::string filename = getFileNameWithExtension(url);
    size_t pos = filename.rfind(".");

    if (pos == std::string::npos)
        return "";

    std::string fileExtension = filename.substr(pos+1, url.length());
    return fileExtension;
}

std::string URLParser::replaceFileExtension(const std::string& url, const std::string& newFileExtension) {
    size_t pos = url.find_last_of(".") + 1;
    std::string newUrl = url.substr(0, pos) + newFileExtension;
    return newUrl;
}

std::string URLParser::getRelativePath(const std::string& bPath, const std::string& absolutePath) {
    // FIXME: is the case that the bath path and the absolute path are lying on different drives considered?
    std::string basePath(getFileDirectory(bPath));
    std::string absPath(getFileDirectory(absolutePath));
    std::string fileName(getFileNameWithExtension(absolutePath));
    std::string relativePath("");

    //if given base path is empty use system base path
    if (basePath.empty())
        basePath = InviwoApplication::getPtr()->getBasePath();

    //path as string tokens
    std::vector<std::string> basePathTokens;
    std::vector<std::string> absolutePathTokens;
    size_t pos = 0, pos1 = std::string::npos;

    while (pos != std::string::npos) {
        pos1 = basePath.find_first_of("\\/", pos);

        if (pos1 != pos)
            basePathTokens.push_back(basePath.substr(pos, pos1-pos));

        pos = basePath.find_first_not_of("\\/", pos1);
    }

    pos = 0, pos1 = std::string::npos;

    while (pos != std::string::npos) {
        pos1 = absPath.find_first_of("\\/", pos);

        if (pos1 != pos)
            absolutePathTokens.push_back(absPath.substr(pos, pos1-pos));

        pos = absPath.find_first_not_of("\\/", pos1);
    }

    //discard matching tokens
    for (size_t i=0; (i<basePathTokens.size() && i<absolutePathTokens.size()); i++) {
        if (basePathTokens[i] == absolutePathTokens[i])
            basePathTokens[i] = absolutePathTokens[i] = "";
        else
            break;
    }

    //handle non-matching tokens
    for (size_t i=0; i<basePathTokens.size(); i++)
        if (basePathTokens[i]!="") relativePath+="../";

    for (size_t i=0; i<absolutePathTokens.size(); i++)
        if (absolutePathTokens[i]!="") relativePath+=(absolutePathTokens[i]+"/");

    return relativePath+fileName;
}

bool URLParser::isAbsolutePath(const std::string& path) {
#ifdef WIN32

    if (toupper(path[0])>='A'&&toupper(path[0])<='Z'&&path[1]==':') return true;
    else return false;

#else

    if (path[0]=='/') return true;
    else return false;

#endif
}

bool URLParser::sameDrive(const std::string& absPath1, const std::string absPath2) {
#ifdef WIN32
    // Check if drives are equal and that they are not absolute paths.
    // We only need to check if one of the directories are absolute since the 
    // first comparison will return false otherwise.
    if (toupper(absPath1[0])==toupper(absPath2[0]) && !isAbsolutePath(absPath1)) return true;
    else return false;

#else
    return true;
#endif
}

}