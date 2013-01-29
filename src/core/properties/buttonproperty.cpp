#include <inviwo/core/properties/buttonproperty.h>

namespace inviwo {

ButtonProperty::ButtonProperty(std::string identifier, std::string displayName, std::string value)
	: TemplateProperty<std::string>(identifier,displayName, value)
	{}

	void ButtonProperty::serialize(inviwo::IvwSerializer &s) const {
		Property::serialize(s);
		s.serialize("value",get());
	}
	
	void ButtonProperty::deserialize(IvwDeserializer& d) {
		Property::deserialize(d) ;
		std::string value;
		d.deserialize("value", value);
		set(value);
	}
} //namespace
