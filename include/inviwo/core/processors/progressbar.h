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

#ifndef IVW_PROGRESSBAR_H
#define IVW_PROGRESSBAR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/util/observer.h>

namespace inviwo {


/** \class ProgressBarObserver
*
* Observer for ProgressBar
*
* @see ProgressBar
* @see ProgressBarObservable
*/
class IVW_CORE_API ProgressBarObserver: public Observer {
public:
    ProgressBarObserver(): Observer() {};

    /**
    * This method will be called when observed object changes.
    * Override it to add behavior.
    */
    virtual void progressChanged() {};

    /**
    * This method will be called when observed object changes.
    * Override it to add behavior.
    */
    virtual void progressBarVisibilityChanged() {};
};


/** \class ProgressBarObservable
*
* Observable for ProgressBar.
*
* @see ProgressBar
* @see ProgressBarObserver
*/
class IVW_CORE_API ProgressBarObservable: public Observable<ProgressBarObserver> {
public:
    ProgressBarObservable(): Observable<ProgressBarObserver>() {};

    void notifyProgressChanged() const {
        // Notify observers
        ObserverSet::iterator endIt = observers_->end();

        for (ObserverSet::iterator it = observers_->begin(); it != endIt; ++it) {
            // static_cast can be used since only template class objects can be added
            static_cast<ProgressBarObserver*>(*it)->progressChanged();
        }
    }

    void notifyVisibilityChanged() const {
        // Notify observers
        ObserverSet::iterator endIt = observers_->end();

        for (ObserverSet::iterator it = observers_->begin(); it != endIt; ++it) {
            // static_cast can be used since only template class objects can be added
            static_cast<ProgressBarObserver*>(*it)->progressBarVisibilityChanged();
        }
    }
};

/** \class ProgressBar
*
* Simple progressbar to be used in a ProgressBarOwner.
*
* @note Use ProgressBarOwner when using it for a Processor
* @see ProgressBarOwner
* @see ProgressBarObserver
*/
class IVW_CORE_API ProgressBar : public IvwSerializable, ProgressBarObservable {

public:
    ProgressBar();
    virtual ~ProgressBar();

    float getProgress() const { return progress_; }
    void resetProgress() { progress_ = 0.0f; notifyProgressChanged(); }
    void finishProgress() { progress_ = 1.0f; notifyProgressChanged(); }

    void updateProgress(float progress);
    void updateProgressLoop(size_t loopVar, size_t maxLoopVar, float endProgress);

    void show() { visible_ = true; notifyVisibilityChanged(); }
    void hide() { visible_ = false; notifyVisibilityChanged(); }
    bool isVisible() const { return visible_; }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

private:
    float progress_;
    float beginLoopProgress_;
    bool visible_;

};

} // namespace

#endif // IVW_PROGRESSBAR_H
