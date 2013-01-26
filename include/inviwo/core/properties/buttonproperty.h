#ifndef IVW_BUTTONPROPERTY_H
#define IVW_BUTTONPROPERTY_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/properties/templateproperty.h"

namespace inviwo {

	class IVW_CORE_API ButtonProperty : public TemplateProperty<std::string> {
	
	public:
		ButtonProperty(std::string identifier, std::string displayName, std::string);

		virtual void serialize(IvwSerializer& s) const;
		virtual void deserialize(IvwDeserializer& d);
	};

} //namespace

#endif 