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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <modules/openglqt/openglqtmodule.h>
#include <modules/openglqt/openglqtcapabilities.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <modules/opengl/canvasprocessorgl.h>
#include <modules/openglqt/processors/canvasprocessorwidgetqt.h>

namespace inviwo {

OpenGLQtModule::OpenGLQtModule() : InviwoModule() {
    setIdentifier("OpenGLQt");
    setXMLFileName("openglqt/openglqtmodule.xml");

    //Create GL Context
    qtGLSharedCanvas_ = new CanvasQt();
    if(InviwoApplication::getPtr()){
        ProcessorNetwork* network = InviwoApplication::getPtr()->getProcessorNetwork();
        if(network){
            ProcessorNetworkEvaluator* evaluator = ProcessorNetworkEvaluator::getProcessorNetworkEvaluatorForProcessorNetwork(network);
            qtGLSharedCanvas_->setNetworkEvaluator(evaluator);
            evaluator->setDefaultRenderContext(qtGLSharedCanvas_);
        }
    }

    registerProcessorWidgetAndAssociate<CanvasProcessorGL>(new CanvasProcessorWidgetQt());
    registerCapabilities(new OpenGLQtCapabilities());
}

OpenGLQtModule::~OpenGLQtModule(){
    delete qtGLSharedCanvas_;
}

} // namespace
