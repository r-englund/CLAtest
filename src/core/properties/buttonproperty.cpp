#include <inviwo/core/properties/buttonproperty.h>

namespace inviwo {
ButtonProperty::ButtonProperty(std::string identifier, std::string displayName, TFunctionPointer<ButtonProperty>::Type value)
	: TemplateProperty<TFunctionPointer<ButtonProperty>::Type>(identifier,displayName, value)
	{}

	void ButtonProperty::serialize(inviwo::IvwSerializer &s) const {
		Property::serialize(s);
		s.serialize("value",get());
	}
	
	//void ButtonProperty::deserialize(IvwDeserializer& d) {
	//	Property::deserialize(d);
	//	TFunctionPointer<ButtonProperty>::Type value;
	//	//d.deserialize("value", value);
 //     d.deserializePointer("value",value);
	//	set(value);
	//}

    void ButtonProperty::consolePrinter(){

        std::cout <<"Function pointer working  "<<std::endl;

    }
} //namespace
