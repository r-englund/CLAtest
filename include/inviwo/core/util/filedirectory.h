#ifndef IVW_FILEDIRECTORY_H
#define IVW_FILEDIRECTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

IVW_CORE_API bool fileExists(std::string fileName);

class IVW_CORE_API UrlParser {
public:
    UrlParser() {}

    static std::string getFileDirectory(const std::string url) {
        size_t pos = url.find_last_of("\\/") + 1;
        std::string fileDirectory = url.substr(0, pos);
        return fileDirectory;
    }

    static std::string getFileNameWithExtension(const std::string url) {
        size_t pos = url.find_last_of("\\/") + 1;
        std::string fileNameWithExtension = url.substr(pos, url.length());
        return fileNameWithExtension;
    }

    static std::string getFileExtension(const std::string url) {
        size_t pos = url.rfind(".") + 1;
        std::string fileExtension = url.substr( pos, url.length());
        return fileExtension;
    }

    static std::string replaceFileExtension(const std::string url, const std::string newFileExtension) {
        size_t pos = url.find_last_of(".") + 1;
        std::string newUrl = url.substr(0, pos) + newFileExtension;
        return newUrl;
    }
};

} // namespace

#endif // IVW_FILEDIRECTORY_H
