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
 * Main file author: Martin Falk
 *
 *********************************************************************************/

#include "heightfieldprocessor.h"
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>
#include <inviwo/core/datastructures/light/directionallight.h>
#include <inviwo/core/util/glmstreamoperators.h>

namespace inviwo {

ProcessorClassIdentifier(HeightFieldProcessor, "org.inviwo.HeightFieldRenderGL");
ProcessorDisplayName(HeightFieldProcessor,  "Height Field Render");
ProcessorTags(HeightFieldProcessor, Tags::GL); 
ProcessorCategory(HeightFieldProcessor, "Heightfield");
ProcessorCodeState(HeightFieldProcessor, CODE_STATE_EXPERIMENTAL); 

HeightFieldProcessor::HeightFieldProcessor()
    : GeometryRenderProcessorGL()
    , inportHeightfield_("heightfield.inport", true)
    , inportTexture_("texture.inport", true)
    , inportNormalMap_("normalmap.inport", true)
    , inportLightSource_("lightsource.inport")
    , heightScale_("heightScale", "Height Scale", 1.0f, 0.0f, 100.0f)
    , terrainShadingMode_("terrainShadingMode", "Terrain Shading")
    , lightingProperty_("lighting", "Lighting", false)
    , shader_(0)
    , lighting_(false)
{
    inportHeightfield_.onChange(this, &HeightFieldProcessor::heightfieldChanged);
    addPort(inportHeightfield_);
    addPort(inportTexture_);
    addPort(inportNormalMap_);
    addPort(inportLightSource_);
    
    addProperty(heightScale_);

    terrainShadingMode_.addOption("shadingFlat", "Flat Shading", HF_SHADING_FLAT);
    terrainShadingMode_.addOption("shadingColorTex", "Color Texture", HF_SHADING_COLORTEX);
    terrainShadingMode_.addOption("shadingHeightField", "Heightfield Texture", HF_SHADING_HEIGHTFIELD);
    terrainShadingMode_.set(HF_SHADING_FLAT);
    terrainShadingMode_.setCurrentStateAsDefault();
    addProperty(terrainShadingMode_);

    //lightingProperty_.onChange(this, &HeightFieldProcessor::lightingChanged);
    addProperty(lightingProperty_);
}

HeightFieldProcessor::~HeightFieldProcessor() 
{
}

void HeightFieldProcessor::initialize() {
    GeometryRenderProcessorGL::initialize();

    // initialize shader to offset vertices in the vertex shader
    shader_ = new Shader("heightfield.vert", "heightfield.frag", true);
}

void HeightFieldProcessor::deinitialize() {
    // cleanup shader
    delete shader_;

    GeometryRenderProcessorGL::deinitialize();
}

void HeightFieldProcessor::process() {
    int terrainShadingMode = terrainShadingMode_.get();

    bool lighting = (lightingProperty_.get() && inportLightSource_.isReady());

    // bind input textures
    TextureUnit heightFieldUnit;
    if (inportHeightfield_.isReady()) {
        bindColorTexture(inportHeightfield_, heightFieldUnit.getEnum());
    } else if (terrainShadingMode == HF_SHADING_HEIGHTFIELD) {
        // switch to flat shading since color texture is not available
        terrainShadingMode = HF_SHADING_FLAT;
    }
    TextureUnit::setZeroUnit();
    
    TextureUnit colorTexUnit;
    if (inportTexture_.isReady()) {
        bindColorTexture(inportTexture_, colorTexUnit.getEnum());
    } else if (terrainShadingMode == HF_SHADING_COLORTEX) {
        // switch to flat shading since heightfield texture is not available
        terrainShadingMode = HF_SHADING_FLAT;
    }
    TextureUnit::setZeroUnit();
    
    TextureUnit normalTexUnit;
    if (inportNormalMap_.isReady()) {
        bindColorTexture(inportNormalMap_, normalTexUnit.getEnum());
    } else {
        // switch of lighting
        lighting = false;
    }
    TextureUnit::setZeroUnit();

    //bool lightColorChanged = false;
    //if(inportLightSource_.getInvalidationLevel() >= INVALID_OUTPUT) {
    //    lightColorChanged = lightSourceChanged();
    //}

    shader_->activate();
    shader_->setUniform("inportHeightfield_", heightFieldUnit.getUnitNumber());
    shader_->setUniform("inportTexture_", colorTexUnit.getUnitNumber());
    shader_->setUniform("inportNormalMap_", normalTexUnit.getUnitNumber());
    shader_->setUniform("terrainShadingMode_", terrainShadingMode);
    shader_->setUniform("heightScale_", heightScale_.get());

    shader_->setUniform("lighting_", lighting ? 1 : 0);
    if (lighting) {

        const DirectionalLight* directionLight = dynamic_cast<const DirectionalLight*>(inportLightSource_.getData());

        shader_->setUniform("lightPos_", directionLight->getDirection());
        shader_->setUniform("lightIntensity_", directionLight->getIntensity());

        //directionLight->getDirection();
        //directionLight->getIntensity();
        //directionLight->getPower();
    }

    // render mesh
    GeometryRenderProcessorGL::process();

    shader_->deactivate();
}

void HeightFieldProcessor::setupLight() {
    /*
    glm::vec3 color = glm::vec3(1.f);
    switch(inportLightSource_.getData()->getLightSourceType())
    {
        case LightSourceType::LIGHT_DIRECTIONAL:{
            if(lightType_ != LightSourceType::LIGHT_DIRECTIONAL){
                lightType_ = LightSourceType::LIGHT_DIRECTIONAL;
                shader_->getFragmentShaderObject()->removeShaderDefine("POINT_LIGHT");
                shader_->getFragmentShaderObject()->build();
                shader_->link();
            }
            const DirectionalLight* directionLight = dynamic_cast<const DirectionalLight*>(inportLightSource_.getData());
            if(directionLight){
                directionToCenterOfVolume = directionLight->getDirection();
                color = directionLight->getIntensity();
            }
            break;
        }
        case LightSourceType::LIGHT_POINT:{
            if(lightType_ != LightSourceType::LIGHT_POINT){
                lightType_ = LightSourceType::LIGHT_POINT;
                shader_->getFragmentShaderObject()->addShaderDefine("POINT_LIGHT");
                shader_->getFragmentShaderObject()->build();
                shader_->link();
            }
            const PointLight* pointLight = dynamic_cast<const PointLight*>(inportLightSource_.getData());
            if(pointLight){
                mat4 toWorld = inport_.getData()->getWorldTransform();
                vec4 volumeCenterWorldW = toWorld * vec4(0.f, 0.f, 0.f, 1.f);
                vec3 volumeCenterWorld = volumeCenterWorldW.xyz();
                lightPos_ = pointLight->getPosition();
                directionToCenterOfVolume = glm::normalize(volumeCenterWorld - lightPos_);
                color = pointLight->getIntensity();
            }
            break;
        }
        default:
            LogWarn("Light source not supported, can only handle Directional or Point Light");
            break;
    }

    bool lightColorChanged = false;
    if(color.r != lightColor_.r){
        lightColor_.r = color.r;
        lightColorChanged = true;
    }
    if(color.g != lightColor_.g){
        lightColor_.g = color.g;
        lightColorChanged = true;
    }
    if(color.b != lightColor_.b){
        lightColor_.b = color.b;
        lightColorChanged = true;
    }
    */
}

void HeightFieldProcessor::lightingChanged() {
    if (lighting_ != lightingProperty_.get()) {
        // state of lighting changed, adjust shader
    }
}


void HeightFieldProcessor::heightfieldChanged() {
    if (!inportHeightfield_.isConnected())
        return;

    std::ostringstream str;
    const Image *img = inportHeightfield_.getData();
    const DataFormatBase* format = img->getDataFormat();

    str << "Heightfield Port Properties:"
        << "\ndim: " << glm::to_string(img->getDimension())
        << "\nType: " << img->getImageType()
        << "\nNum Color Layers: " << img->getNumberOfColorLayers()
        << std::endl << std::endl
        << "Format:"
        << "\nName: " << format->getString()
        << "\nComponents: " << format->getComponents()
        ;

    LogInfo(str.str());
}


} // namespace
