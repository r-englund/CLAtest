#ifndef IVW_PROPERTYVISIBILITY_H
#define IVW_PROPERTYVISIBILITY_H

namespace inviwo {
    class IVW_CORE_API PropertyVisibility {

    public:
        enum VisibilityMode { 
            DEVELOPMENT =0,
            APPLICATION =1,
            INVISIBLE =2
        };
    };
} //Namespace

#endif //IVW_PROPERTYSEMANTCIS_H