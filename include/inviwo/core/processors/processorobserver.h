 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file author: Erik Sundén
 *
 *********************************************************************************/

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
    //TODO: Use seperate class for property observation if necessary
    virtual void notifyObserversAboutPropertyChange(Property*) {};
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
        for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it)
            static_cast<ProcessorObserver*>(*it)->notify();
    }

    void notifyObservers(Processor* p) const {
        for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it)
            static_cast<ProcessorObserver*>(*it)->notifyProcessorObserver(p);
    }

    //TODO: Use seperate class for property observation if necessary
    void notifyObserversAboutPropertyChange(Property* p) const {
        for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it)
            static_cast<ProcessorObserver*>(*it)->notifyObserversAboutPropertyChange(p);
    }

    void notifyObserversInvalidationBegin(Processor* p) const {
        for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it)
            static_cast<ProcessorObserver*>(*it)->notifyInvalidationBegin(p);
    }

    void notifyObserversInvalidationEnd(Processor* p) const {
        for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it)
            static_cast<ProcessorObserver*>(*it)->notifyInvalidationEnd(p);
    }

    void notifyObserversRequestEvaluate(Processor* p) const {
        for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend(); ++it)
            static_cast<ProcessorObserver*>(*it)->notifyRequestEvaluate(p);
    }
};

} // namespace

#endif // IVW_PROCESSOROBSERVER_H
