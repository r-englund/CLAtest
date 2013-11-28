#ifndef IVW_FILEEXTENSION_H
#define IVW_FILEEXTENSION_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API FileExtension { 
public:
    FileExtension();
    FileExtension(std::string extension, std::string description);
    virtual ~FileExtension() {};

    std::string extension_;
    std::string description_;
};

} // namespace

#endif // IVW_FILEEXTENSION_H

