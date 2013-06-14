#include <inviwo/core/util/filedirectory.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <fstream>

namespace inviwo {

std::string UrlParser::addBasePath(const std::string url){
    return InviwoApplication::getPtr()->getBasePath()+url;
}

bool UrlParser::fileExists(std::string fileName) {
    std::ifstream inputFile(fileName.c_str());
    return (inputFile != 0);
}

std::string UrlParser::getFileDirectory(const std::string url) {
    size_t pos = url.find_last_of("\\/") + 1;
    std::string fileDirectory = url.substr(0, pos);
    return fileDirectory;
}

std::string UrlParser::getFileNameWithExtension(const std::string url) {
    size_t pos = url.find_last_of("\\/") + 1;
    std::string fileNameWithExtension = url.substr(pos, url.length());
    return fileNameWithExtension;
}

std::string UrlParser::getFileExtension(const std::string url) {
    size_t pos = url.rfind(".") + 1;
    std::string fileExtension = url.substr( pos, url.length());
    return fileExtension;
}

std::string UrlParser::replaceFileExtension(const std::string url, const std::string newFileExtension) {
    size_t pos = url.find_last_of(".") + 1;
    std::string newUrl = url.substr(0, pos) + newFileExtension;
    return newUrl;
}

}