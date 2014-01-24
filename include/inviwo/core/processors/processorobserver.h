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

namespace inviwo {

class Processor;

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

    virtual void notifyProcessorObserver(Processor*) {};
    virtual void notifyInvalidationBegin(Processor*) {};
    virtual void notifyInvalidationEnd(Processor*) {};
    virtual void notifyRequestEvaluate(Processor*) {};
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
        for(ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it) {
            static_cast<ProcessorObserver*>(*it)->notify();    
        }
    }

    void notifyObservers(Processor* p) const {
        for(ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it) {
            static_cast<ProcessorObserver*>(*it)->notifyProcessorObserver(p);    
        }
    }

    void notifyObserversInvalidationBegin(Processor* p) const {
        for(ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it) {
            static_cast<ProcessorObserver*>(*it)->notifyInvalidationBegin(p);    
        }
    }
    
    void notifyObserversInvalidationEnd(Processor* p) const {
        for(ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it) {
            static_cast<ProcessorObserver*>(*it)->notifyInvalidationEnd(p);    
        }
    }

    void notifyObserversRequestEvaluate(Processor* p) const {
        for(ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it) {
            static_cast<ProcessorObserver*>(*it)->notifyRequestEvaluate(p);    
        }
    }
};

} // namespace

#endif // IVW_PROCESSOROBSERVER_H
