#ifndef IVW_SETTINGS_H
#define IVW_SETTINGS_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/properties/properties.h>
#include <inviwo/core/properties/propertyowner.h>

namespace inviwo {

    class IVW_CORE_API Settings : public PropertyOwner {

    public:
        Settings();
        virtual ~Settings();

        virtual void initialize();
        virtual void deinitialize();

        virtual void invalidate();
        virtual bool isValid() { return PropertyOwner::isValid(); }
        virtual void setValid() { PropertyOwner::setValid(); }

        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& d);
    };

} // namespace

#endif // IVW_SETTINGS_H
