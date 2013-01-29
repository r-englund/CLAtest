#ifndef IVW_BUTTONPROPERTY_H
#define IVW_BUTTONPROPERTY_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/properties/templateproperty.h>

namespace inviwo {

    //Memberfunctionpointer
    template <typename T>
    struct TFunctionPointer
    {
        typedef void (T::*Type)();
    };

	class IVW_CORE_API ButtonProperty : public TemplateProperty<TFunctionPointer<ButtonProperty>::Type> {
	
	public:
		ButtonProperty(std::string identifier, std::string displayName, TFunctionPointer<ButtonProperty>::Type value);
        void consolePrinter();
		virtual void serialize(IvwSerializer& s) const;
		//virtual void deserialize(IvwDeserializer& d);
	};

} //namespace

#endif 