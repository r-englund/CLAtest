#ifndef IVW_BUTTONPROPERTY_H
#define IVW_BUTTONPROPERTY_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/util/callback.h>
#include <inviwo/core/properties/property.h>

namespace inviwo {    

class IVW_CORE_API ButtonProperty : public Property {

public:
	ButtonProperty(std::string identifier, std::string displayName);
    

    //TODO: Only member functions zero (void) parameter is allowed now.
    // Example usage
    // myButton_.registerClassMemberFunction(this, &MyButton::doSomethingFunction);
    // it is possible to register more than one functions
    template <typename T>
    void registerClassMemberFunction(T* o, void (T::*m)()) {
        callBackList_.addMemberFunction(o, m); 
    }

    //invokes all functions
    void invokeMemberFunctions() {
        callBackList_.invokeAll();
    }

	virtual void serialize(IvwSerializer& s) const;
	virtual void deserialize(IvwDeserializer& d);

private:
    CallBackList callBackList_;
};

} //namespace

#endif //IVW_BUTTONPROPERTY_H