#ifndef IVW_KEYMAPPROPERTY_H
#define IVW_KEYMAPPROPERTY_H

#include <inviwo/core/properties/templateproperty.h>
#include <inviwo/core/interaction/trackballkeymapper.h>

namespace inviwo {

    class IVW_CORE_API KeyMapProperty : public Property {

    public:
        KeyMapProperty(std::string identifier, std::string displayName, PropertySemantics::Type semantics = PropertySemantics::Default);
        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& d);
        void setMapper(TrackballKeyMapper* mapper);
        TrackballKeyMapper* getMapper();

    private:
        TrackballKeyMapper *keymapper_;
    };

} // namespace

#endif // IVW_KEYMAPPROPERTY_H