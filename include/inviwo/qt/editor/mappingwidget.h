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
 * Primary author : Hans-Christian Helltegen
 *
 **********************************************************************/

#ifndef IVW_MAPPINGWIDGET_H
#define IVW_MAPPINGWIDGET_H

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

class IVW_QTEDITOR_API MappingWidget : public InviwoDockWidget, public VoidObserver {
    Q_OBJECT
public:
    MappingWidget(QWidget* parent);
    ~MappingWidget();
    void notify();	

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