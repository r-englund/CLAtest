#include <inviwo/core/util/filedirectory.h>

#include <fstream>

namespace inviwo {

IVW_CORE_API bool fileExists(std::string fileName) {
    std::ifstream inputFile(fileName.c_str());
    return (inputFile != 0);
}

}