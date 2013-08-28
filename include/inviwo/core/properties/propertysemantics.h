#ifndef IVW_PROPERTYSEMANTCIS_H
#define IVW_PROPERTYSEMANTCIS_H

namespace inviwo {
class IVW_CORE_API PropertySemantics {

public:
    enum Type { Default =0,
                Color =1,
                LightPosition =2,
                Editor=3,
                Collapsible=4,
                Custom};
};
} //Namespace

#endif //IVW_PROPERTYSEMANTCIS_H