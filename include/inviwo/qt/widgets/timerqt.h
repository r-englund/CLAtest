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

#ifndef IVW_TIMERQT_H
#define IVW_TIMERQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/core/util/timer.h>
#include <QBasicTimer>
#include <QObject>

namespace inviwo {

//class IVW_QTWIDGETS_API TimerQt: public QTimer, public Timer {
//    Q_OBJECT
//public:
//    TimerQt(): QTimer(), Timer() {};
//    virtual ~TimerQt() { stop();}
//
//    virtual void start(unsigned int intervalInMilliseconds, bool once = false) {
//        if(isActive())
//            return;
//
//        setSingleShot(once);
//
//        connect(this, SIGNAL(timeout()), this, SLOT(onIntervalEventSlot()));
//        
//        QTimer::start(intervalInMilliseconds);
//    };
//
//    virtual void stop() {
//        QTimer::stop();
//    };
//
//public slots:
//    void onIntervalEventSlot() const {
//        Timer::onIntervalEvent();
//    }
//protected:
//
//
//};

class IVW_QTWIDGETS_API TimerQt: public QObject, public Timer {
    Q_OBJECT
public:
    TimerQt(): QObject(), Timer() {};
    virtual ~TimerQt() { stop();}

    virtual void start(unsigned int intervalInMilliseconds, bool once = false) {
        if(timer_.isActive())
            return;
        once_ = once;
        timer_.start(intervalInMilliseconds, this);

    };

    virtual void stop() {
        timer_.stop();
    };

protected:
    void timerEvent(QTimerEvent *event) {
        onIntervalEvent();
        if(once_) {
            stop();
        }
    }
     QBasicTimer timer_;
     bool once_;


};

} // namespace inviwo

#endif // IVW_TIMERQT_H