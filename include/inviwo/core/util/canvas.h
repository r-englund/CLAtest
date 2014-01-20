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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#ifndef IVW_CANVAS_H
#define IVW_CANVAS_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/image/image.h>
#include <inviwo/core/datastructures/geometry/geometry.h>
#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/interaction/pickingcontainer.h>
#include <inviwo/core/interaction/events/eventhandler.h>
#include <inviwo/core/interaction/events/keyboardevent.h>
#include <inviwo/core/interaction/events/mouseevent.h>

namespace inviwo {

class IVW_CORE_API Canvas {
public:
    Canvas(uvec2 dimensions);
    virtual ~Canvas();

    virtual void initialize();
    virtual void deinitialize();
    virtual void activate();
    virtual void render(const Image*);
    virtual void resize(uvec2 dimensions);
    virtual uvec2 getDimension() { return dimensions_;}
    virtual void update();

    void setNetworkEvaluator(ProcessorNetworkEvaluator* networkEvaluator) { processorNetworkEvaluator_ = networkEvaluator; };
    ProcessorNetworkEvaluator* getNetworkEvaluator() const { return processorNetworkEvaluator_; };

protected:
    void interactionEvent(InteractionEvent* e);

    void mousePressEvent(MouseEvent* e);
    void mouseReleaseEvent(MouseEvent* e);
    void mouseMoveEvent(MouseEvent* e);
    
    void keyPressEvent(KeyboardEvent* e);
    void keyReleaseEvent(KeyboardEvent* e);

    uvec2 mousePosToPixelCoordinates(ivec2 mpos);

    static Geometry* screenAlignedRect_;

    bool shared_;
    uvec2 dimensions_;

    PickingContainer* pickingContainer_;

    ProcessorNetworkEvaluator* processorNetworkEvaluator_;
};

} // namespace

#endif // IVW_CANVAS_H
