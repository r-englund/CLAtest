#ifndef IVW_FILEDIRECTORY_H
#define IVW_FILEDIRECTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

#include <stdio.h>  // defines FILENAME_MAX
#ifdef WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

namespace inviwo {

class IVW_CORE_API URLParser {
public:
    URLParser() {}

    static std::string addBasePath(const std::string url);
    static bool fileExists(std::string fileName);
    static std::string getFileDirectory(const std::string url);
    static std::string getFileNameWithExtension(const std::string url);
    static std::string getFileNameWithoutExtension(const std::string url);
    static std::string getFileExtension(const std::string url);
    static std::string replaceFileExtension(const std::string url, const std::string newFileExtension);
    static std::string getRelativePath(const std::string& basePath, const std::string absolutePath);
};

} // namespace

#endif // IVW_FILEDIRECTORY_H
