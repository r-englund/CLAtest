#ifndef IVW_SETTINGS_H
#define IVW_SETTINGS_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/properties/properties.h>
#include <inviwo/core/properties/propertyowner.h>

namespace inviwo {

    class IVW_CORE_API Settings : public PropertyOwner {

    public:
        Settings();
        virtual ~Settings();

        virtual void initialize();
        virtual void deinitialize();

        void invalidate();
        bool isValid() { return PropertyOwner::isValid(); }
        void setValid() { PropertyOwner::setValid(); }

        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& d);

    protected:
        static const std::string logSource_; ///< Source string to be displayed for log messages.
    };

} // namespace

#endif // IVW_SETTINGS_H
