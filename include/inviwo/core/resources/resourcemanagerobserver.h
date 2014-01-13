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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#ifndef IVW_RESOURCE_MANAGER_OBSERVER_H
#define IVW_RESOURCE_MANAGER_OBSERVER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/util/observer.h>
#include <inviwo/core/resources/resource.h>

namespace inviwo {


/** \class ResourceManagerObserver 
*
* Observer for list type objects, such as std::vector.
*
* @see ListObservable
*/
class IVW_CORE_API ResourceManagerObserver: public Observer {
public:
    ResourceManagerObserver(): Observer() {};

    /**
    * This method will be called when an item has been added to the observed object.
    * Override it to add behavior.
    */
    virtual void resourceAdded(const Resource*) {};

    /**
    * This method will be called when an item has been removed from the observed object.
    * Override it to add behavior.
    */
    virtual void resourceRemoved(const Resource*) {};
};


/** \class ListObservable 
*
* Observable for list type objects. Should call notifyResourceAdded as soon as an item 
* has been added to the list and notifyResourceRemoved when an item has been removed. 
*
* @see ListObserver
*/
class IVW_CORE_API ResourceManagerObservable: public Observable<ResourceManagerObserver> {
public:
    ResourceManagerObservable(): Observable<ResourceManagerObserver>() {};


    void notifyResourceAdded(const Resource* item) const {
        // Notify observers
        ObserverSet::iterator endIt = observers_.end();
        for(ObserverSet::iterator it = observers_.begin(); it != endIt; ++it) {
            // static_cast can be used since only template class objects can be added
            static_cast<ResourceManagerObserver*>(*it)->resourceAdded(item);    
        }
    }

    void notifyResourceRemoved(const Resource* item) const {
        // Notify observers
        ObserverSet::iterator endIt = observers_.end();
        for(ObserverSet::iterator it = observers_.begin(); it != endIt; ++it) {
            // static_cast can be used since only template class objects can be added
            static_cast<ResourceManagerObserver*>(*it)->resourceRemoved(item);    
        }
    }
};

} // namespace

#endif // IVW_RESOURCE_MANAGER_OBSERVER_H
