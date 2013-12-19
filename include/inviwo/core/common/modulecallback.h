/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_MODULECALLBACK_H
#define IVW_MODULECALLBACK_H

#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API CallbackWithSingleArgument {
public:
    virtual void invoke(const void*) const=0;
};

template <typename T, typename U>
class IVW_CORE_API BaseModuleCallback  : public CallbackWithSingleArgument {
public:
    typedef void (T::*fPointer)(const U*);

    BaseModuleCallback(T* obj, fPointer functionPtr) 
        : functionPtr_(functionPtr)
        , obj_(obj){}

    void invoke(const U* argument) const {
        if(!argument) {
            LogInfo("Callback function argument does not match");
            return;
        }
        if (functionPtr_) (*obj_.*functionPtr_)(argument);
    }

    virtual void invoke(const void* p) const{
	    const U* argument = static_cast<const U*>(p);  
        BaseModuleCallback::invoke(argument);
    }

private:
    fPointer functionPtr_;
    T* obj_;
};

class IVW_CORE_API ModuleCallback {
public:
    ModuleCallback() : callBack_(0) {}

	template <typename U>
    void invoke(const U* p) const{
        if (callBack_)
            callBack_->invoke(p);
    }

    template <typename T, typename U>
    void addMemberFunction(T* o, void (T::*m)(const U*)){
		if( callBack_ ) delete callBack_;
        callBack_ = new BaseModuleCallback<T,U>(o,m);
    }

private:
    CallbackWithSingleArgument* callBack_;
};


} // namespace

#endif // IVW_MODULECALLBACK_H
