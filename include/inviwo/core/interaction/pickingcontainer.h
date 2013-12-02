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

#ifndef IVW_PICKINGCONTAINER_H
#define IVW_PICKINGCONTAINER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class Image;
class PickingObject;

class IVW_CORE_API PickingContainer {
public:
    PickingContainer();
    virtual ~PickingContainer();

    bool isPickableSelected();

    bool performPick(const uvec2& coord);
    void movePicked(const uvec2& coord);

    void setPickableSelected(bool selected);
    void setPickingSource(const Image* src);

protected:
    vec2 pixelMoveVector(const uvec2& previous, const uvec2& current);
    vec2 normalizedCoordinates(const uvec2& coord);

private:
    const Image* src_;
    PickingObject* currentPickObj_;
    uvec2 prevCoord_;
    bool selected_;
};

} // namespace

#endif // IVW_PICKINGCONTAINER_H