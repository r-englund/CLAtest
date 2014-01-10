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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/processors/processorwidget.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

ProcessorWidget::ProcessorWidget()
    : metaData_(0)
    , processor_(0)
    , initialized_(false){}

ProcessorWidget::~ProcessorWidget() {}

void ProcessorWidget::initialize() {    
     metaData_ = dynamic_cast<ProcessorWidgetMetaData*>(processor_->getMetaData("ProcessorWidgetMetaData"));
}

void ProcessorWidget::setVisible(bool visible) {
    metaData_->setVisibile(visible);
}

void ProcessorWidget::show() {
    metaData_->setVisibile(true);
}

void ProcessorWidget::hide() {    
    metaData_->setVisibile(false);
}

void ProcessorWidget::setDimension(ivec2 dimension) {   
    metaData_->setDimension(dimension);
}

void ProcessorWidget::move(ivec2 pos) {    
    metaData_->setWidgetPosition(pos);
}

bool ProcessorWidget::getVisibilityMetaData() {    
    return metaData_->isVisible();
}

ivec2 ProcessorWidget::getPositionMetaData() {
    return metaData_->getWidgetPosition();
}

ivec2 ProcessorWidget::getDimensionMetaData() {
    return metaData_->getDimension();
}

} // namespace
