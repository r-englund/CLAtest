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
#include <inviwo/core/rendering/geometryrendererfactory.h>
#include <modules/opengl/rendering/meshrenderer.h>

namespace inviwo {

ProcessorClassName(GeometryRenderProcessorGL, "GeometryRenderGL"); 
ProcessorCategory(GeometryRenderProcessorGL, "Geometry Rendering");
ProcessorCodeState(GeometryRenderProcessorGL, CODE_STATE_STABLE);

GeometryRenderProcessorGL::GeometryRenderProcessorGL()
    : ProcessorGL()
    , inport_("geometry.inport")
    , outport_("image.outport")
    , camera_("camera", "Camera")
    , centerViewOnGeometry_("centerView", "Center view on geometry")
{
    addPort(inport_);
    addPort(outport_);

    addProperty(camera_);
    addInteractionHandler(new Trackball(&camera_));

    centerViewOnGeometry_.onChange(this, &GeometryRenderProcessorGL::centerViewOnGeometry);
    addProperty(centerViewOnGeometry_);
    outport_.addResizeEventListener(&camera_);

}

void GeometryRenderProcessorGL::deinitialize() {
    // Delete all renderers
    for (std::vector<GeometryRenderer*>::iterator it = renderers_.begin(), endIt = renderers_.end(); it != endIt; ++it) {
        delete *it;
    }
}


void GeometryRenderProcessorGL::process() {
    std::vector<const Geometry*> geometries = inport_.getData();
    std::vector<GeometryRenderer*>::iterator renIt = renderers_.begin();
    for (std::vector<const Geometry*>::const_iterator it = geometries.begin(), endIt = geometries.end(); it != endIt; ++it, ++renIt) {
        const Geometry* geom = *it;
        if (renIt == renderers_.end()) {
            GeometryRenderer *renderer = GeometryRendererFactory::getPtr()->create(geom);
            // Do not render if no renderer exist for this geometry
            if(renderer != NULL) {
                renderers_.push_back(renderer);
                // Assign iterator to last element
                renIt = --renderers_.end();
            }
        } else if (geom != (*renIt)->getGeometry()) {
            // The geometry changed. Create a new renderer for it
            delete *renIt;
            *renIt = GeometryRendererFactory::getPtr()->create(geom);
        }
    }
    glEnable(GL_CULL_FACE); 
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.projectionMatrix()));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.viewMatrix()));


    activateAndClearTarget(outport_);
    glCullFace(GL_BACK);

    for (std::vector<GeometryRenderer*>::const_iterator it = renderers_.begin(), endIt = renderers_.end(); it != endIt; ++it) {
        (*it)->render();
    }

    deactivateCurrentTarget();

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glDisable(GL_CULL_FACE);
}

void GeometryRenderProcessorGL::centerViewOnGeometry() {
	std::vector<const Geometry*> geometries = inport_.getData();
	if (geometries.empty()) return;

    const Mesh* geom = dynamic_cast<const Mesh*>(geometries[0]);
    if (geom == NULL) {
        return;
    }
    const Position3dBufferRAM* posBuffer = dynamic_cast<const Position3dBufferRAM*>(geom->getAttributes(0)->getRepresentation<BufferRAM>());
    if (posBuffer == NULL) {
        return;
    }
    const std::vector<vec3>* pos = posBuffer->getDataContainer();
    vec3 maxPos, minPos;
    if (pos->size() > 0) {
        maxPos = (*pos)[0];
        minPos = maxPos;
    }
    for (size_t i = 0; i < pos->size(); ++i) {
        maxPos = glm::max(maxPos, (*pos)[i]);
        minPos = glm::min(minPos, (*pos)[i]);
    }
    vec3 centerPos = (geom->getWorldTransform()*geom->getBasisAndOffset()*vec4(0.5f*(maxPos+minPos), 1.f)).xyz();
    vec3 lookFrom = camera_.getLookFrom();
    vec3 dir = centerPos - lookFrom;
    if (glm::length(dir) < 1e-6f) {
        dir = vec3(0.f, 0.f, -1.f);
    }
    dir = glm::normalize(dir);
    vec3 worldMin = (geom->getWorldTransform()*geom->getBasisAndOffset()*vec4(minPos, 1.f)).xyz();
    vec3 worldMax = (geom->getWorldTransform()*geom->getBasisAndOffset()*vec4(maxPos, 1.f)).xyz();
    vec3 newLookFrom = lookFrom -dir*glm::length(worldMax-worldMin);
    camera_.setLook(newLookFrom, centerPos, camera_.getLookUp());


}

} // namespace
