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

#ifndef IVW_PICKINGMANAGER_H
#define IVW_PICKINGMANAGER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/pickingobject.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {
/** \class PickingManager
* Manager for picking objects.
*/

class FindPickingObject {
public:
    FindPickingObject(const DataVec3UINT8::type& c) : color_(c) {}

    bool operator()(PickingObject* obj) {
        return obj->getPickingColorAsUINT8() == color_;
    }
private:
    DataVec3UINT8::type color_;
};

class IVW_CORE_API PickingManager {

    friend class PickingContainer;

public:
    static PickingManager* instance() {
        static PickingManager instance;// Guaranteed to be destroyed. Instantiated on first use.
        return &instance;
    }
    ~PickingManager();

    template <typename T>
    const PickingObject* registerPickingCallback(T* o, void (T::*m)(const PickingObject*), bool readDepth = true) {
        PickingObject* pickObj;

        if (unRegisteredPickingObjects_.empty()) {
            pickObj = generatePickingObject(pickingObjects_.size());
            pickingObjects_.push_back(pickObj);
        }
        else {
            pickObj = unRegisteredPickingObjects_.back();
            unRegisteredPickingObjects_.pop_back();
        }

        pickObj->getCallbackContainer()->addMemberFunction(o,m);
        pickObj->setReadDepth(readDepth);
        return pickObj;
    }

    bool unregisterPickingObject(const PickingObject*);

protected:
    PickingManager() {};
    PickingManager(PickingManager const&) {};
    void operator=(PickingManager const&) {};

    PickingObject* generatePickingObject(size_t);

    PickingObject* getPickingObjectFromColor(const DataVec3UINT8::type&);

    void performUniqueColorGenerationTest(int iterations);

private:
    std::vector<PickingObject*> pickingObjects_;
    std::vector<PickingObject*> unRegisteredPickingObjects_;
};

} // namespace

#endif // IVW_PICKINGMANAGER_H