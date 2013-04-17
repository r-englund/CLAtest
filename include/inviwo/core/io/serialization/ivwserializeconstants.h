#ifndef IVW_SERIALIZE_CONSTANTS_H
#define IVW_SERIALIZE_CONSTANTS_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <string>

namespace inviwo {

class IVW_CORE_API IvwSerializeConstants {
public:
    static const std::string XML_VERSION;
    static const std::string INVIWO_TREEDATA;
    static const std::string INVIWO_VERSION;
    static const std::string NETWORK_VERSION;
    static const std::string VERSION;
    static const std::string EDIT_COMMENT;
    static const std::string ID_ATTRIBUTE;
    static const std::string REF_ATTRIBUTE;
    static const std::string VERSION_ATTRIBUTE;
    static const std::string CONTENT_ATTRIBUTE;
    static const std::string TYPE_ATTRIBUTE;
    static const std::string COLOR_R_ATTRIBUTE;
    static const std::string COLOR_G_ATTRIBUTE;
    static const std::string COLOR_B_ATTRIBUTE;
    static const std::string COLOR_A_ATTRIBUTE;
    static const std::string VECTOR_X_ATTRIBUTE;
    static const std::string VECTOR_Y_ATTRIBUTE;
    static const std::string VECTOR_Z_ATTRIBUTE;
    static const std::string VECTOR_W_ATTRIBUTE;
    static const unsigned int STRINGSTREAM_PRECISION;

    static const std::string PROPERTY_ATTRIBUTE_1;
    static const std::string PROPERTY_ATTRIBUTE_2;

    static const std::string PROCESSOR_ATTRIBUTE_1;
    static const std::string PROCESSOR_ATTRIBUTE_2;

    static bool isReversvedAttribute(const std::string key);
};

} //namespace
#endif