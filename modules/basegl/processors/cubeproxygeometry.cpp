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
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#include "cubeproxygeometry.h"
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>

namespace inviwo {

ProcessorClassName(CubeProxyGeometry, "CubeProxyGeometry"); 
ProcessorCategory(CubeProxyGeometry, "Geometry Creation");
ProcessorCodeState(CubeProxyGeometry, CODE_STATE_STABLE); 

CubeProxyGeometry::CubeProxyGeometry()
    : ProcessorGL(),
      inport_("volume.inport"),
      outport_("geometry.outport"),
      clippingEnabled_("clippingEnabled", "Enable Clipping", true),
      clipX_("clipX", "Clip X Slices", 0, 256, 0, 256),
      clipY_("clipY", "Clip Y Slices", 0, 256, 0, 256),
      clipZ_("clipZ", "Clip Z Slices", 0, 256, 0, 256)
{
    addPort(inport_);
    addPort(outport_);

    addProperty(clippingEnabled_);
    addProperty(clipX_);
    addProperty(clipY_);
    addProperty(clipZ_);

    dims_ = uvec3(1,1,1);
}

CubeProxyGeometry::~CubeProxyGeometry() {}

void CubeProxyGeometry::initialize() {
    ProcessorGL::initialize();
}

void CubeProxyGeometry::deinitialize() {
    ProcessorGL::deinitialize();
}

void CubeProxyGeometry::process() {
    if (inport_.hasData() && (dims_ != inport_.getData()->getDimension() || basis_ != inport_.getData()->getBasisAndOffset()) ) {
            dims_ = inport_.getData()->getDimension();
            basis_ = inport_.getData()->getBasisAndOffset();
            clipX_.setRangeMax(static_cast<int>(dims_.x));
            clipY_.setRangeMax(static_cast<int>(dims_.y));
            clipZ_.setRangeMax(static_cast<int>(dims_.z));
	}

    glm::vec3 pos(0.0f);
    glm::vec3 p1(1.0f,0.0f,0.0f);
    glm::vec3 p2(0.0f,1.0f,0.0f);
    glm::vec3 p3(0.0f,0.0f,1.0f);

    glm::vec3 tex(0.0f);
    glm::vec3 t1(1.0f,0.0f,0.0f);
    glm::vec3 t2(0.0f,1.0f,0.0f);
    glm::vec3 t3(0.0f,0.0f,1.0f);

    glm::vec4 col(0.0f,0.0f,0.0f,1.0f);
    glm::vec4 c1(1.0f,0.0f,0.0f,0.0f);
    glm::vec4 c2(0.0f,1.0f,0.0f,0.0f);
    glm::vec4 c3(0.0f,0.0f,1.0f,0.0f);

    if (clippingEnabled_.get()) {
        pos = pos + p1*static_cast<float>(clipX_.get().x)/static_cast<float>(dims_.x)
                  + p2*static_cast<float>(clipY_.get().x)/static_cast<float>(dims_.y)
                  + p3*static_cast<float>(clipZ_.get().x)/static_cast<float>(dims_.z);

        p1 = p1*(static_cast<float>(clipX_.get().y)-static_cast<float>(clipX_.get().x))/static_cast<float>(dims_.x);
        p2 = p2*(static_cast<float>(clipY_.get().y)-static_cast<float>(clipY_.get().x))/static_cast<float>(dims_.y);
        p3 = p3*(static_cast<float>(clipZ_.get().y)-static_cast<float>(clipZ_.get().x))/static_cast<float>(dims_.z);

        tex = tex + t1*static_cast<float>(clipX_.get().x)/static_cast<float>(dims_.x)
                  + t2*static_cast<float>(clipY_.get().x)/static_cast<float>(dims_.y)
                  + t3*static_cast<float>(clipZ_.get().x)/static_cast<float>(dims_.z);

        t1 = t1*(static_cast<float>(clipX_.get().y)-static_cast<float>(clipX_.get().x))/static_cast<float>(dims_.x);
        t2 = t2*(static_cast<float>(clipY_.get().y)-static_cast<float>(clipY_.get().x))/static_cast<float>(dims_.y);
        t3 = t3*(static_cast<float>(clipZ_.get().y)-static_cast<float>(clipZ_.get().x))/static_cast<float>(dims_.z);

        col = col + c1*static_cast<float>(clipX_.get().x)/static_cast<float>(dims_.x)
                  + c2*static_cast<float>(clipY_.get().x)/static_cast<float>(dims_.y)
                  + c3*static_cast<float>(clipZ_.get().x)/static_cast<float>(dims_.z);

        c1 = c1*(static_cast<float>(clipX_.get().y)-static_cast<float>(clipX_.get().x))/static_cast<float>(dims_.x);
        c2 = c2*(static_cast<float>(clipY_.get().y)-static_cast<float>(clipY_.get().x))/static_cast<float>(dims_.y);
        c3 = c3*(static_cast<float>(clipZ_.get().y)-static_cast<float>(clipZ_.get().x))/static_cast<float>(dims_.z);
	}


    //Create parallelepiped and set it to the outport
    Geometry* geom = 
        SimpleMeshCreator::parallelepiped(pos, p1, p2, p3, 	
										  tex, t1, t2, t3, 	
										  col, c1, c2, c3);

    // This would be easier, use unit box to make geom instead...
    geom->setBasisAndOffset(inport_.getData()->getBasisAndOffset());

    geom->setWorldTransform(inport_.getData()->getWorldTransform());

    outport_.setData(geom);
}

} // namespace
