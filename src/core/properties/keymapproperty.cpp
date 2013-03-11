#include <inviwo/core/properties/keymapproperty.h>

namespace inviwo {

    KeyMapProperty::KeyMapProperty(std::string identifier, std::string displayName, PropertySemantics::Type semantics)
        : Property(identifier, displayName, semantics)
    {}

    void KeyMapProperty::serialize(IvwSerializer& s) const {
        Property::serialize(s) ;
    }

    void KeyMapProperty::deserialize(IvwDeserializer& d) {
        Property::deserialize(d) ;
    }

    void KeyMapProperty::setMapper( TrackballKeyMapper* mapper )
    {
        keymapper_ = mapper;
    }

    TrackballKeyMapper* KeyMapProperty::getMapper()
    {
        return keymapper_;
    }




} // namespace
