#ifndef IVW_BUTTONPROPERTY_H
#define IVW_BUTTONPROPERTY_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/util/callback.h>
#include <inviwo/core/properties/property.h>

namespace inviwo {    

class IVW_CORE_API ButtonProperty : public Property {

public:
	ButtonProperty(std::string identifier, std::string displayName, PropertySemantics::Type semantics = PropertySemantics::Default);
    

    //TODO: Only member functions with zero (void) arguments is allowed now.
    // Example usage
    // myButton_.registerClassMemberFunction(this, &MyButton::doSomethingFunction);
    // it is possible to register only one function
    template <typename T>
    void registerClassMemberFunction(T* o, void (T::*m)()) {
        callBack_.addMemberFunction(o,m);
    }

    //invokes all functions
    void invokeMemberFunctions() {
        callBack_.invoke();
    }

	virtual void serialize(IvwSerializer& s) const;
	virtual void deserialize(IvwDeserializer& d);

private:
    SingleCallBack callBack_;
};

} //namespace

#endif //IVW_BUTTONPROPERTY_H