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

#ifndef IVW_TEXTURECALLBACK_H
#define IVW_TEXTURECALLBACK_H

#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class Texture;

class BaseTextureCallBack {
public:
    BaseTextureCallBack(){}
    virtual ~BaseTextureCallBack(){}
    virtual void invoke(Texture*) const=0;
};

template <typename T>
class MemberFunctionTextureCallback : public BaseTextureCallBack {
public:
    typedef void (T::*fPointerTexture)(Texture*);

    MemberFunctionTextureCallback(T* obj, fPointerTexture functionPtr) 
        : functionPtr_(functionPtr)
        , obj_(obj){}

    void invoke(Texture* p) const{
        if (functionPtr_) (*obj_.*functionPtr_)(p);
    }

private:
    fPointerTexture functionPtr_;
    T* obj_;
};

class TextureCallback {
public:
    TextureCallback() : callBack_(0) {}
    virtual ~TextureCallback(){
        delete callBack_;
    }

    void invoke(Texture* p) const{
        if (callBack_)
            callBack_->invoke(p);
    }

    template <typename T>
    void addMemberFunction(T* o, void (T::*m)(Texture*)){
        callBack_ = new MemberFunctionTextureCallback<T>(o,m);
    }

private:
    BaseTextureCallBack* callBack_;
};


} // namespace

#endif // IVW_TEXTURECALLBACK_H, settingsWidget_(0)
