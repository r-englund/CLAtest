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

#include "geometryrenderprocessorgl.h"
#include <modules/opengl/geometry/geometrygl.h>
#include <inviwo/core/interaction/trackball.h>
#include <modules/opengl/geometry/meshglrenderer.h>

namespace inviwo {

ProcessorClassName(GeometryRenderProcessorGL, "GeometryRenderGL"); 
ProcessorCategory(GeometryRenderProcessorGL, "Geometry Rendering");
ProcessorCodeState(GeometryRenderProcessorGL, CODE_STATE_STABLE);

GeometryRenderProcessorGL::GeometryRenderProcessorGL()
    : ProcessorGL(),
      inport_("geometry.inport"),
      outport_("image.outport"),
      camera_("camera", "Camera", vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 1.0f, 0.0f)),
      centerViewOnGeometry_("centerView", "Center view on geometry")
{
    addPort(inport_);
    addPort(outport_);

    addProperty(camera_);
    addInteractionHandler(new Trackball(&camera_));

    centerViewOnGeometry_.onChange(this, &GeometryRenderProcessorGL::centerViewOnGeometry);
    addProperty(centerViewOnGeometry_);

}

void GeometryRenderProcessorGL::process() {
    glEnable(GL_CULL_FACE); 

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.viewMatrix()));
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.projectionMatrix()));

    activateAndClearTarget(outport_);
    glCullFace(GL_BACK);

    //inport_.getData()->getRepresentation<GeometryGL>()->render();
    const Geometry* geom = inport_.getData();
    if( const Mesh* mesh = dynamic_cast<const Mesh*>(geom)) {
        MeshGLRenderer renderer(mesh);
        renderer.render();
    }

    deactivateCurrentTarget();

    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    glDisable(GL_CULL_FACE);
}

void GeometryRenderProcessorGL::centerViewOnGeometry() {
    const Mesh* geom = dynamic_cast<const Mesh*>(inport_.getData());
    if(geom == NULL) {
        return;
    }
    const Position3dBufferRAM* posBuffer = dynamic_cast<const Position3dBufferRAM*>(geom->getAttributes(0)->getRepresentation<BufferRAM>());
    const std::vector<vec3>* pos = posBuffer->getDataContainer();
    vec3 maxPos, minPos;
    if(pos->size() > 0) {
        maxPos = (*pos)[0];
        minPos = maxPos;
    }
    for(size_t i = 0; i < pos->size(); ++i) {
        maxPos = glm::max(maxPos, (*pos)[i]);
        minPos = glm::min(minPos, (*pos)[i]);
    }
    vec3 centerPos = 0.5f*(maxPos-minPos);
    camera_.setLookTo(centerPos);


}

} // namespace
