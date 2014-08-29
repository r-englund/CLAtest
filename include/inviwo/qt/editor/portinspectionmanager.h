/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file authors: Peter Steneteg
 *
 *********************************************************************************/

#ifndef IVW_PORTINSPECTIONMANAGER_H
#define IVW_PORTINSPECTIONMANAGER_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/core/util/timer.h>
#include <inviwo/qt/editor/portinfowidgetqt.h>

namespace inviwo {

class Timer;
class PortInfoWidgetQt;
class EditorGrapicsItem;

class IVW_QTEDITOR_API PortInspectionManager {
public:
    PortInspectionManager(EditorGrapicsItem* item);
    virtual ~PortInspectionManager();

    void startTimer();
    void startTimer(QPoint pos);
    void showPortInfo();
    void hidePortInfo();

protected:
    virtual void executeHoverTimer();

private:
    EditorGrapicsItem* item_;           //< Non-owning reference
    Timer* hoverTimer_;
    float hoverDelay_;
    PortInfoWidgetQt* portInfoWidget_;  //< Owning reference
    QPoint pos_;
};

}  // namespace

#endif  // IVW_PORTINSPECTIONMANAGER_H
