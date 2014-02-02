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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_PICKINGCALLBACK_H
#define IVW_PICKINGCALLBACK_H

#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class PickingObject;

class BasePickingCallBack {
public:
    BasePickingCallBack() {}
    virtual ~BasePickingCallBack() {}
    virtual void invoke(const PickingObject*) const=0;
};

template <typename T>
class MemberFunctionPickingCallback : public BasePickingCallBack {
public:
    MemberFunctionPickingCallback() {}
    virtual ~MemberFunctionPickingCallback() {}

    typedef void (T::*fPointerPicking)(const PickingObject*);

    MemberFunctionPickingCallback(T* obj, fPointerPicking functionPtr)
        : functionPtr_(functionPtr)
        , obj_(obj) {}

    void invoke(const PickingObject* p) const {
        if (functionPtr_)(*obj_.*functionPtr_)(p);
    }

private:
    fPointerPicking functionPtr_;
    T* obj_;
};

class PickingCallback {
public:
    PickingCallback() : callBack_(0) {}

    void invoke(const PickingObject* p) const {
        if (callBack_)
            callBack_->invoke(p);
    }

    template <typename T>
    void addMemberFunction(T* o, void (T::*m)(const PickingObject*)) {
        if (callBack_)
            delete callBack_;

        callBack_ = new MemberFunctionPickingCallback<T>(o,m);
    }

private:
    BasePickingCallBack* callBack_;
};


} // namespace

#endif // IVW_PICKINGCALLBACK_H
