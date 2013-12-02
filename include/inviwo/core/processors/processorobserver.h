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

#ifndef IVW_PROCESSOROBSERVER_H
#define IVW_PROCESSOROBSERVER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/util/observer.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

/** \class ProcessorObserver 
*
* Processor observer that knows which processor did the notification.
* Has notification for begin and end of it's invalidation.
* Inherits from VoidObserver.
*
* @see ProcessorObservable
*/
class IVW_CORE_API ProcessorObserver: public VoidObserver {
public:
    ProcessorObserver(): VoidObserver() {};

    virtual void notify(Processor*) {};
    virtual void notifyInvalidationBegin(Processor*) {};
    virtual void notifyInvalidationEnd(Processor*) {};
};


/** \class ProcessorObservable 
*
* Can call notifyObserversInvalidationBegin and notifyObserversInvalidationEnd
*
* @see ProcessorObserver
*/
class IVW_CORE_API ProcessorObservable: public Observable<ProcessorObserver> {
public:
    ProcessorObservable(): Observable<ProcessorObserver>() {};

    void notifyObservers() const {
        ObserverSet::iterator endIt = observers_->end();
        for(ObserverSet::iterator it = observers_->begin(); it != endIt; ++it) {
            static_cast<ProcessorObserver*>(*it)->notify();    
        }
    }

    void notifyObservers(Processor* p) const {
        ObserverSet::iterator endIt = observers_->end();
        for(ObserverSet::iterator it = observers_->begin(); it != endIt; ++it) {
            static_cast<ProcessorObserver*>(*it)->notify(p);    
        }
    }

    void notifyObserversInvalidationBegin(Processor* p) const {
        ObserverSet::iterator endIt = observers_->end();
        for(ObserverSet::iterator it = observers_->begin(); it != endIt; ++it) {
            static_cast<ProcessorObserver*>(*it)->notifyInvalidationBegin(p);    
        }
    }
    
    void notifyObserversInvalidationEnd(Processor* p) const {
        ObserverSet::iterator endIt = observers_->end();
        for(ObserverSet::iterator it = observers_->begin(); it != endIt; ++it) {
            static_cast<ProcessorObserver*>(*it)->notifyInvalidationEnd(p);    
        }
    }
};

} // namespace

#endif // IVW_PROCESSOROBSERVER_H
