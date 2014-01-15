/**********************************************************************
 * Copyright (C) 2012-2014 Scientific Visualization Group - Linköping University
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

#ifndef IVW_GEOMETRY_RENDERER_FACTORY_H
#define IVW_GEOMETRY_RENDERER_FACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/geometry/geometry.h>
#include <inviwo/core/rendering/geometryrenderer.h>
#include <inviwo/core/util/inviwofactorybase.h>
#include <inviwo/core/util/singleton.h>
#include <set>

namespace inviwo {



class IVW_CORE_API GeometryRendererFactory : public Singleton<GeometryRendererFactory>  {
public:
    GeometryRendererFactory();
    virtual ~GeometryRendererFactory() {
        //for (std::set<GeometryRenderer*>::iterator it = renderers_.begin(), itEnd = renderers_.end(); it != itEnd; ++it) {
        //    delete *it;
        //}
        //renderers_.clear();
    };

    void registerFactoryObject(GeometryRenderer* renderer);
    virtual GeometryRenderer* create(const Geometry* geom) const;


private:
    std::set<GeometryRenderer*> renderers_;
};

} // namespace

#endif // IVW_GEOMETRY_RENDERER_FACTORY_H
