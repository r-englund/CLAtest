/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Daniel Jönsson, Erik Sundén
 *
 *********************************************************************************/

#include "geometryrenderprocessorgl.h"
#include <modules/opengl/geometry/geometrygl.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>
#include <inviwo/core/interaction/trackball.h>
#include <inviwo/core/rendering/geometryrendererfactory.h>
#include <modules/opengl/rendering/meshrenderer.h>
#include <inviwo/core/processors/processor.h>

namespace inviwo {

ProcessorClassIdentifier(GeometryRenderProcessorGL,  "GeometryRenderGL");
ProcessorDisplayName(GeometryRenderProcessorGL,  "Geometry Render");
ProcessorTags(GeometryRenderProcessorGL, Tags::GL);
ProcessorCategory(GeometryRenderProcessorGL, "Geometry Rendering");
ProcessorCodeState(GeometryRenderProcessorGL, CODE_STATE_STABLE);

GeometryRenderProcessorGL::GeometryRenderProcessorGL()
    : ProcessorGL()
    , inport_("geometry.inport")
    , outport_("image.outport")
    , camera_("camera", "Camera")
    , centerViewOnGeometry_("centerView", "Center view on geometry")
    , resetViewParams_("resetView", "Reset Camera")
    , cullFace_("cullFace", "Cull Face")
    , polygonMode_("polygonMode", "Polygon Mode")
    , renderPointSize_("renderPointSize", "Point Size", 1.0f, 0.001f, 15.0f, 0.001f)
    , shadingMode_("shadingMode", "Shading", PropertyOwner::INVALID_RESOURCES)
    , lightPosition_("lightPosition", "Position", vec3(0.0f, 0.7071f, 0.7071f) , vec3(-10,-10,-10) , vec3(10,10,10))
    , lightColorAmbient_("lightColorAmbient", "Ambient color", vec3(1.f, 1.f, 1.f))
    , lightColorDiffuse_("lightColorDiffuse", "Diffuse color", vec3(0.5f, 0.5f, 0.5f))
    , lightColorSpecular_("lightColorSpecular", "Specular color", vec3(1.f, 1.f, 1.f))
    , lightSpecularExponent_("materialShininess", "Material shininess", 110, 0, 180)
    , applyLightAttenuation_("applyLightAttenuation", "Light attenuation", false)
    , lightAttenuation_("lightAttenuation", "Light attenuation values", vec3(0.5f, 0.5f, 0.5f)) {
    
    addPort(inport_);
    addPort(outport_);
    addProperty(camera_);
    trackball_ = new Trackball(&camera_);
    addInteractionHandler(trackball_);
    centerViewOnGeometry_.onChange(this, &GeometryRenderProcessorGL::centerViewOnGeometry);
    addProperty(centerViewOnGeometry_);
    resetViewParams_.onChange(this, &GeometryRenderProcessorGL::resetViewParams);
    addProperty(resetViewParams_);
    outport_.addResizeEventListener(&camera_);
    inport_.onChange(this,&GeometryRenderProcessorGL::updateRenderers);
    cullFace_.addOption("culldisable", "Disable", 0);
    cullFace_.addOption("cullfront", "Front", GL_FRONT);
    cullFace_.addOption("cullback", "Back", GL_BACK);
    cullFace_.addOption("cullfrontback", "Front & Back", GL_FRONT_AND_BACK);
    cullFace_.set(0);

    polygonMode_.addOption("polypoint", "Points", GL_POINT);
    polygonMode_.addOption("polyline", "Lines", GL_LINE);
    polygonMode_.addOption("polyfill", "Fill", GL_FILL);
    polygonMode_.set(GL_FILL);

    addProperty(cullFace_);
    addProperty(polygonMode_);
    addProperty(renderPointSize_);

    // add shading properties
    shadingMode_.addOption("none", "No Shading");
    shadingMode_.addOption("ambient", "Ambient");
    shadingMode_.addOption("diffuse", "Diffuse");
    shadingMode_.addOption("specular", "Specular");
    shadingMode_.addOption("phong", "Phong");
    shadingMode_.setSelectedIdentifier("none");
    shadingMode_.setCurrentStateAsDefault();

    ///lightPosition_.setSemantics(PropertySemantics::LightPosition);
    lightPosition_.setSemantics(PropertySemantics("Spherical"));
    lightColorAmbient_.setSemantics(PropertySemantics::Color);
    lightColorDiffuse_.setSemantics(PropertySemantics::Color);
    lightColorSpecular_.setSemantics(PropertySemantics::Color);

    addProperty(shadingMode_);
    addProperty(lightPosition_);
    addProperty(lightColorAmbient_);
    addProperty(lightColorDiffuse_);
    addProperty(lightColorSpecular_);
    addProperty(lightSpecularExponent_);
    addProperty(applyLightAttenuation_);
    addProperty(lightAttenuation_);

    // assign lighting properties to property group
    lightPosition_.setGroupID("lighting");
    lightColorAmbient_.setGroupID("lighting");
    lightColorDiffuse_.setGroupID("lighting");
    lightColorSpecular_.setGroupID("lighting");
    lightSpecularExponent_.setGroupID("lighting");
    applyLightAttenuation_.setGroupID("lighting");
    lightAttenuation_.setGroupID("lighting");

    setAllPropertiesCurrentStateAsDefault();
}

GeometryRenderProcessorGL::~GeometryRenderProcessorGL() {
    removeInteractionHandler(trackball_);
    delete trackball_;
    trackball_ = NULL;
}

void GeometryRenderProcessorGL::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("geometryrendering.vert", "geometryrendering.frag", false);
    initializeResources();
}

void GeometryRenderProcessorGL::deinitialize() {
    // Delete all renderers
    for (std::vector<GeometryRenderer*>::iterator it = renderers_.begin(), endIt = renderers_.end(); it != endIt; ++it) {
        delete *it;
    }
    if (shader_) 
        delete shader_;
    shader_ = NULL;
    ProcessorGL::deinitialize();
}

void GeometryRenderProcessorGL::initializeResources() {
    // shading defines
    std::string shadingKey = "APPLY_SHADING(colorAmb, colorDiff, colorSpec, samplePos, gradient, lightPos, cameraPos)";
    std::string shadingValue = "";

    if(shadingMode_.isSelectedIdentifier("none"))
        shadingValue = "colorAmb;";
    else if(shadingMode_.isSelectedIdentifier("ambient"))
        shadingValue = "shadeAmbient(colorAmb);";
    else if(shadingMode_.isSelectedIdentifier("diffuse"))
        shadingValue = "shadeDiffuse(colorDiff, samplePos, gradient, lightPos);";
    else if(shadingMode_.isSelectedIdentifier("specular"))
        shadingValue = "shadeSpecular(colorSpec, samplePos, gradient, lightPos, cameraPos);";
    else if(shadingMode_.isSelectedIdentifier("phong"))
        shadingValue = "shadePhong(colorAmb, colorDiff, colorSpec, samplePos, gradient, lightPos, cameraPos);";

    shader_->getFragmentShaderObject()->addShaderDefine(shadingKey, shadingValue);

    shader_->build();
}

void GeometryRenderProcessorGL::process() {
    if (!inport_.hasData()) {
        return;
    }

    if (renderers_.empty()) {
        return;
    }

    int prevPolygonMode[2];
    glGetIntegerv(GL_POLYGON_MODE, prevPolygonMode);
    glPolygonMode(GL_FRONT_AND_BACK, polygonMode_.get());
    GLboolean depthTest = glIsEnabled(GL_DEPTH_TEST);
    if (!depthTest) {    
        glEnable(GL_DEPTH_TEST);
    }

    activateAndClearTarget(outport_);

    shader_->activate();
    setGlobalShaderParameters(shader_);
    shader_->setUniform("projectionMatrix_", camera_.projectionMatrix());

    bool culling = (cullFace_.get() != 0);
    if (culling) {
        glEnable(GL_CULL_FACE); 
        glCullFace(cullFace_.get());
    }

    if (polygonMode_.get()==GL_LINE) {
        glEnable(GL_LINE_SMOOTH);
        glLineWidth((GLfloat)renderPointSize_.get());
    }
    else if (polygonMode_.get()==GL_POINT)
        glPointSize((GLfloat)renderPointSize_.get());

    for (std::vector<GeometryRenderer*>::const_iterator it = renderers_.begin(), endIt = renderers_.end(); it != endIt; ++it) {
        shader_->setUniform("viewToTexture_", (*it)->getGeometry()->getCoordinateTransformer().getWorldToModelMatrix());
        mat4 modelViewMatrix = camera_.viewMatrix()*(*it)->getGeometry()->getWorldTransform()*(*it)->getGeometry()->getBasisAndOffset();
        shader_->setUniform("modelViewMatrix_", modelViewMatrix);
        (*it)->render();
    }

    if (polygonMode_.get()==GL_LINE)
        glDisable(GL_LINE_SMOOTH);

    shader_->deactivate();

    deactivateCurrentTarget();

    if (culling) {
        glDisable(GL_CULL_FACE);
    }
    if (!depthTest) {
        glDisable(GL_DEPTH_TEST);
    }
    // restore
    glPointSize(1.f);
    glPolygonMode(GL_FRONT, prevPolygonMode[0]);
    glPolygonMode(GL_BACK, prevPolygonMode[1]);
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

void GeometryRenderProcessorGL::setGlobalShaderParameters(Shader* shader) {
    ProcessorGL::setGlobalShaderParameters(shader);
    // camera uniform
    shader->setUniform("viewMatrix_", camera_.viewMatrix());
    shader->setUniform("cameraPosition_", camera_.getLookFrom());
    // illumination uniforms
    shader->setUniform("lightPosition_", lightPosition_.get());
    shader->setUniform("lightColorAmbient_", lightColorAmbient_.get());
    shader->setUniform("lightColorDiffuse_", lightColorDiffuse_.get());
    shader->setUniform("lightColorSpecular_", lightColorSpecular_.get());
    shader->setUniform("lightSpecularExponent_", lightSpecularExponent_.get());
}

void GeometryRenderProcessorGL::updateRenderers() {
    std::vector<const Geometry*> geometries = inport_.getData();

    if (geometries.empty()) {
        while (!renderers_.empty()) {
            delete renderers_.back();
            renderers_.pop_back();
        }
    }

    if (!renderers_.empty()) {
        std::vector<GeometryRenderer*>::iterator it = renderers_.begin();

        while (it!=renderers_.end()) {
            const Geometry* geo = (*it)->getGeometry();
            bool geometryRemoved = true;

            for (size_t j = 0; j < geometries.size(); j++) {
                if (geo == geometries[j]) {
                    geometryRemoved = false;
                    geometries.erase(geometries.begin()+j); //nothing needs to be changed for this geometry
                    break;
                }
            }

            if (geometryRemoved) {
                GeometryRenderer* tmp = (*it);
                it = renderers_.erase(it); //geometry removed, so we delete the old renderer
                delete tmp;
            } else {
                ++it;
            }
        }
    }

    for (size_t i = 0; i < geometries.size() ; ++i) { //create new renderer for new geometries
        GeometryRenderer* renderer = GeometryRendererFactory::getPtr()->create(geometries[i]);

        if (renderer) {
            renderers_.push_back(renderer);
        }
    }
}

void GeometryRenderProcessorGL::resetViewParams() {
    camera_.resetCamera();
}
} // namespace
