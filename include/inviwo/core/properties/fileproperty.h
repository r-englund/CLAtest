#ifndef IVW_FILEPROPERTY_H
#define IVW_FILEPROPERTY_H

#include "inviwo/core/properties/templateproperty.h"

namespace inviwo {

class FileProperty : public TemplateProperty<std::string> {

public:
    FileProperty(std::string identifier, std::string displayName, std::string value);
};

} // namespace

#endif // IVW_FILEPROPERTY_H