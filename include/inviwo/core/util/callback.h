#ifndef IVW_CALLBACK_H
#define IVW_CALLBACK_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>

namespace inviwo {

class IVW_CORE_API BaseCallBack {
public:
    virtual void invoke()=0;
};


template <typename T>
class IVW_CORE_API MemberFunctionCallBack : public BaseCallBack {
public:
    typedef void (T::*fPointerType1)();

    MemberFunctionCallBack(T* obj, fPointerType1 functionPtr) 
        : functionPtr1_(functionPtr)
        , obj_(obj){}

    virtual void invoke() {
        if (functionPtr1_) (*obj_.*functionPtr1_)();
    }

private:
    fPointerType1 functionPtr1_;
    T* obj_;
};


// Example usage
// CallBackList cbList;
// cbList.addMemberFunction(&myClassObject, &MYClassObject::myFunction);

//TODO: Use map that uses string as keys : CallBackMap
class IVW_CORE_API CallBackList {
public:
    typedef std::vector<BaseCallBack*>::iterator cblIt;
    CallBackList() {}
    void invokeAll() { 
        for (cblIt it=callBackList_.begin(); it!=callBackList_.end(); ++it)
            (*it)->invoke();        
    }

    template <typename T>
    void addMemberFunction(T* o, void (T::*m)()) {
        callBackList_.clear();
        callBackList_.push_back(new MemberFunctionCallBack<T>(o, m));
    }

private:
    std::vector<BaseCallBack*> callBackList_;
};

class IVW_CORE_API SingleCallBack {
public:
    SingleCallBack(){}
    void invoke(){
        callBack_->invoke();
    }

    template <typename T>
    void addMemberFunction(T* o, void (T::*m)()){
        callBack_ = new MemberFunctionCallBack<T>(o,m);
    }

private:
    BaseCallBack* callBack_;
};


} // namespace

#endif // IVW_CALLBACK_H
