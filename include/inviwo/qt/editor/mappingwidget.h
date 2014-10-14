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
 * Contact: Sathish Kottravel
 *
 *********************************************************************************/

#ifndef IVW_MAPPINGWIDGET_H
#define IVW_MAPPINGWIDGET_H

#include <inviwo/core/network/processornetworkobserver.h>
#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/qt/widgets/inviwodockwidget.h>
#include <inviwo/core/util/observer.h>
#include <inviwo/core/properties/eventproperty.h>
#include <vector>
#include <QFrame>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QComboBox>

namespace inviwo {

class Processor;
class ProcessorNetwork;
class EventPropertyManager;

class IVW_QTEDITOR_API MappingWidget : public InviwoDockWidget, public ProcessorNetworkObserver {
    Q_OBJECT
public:
    MappingWidget(QWidget* parent);
    ~MappingWidget();
    void onProcessorNetworkChange();

private:
    void updateWidget();
    void buildLayout();
    void findProcessorsWithInteractionHandlers(std::vector<Processor*>* container, std::vector<Processor*> processors);

    ProcessorNetwork* processorNetwork_;
    EventPropertyManager* eventPropertyManager_;
    std::vector<Processor*>* processorsWithInteractionHandlers_;
    size_t prevProcessorsWithInteractionHandlersSize_;

    QComboBox* comboBox_;

    int currentIndex_;
public slots:
    void comboBoxChange();

};

} // namespace

#endif // IVW_MAPPINGWIDGET_H