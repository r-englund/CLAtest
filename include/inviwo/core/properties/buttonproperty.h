#ifndef IVW_BUTTONPROPERTY_H
#define IVW_BUTTONPROPERTY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/util/callback.h>
#include <inviwo/core/properties/property.h>

namespace inviwo {    
/** class ButtonProperty
*  Property for buttons where you are able to assign functions to the property
*  Its only possible to register 1 function for each property.
*  
* @see ButtonPropertyWidgetQt
*/
class IVW_CORE_API ButtonProperty : public Property {

public:
	ButtonProperty(std::string identifier, std::string displayName,
                   PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT, 
                   PropertySemantics::Type semantics = PropertySemantics::Default);
    

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