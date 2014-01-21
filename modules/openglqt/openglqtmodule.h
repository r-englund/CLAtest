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

#ifndef IVW_OPENGLQT_MODULE_H
#define IVW_OPENGLQT_MODULE_H

#include <modules/openglqt/openglqtmoduledefine.h>
#include <inviwo/core/common/inviwomodule.h>
#include <modules/openglqt/canvasqt.h>

namespace inviwo {

class IVW_MODULE_OPENGLQT_API OpenGLQtModule : public InviwoModule {

public:
    OpenGLQtModule();
    virtual ~OpenGLQtModule();

protected:
    template <typename T>
    void registerProcessorWidgetAndAssociate(ProcessorWidget* processorWidget) {      
        registerProcessorWidget(T::CLASS_NAME, processorWidget);
    }

private:
    CanvasQt* qtGLSharedCanvas_;
};

} // namespace

#endif // IVW_OPENGLQT_MODULE_H
