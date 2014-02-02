/**********************************************************************
 * Copyright (C) 2014 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#include <inviwo/core/rendering/geometryrendererfactory.h>
#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

struct CanRenderGeometry {
public:
    CanRenderGeometry(const Geometry* geom): geom_(geom) {};
    bool operator()(const GeometryRenderer* renderer)
    { return renderer->canRender(geom_); }
private:
    const Geometry* geom_;
};

GeometryRendererFactory::GeometryRendererFactory() {
}

void GeometryRendererFactory::registerObject(GeometryRenderer* renderer) {
    renderers_.insert(renderer);
}

GeometryRenderer* GeometryRendererFactory::create(const Geometry* geom) const {
    std::set<GeometryRenderer*>::const_iterator it = std::find_if(renderers_.begin(), renderers_.end(), CanRenderGeometry(geom));

    if (it != renderers_.end())
        return (*it)->create(geom);
    else
        return NULL;
};

} // namespace

