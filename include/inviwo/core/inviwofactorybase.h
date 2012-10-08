#ifndef IVW_INVIWOFACTORYBASE_H
#define IVW_INVIWOFACTORYBASE_H

//#include "inviwo/core/inviwo.h"
#include <string>

namespace inviwo {

    class IvwSerializable;

    class InviwoFactoryBase {

    public:
        ~InviwoFactoryBase() {}

        template <typename T>
        static T* instance() {
            return T::instance();
        }

        virtual void initialize()=0;
        virtual void deinitialize()=0;

        virtual IvwSerializable* create(std::string className) const=0;
        virtual bool isValidType(std::string className) const=0;
    };

} // namespace

#endif // IVW_INVIWOFACETORYBASE_H
