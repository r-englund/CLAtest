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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_CANVASPROCESSOR_H
#define IVW_CANVASPROCESSOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/properties/vectorproperties.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/directoryproperty.h>
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/util/canvas.h>

namespace inviwo {

class IVW_CORE_API CanvasProcessor : public Processor {
public:
    CanvasProcessor();
    ~CanvasProcessor();

    virtual void initialize();
    virtual void deinitialize();

    virtual void process();

    void setCanvas(Canvas* canvas) { canvas_ = canvas; }
    Canvas* getCanvas() const { return canvas_; }
    void setCanvasSize(ivec2);
    ivec2 getCanvasSize() const;

    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel,
                            Property* modifiedProperty=0);

    void saveImageLayer();
    void saveImageLayer(const char* filePath);

protected:
    void performEvaluateRequest();

    ImageInport inport_;

    IntVec2Property dimensions_;
    IntOptionProperty visibleLayer_; // LayerType enum (Cannot serialize/deserialize enums so we use an int and cast it)
    DirectoryProperty saveLayerDirectory_;
    ButtonProperty saveLayerButton_;

private:
    Canvas* canvas_;
    bool disableResize_;

    void resizeCanvas();
};

} // namespace

#endif // IVW_CANVASPROCESSOR_H
