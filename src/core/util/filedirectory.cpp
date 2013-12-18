/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/util/filedirectory.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <fstream>

namespace inviwo {

std::string URLParser::addBasePath(const std::string url){
    return InviwoApplication::getPtr()->getBasePath()+url;
}

bool URLParser::fileExists(std::string fileName) {
    std::ifstream inputFile(fileName.c_str());
    return static_cast<bool>(inputFile);
}

std::string URLParser::getFileDirectory(const std::string url) {
    size_t pos = url.find_last_of("\\/") + 1;
    std::string fileDirectory = url.substr(0, pos);
    return fileDirectory;
}

std::string URLParser::getFileNameWithExtension(const std::string url) {
    size_t pos = url.find_last_of("\\/") + 1;
    std::string fileNameWithExtension = url.substr(pos, url.length());
    return fileNameWithExtension;
}

std::string URLParser::getFileNameWithoutExtension(const std::string url) {    
    std::string fileNameWithExtension = getFileNameWithExtension(url);
    size_t pos = fileNameWithExtension.find_last_of(".");
    std::string fileNameWithoutExtension = fileNameWithExtension.substr(0, pos);
    return fileNameWithoutExtension;
}

std::string URLParser::getFileExtension(const std::string url) {
    size_t pos = url.rfind(".") + 1;
    std::string fileExtension = url.substr( pos, url.length());
    return fileExtension;
}

std::string URLParser::replaceFileExtension(const std::string url, const std::string newFileExtension) {
    size_t pos = url.find_last_of(".") + 1;
    std::string newUrl = url.substr(0, pos) + newFileExtension;
    return newUrl;
}

std::string URLParser::getRelativePath(const std::string& bPath, const std::string absolutePath) {
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
    while(pos != std::string::npos) {
        pos1 = basePath.find_first_of("\\/", pos);
        if(pos1 != pos)            
            basePathTokens.push_back(basePath.substr(pos, pos1-pos));        
        pos = basePath.find_first_not_of("\\/", pos1);
    }

    pos = 0, pos1 = std::string::npos;
    while(pos != std::string::npos) {
        pos1 = absPath.find_first_of("\\/", pos);
        if(pos1 != pos)            
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

}