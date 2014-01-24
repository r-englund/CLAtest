/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
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

#include <inviwo/core/util/observer.h>
#include <inviwo/core/util/assertion.h>

namespace inviwo
{

////////////////////////////// Observer //////////////////////////////////////////// 

Observer::Observer() {
	observables_ = new ObservableSet();
}

Observer::~Observer() {
	removeObservations();
	delete observables_;
}

void Observer::removeObservation(ObservableInterface* observable) {
    ObservableSet::iterator it = observables_->find(observable);
    // Remove from list and observed object if observing it
    if (it != observables_->end()) {
        observables_->erase(it);
        // Remove from observable
        observable->removeObserver(this);
    }
}

void Observer::removeObservations() {
    while (!observables_->empty()) {
        removeObservation(*observables_->begin());
    }
}

void Observer::addObservation(ObservableInterface* observed) {
    ivwAssert(observed!=NULL, "Tried to add null Observable");

	std::pair<ObservableSet::iterator, bool> inserted = observables_->insert(observed);
	if (inserted.second) {
		observed->addObserver(this); 
	}
}

////////////////////////////// ObservableInterface //////////////////////////////////////////// 

ObservableInterface::ObservableInterface() {
    observers_ = new ObserverSet();
}

ObservableInterface::~ObservableInterface() {
    removeObservers();
    delete observers_;
}
void ObservableInterface::addObserver(Observer* observer) {
    ivwAssert(observer!=NULL, "Tried to add null Observer");

    std::pair<ObserverSet::iterator, bool> inserted = observers_->insert(observer);
    if (inserted.second) {
        observer->addObservation(this); 
    }
}

void ObservableInterface::removeObserver(Observer* observer) {
    ObserverSet::iterator it = observers_->find(observer);
    // Remove from list and observer if observed by it
    if (it != observers_->end()) {
        observers_->erase(it);
        // Remove from observer
        observer->removeObservation(this);
    }
}

void ObservableInterface::removeObservers() {
    while (!observers_->empty()) {
        removeObserver(*observers_->begin());
    }
}


} // namespace

