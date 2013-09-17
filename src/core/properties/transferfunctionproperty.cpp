#include <inviwo/core/properties/transferfunctionproperty.h>

namespace inviwo {

TransferFunctionProperty::TransferFunctionProperty(std::string identifier, std::string displayName, TransferFunction value, PropertyOwner::InvalidationLevel invalidationLevel, PropertySemantics::Type semantics )
    : TemplateProperty<TransferFunction>(identifier, displayName, value, invalidationLevel, semantics)
{}

void TransferFunctionProperty::serialize(IvwSerializer& s) const {
	Property::serialize(s);
	std::stringstream stream;
	s.serialize("size", (int)value_.getNumberOfDataPoints());

	for (int i = 0; i < static_cast<int>(value_.getNumberOfDataPoints()); i++){
		stream << "pos" << i;
		s.serialize(stream.str(), value_.getPoint(i)->getPos());
		stream.clear();
		stream.str(std::string());

		stream << "rgba" << i;
		s.serialize(stream.str(), value_.getPoint(i)->getRgba());
		stream.clear();
		stream.str(std::string());
	}
}

void TransferFunctionProperty::deserialize(IvwDeserializer& d) {
	Property::deserialize(d);
	int size;
	vec2 pos;
	vec4 rgba;
	std::stringstream stream;

	d.deserialize("size", size);
	for (int i = 0; i < size; i++){
		stream << "pos" << i;
		d.deserialize(stream.str(), pos);
		stream.clear();
		stream.str(std::string());

		stream << "rgba" << i;
		d.deserialize(stream.str(), rgba);
		stream.clear();
		stream.str(std::string());

		value_.addPoint(pos, rgba);
	}

    callChanged();
}

void TransferFunctionProperty::customSet(){
	callChanged();
}

} // namespace
