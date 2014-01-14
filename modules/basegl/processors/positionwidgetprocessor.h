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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_POSITIONWIDGETPROCESSOR_H
#define IVW_POSITIONWIDGETPROCESSOR_H

#include <modules/basegl/baseglmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/ports/geometryport.h>
#include <modules/opengl/image/compositeprocessorgl.h>
#include <inviwo/core/interaction/pickingobject.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {

class IVW_MODULE_BASEGL_API PositionWidgetProcessor : public CompositeProcessorGL {
public:
    PositionWidgetProcessor();
    ~PositionWidgetProcessor();

    InviwoProcessorInfo();

    void initialize();
    void deinitialize();

    bool isReady() const { return geometryInport_.isReady(); }

    void updateWidgetPositionFromPicking(const PickingObject*);

protected:
    virtual void process();

private:
    GeometryInport geometryInport_;
    ImageInport imageInport_;
    ImageOutport outport_;

    FloatVec3Property position_;

    CameraProperty camera_;

    const PickingObject* widgetPickingObject_;

    Shader* program_;
};

} // namespace

#endif // IVW_POSITIONWIDGETPROCESSOR_H
