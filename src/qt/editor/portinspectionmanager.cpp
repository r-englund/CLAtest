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

#include <inviwo/qt/editor/portinspectionmanager.h>
#include <inviwo/core/util/timer.h>
#include <inviwo/qt/editor/editorgrapicsitem.h>
#include <inviwo/core/ports/port.h>
#include <inviwo/qt/editor/networkeditor.h>

namespace inviwo {

PortInspectionManager::PortInspectionManager(EditorGrapicsItem* item)
    : item_(item), hoverDelay_(750), portInfoWidget_(NULL) {
    hoverTimer_ = InviwoApplication::getPtr()->createTimer();
    hoverTimer_->setElapsedTimeCallback(this, &PortInspectionManager::executeHoverTimer);
}

PortInspectionManager::~PortInspectionManager() {
    delete hoverTimer_;
    if (portInfoWidget_ != NULL) {
        delete portInfoWidget_;
    }
}

void PortInspectionManager::startTimer() {
    startTimer(item_->mapPosToSceen(item_->rect().center()));
}

void PortInspectionManager::startTimer(QPoint pos) {
    pos_ = pos;
    hoverTimer_->start(hoverDelay_, true);
}

void PortInspectionManager::showPortInfo() {
    int offset = 0;

    Port* port = item_->getInfoPort();

    if (port) {
        if (InviwoApplication::getPtr()
                ->getSettingsByType<SystemSettings>()
                ->enablePortInspectorsProperty_.get()) {
            bool added = NetworkEditor::getPtr()->addPortInspector(
                port->getProcessor()->getIdentifier(), port->getIdentifier(), pos_ + QPoint(5, 5));

            if (added) {
                offset = InviwoApplication::getPtr()
                             ->getSettingsByType<SystemSettings>()
                             ->portInspectorSize_.get() +
                         10;
            }
        }

        if (InviwoApplication::getPtr()
                ->getSettingsByType<SystemSettings>()
                ->enablePortInformationProperty_.get()) {
            if (portInfoWidget_ == NULL) {
                portInfoWidget_ = new PortInfoWidgetQt();
            }
            portInfoWidget_->updatePortInfo(port->getIdentifier(), port->getContentInfo());
            portInfoWidget_->move(pos_ + QPoint(5, 5 + offset));
            portInfoWidget_->show();
        }
    }
}




void PortInspectionManager::hidePortInfo() {
    hoverTimer_->stop();
    if (portInfoWidget_ && portInfoWidget_->isVisible()) {
        portInfoWidget_->hide();
    }

    NetworkEditor::getPtr()->removePortInspector();
}

void PortInspectionManager::executeHoverTimer() { showPortInfo(); }

}  // namespace
