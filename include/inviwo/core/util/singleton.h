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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_SINGLETON_H
#define IVW_SINGLETON_H

#include <inviwo/core/util/assertion.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <vector>

namespace inviwo {

class SingeltonBase;
class IVW_CORE_API SingeltonBase {
protected:
    static std::vector<SingeltonBase*> instances_;

    void deleteAllSingeltons() {
        while (!instances_.empty()) {
            SingeltonBase* instance = instances_[0];
            instances_.erase(instances_.begin());

            if (instance != this)
                delete instance;
        }
    }

    SingeltonBase() {
        instances_.push_back(this);
    }

    virtual ~SingeltonBase() {
        for (size_t i = 0; i<instances_.size(); i++) {
            if (instances_[i] == this) {
                instances_.erase(instances_.begin()+i);
                break;
            }
        }
    }
};



template <class T>
class Singleton : public SingeltonBase {
public:

    /**
     * For Singletons in the inviwo::core init should be called in
     * void InviwoApplication::initialize(registerModuleFuncPtr regModuleFunc).
     *
     */
    static void init() {
        ivwAssert(instance_==0, "Singleton already initialized.");
        instance_ = new T;
    };

    static void init(T* instance) {
        ivwAssert(instance_==0, "Singleton already initialized.");
        ivwAssert(instance!=0, "Null pointer passed.");
        instance_ = instance;
    };

    static T& getRef() {
        ivwAssert(instance_!=0, "Singleton not initialized. Ensure that init() "
                  "is called in a thread-safe environment.");
        return *instance_;
    };

    static T* getPtr() {
        ivwAssert(instance_!=0, "Singleton not initialized. Ensure that init() "
                  "is called in a thread-safe environment.");
        return instance_;
    };

    static void deleteInstance() {
        delete instance_;
        instance_ = 0;
    }

    Singleton<T>() {};
    virtual ~Singleton() {};

private:
    // no implementation for copy functionalities by making the following two methods
    // unaccessible. Otherwise unwanted Singleton copies might appear.
    Singleton<T>(Singleton<T> const&);
    void operator=(Singleton<T> const&);

    static T* instance_;
};

template <class T> T* Singleton<T>::instance_ = 0;

} // end of namespace

#endif // IVW_SINGLETON_H
