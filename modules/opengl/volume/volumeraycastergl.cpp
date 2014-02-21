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
 * Main file author: Timo Ropinski
 *
 *********************************************************************************/

#include "volumeraycastergl.h"

namespace inviwo {

VolumeRaycasterGL::VolumeRaycasterGL()
    : ProcessorGL()
    , shader_(0)
    , shaderFileName_("rc_simple.frag")
    , samplingRate_("samplingRate", "Sampling rate", 2.0f, 1.0f, 10.0f)
    , isoValue_("isoValue", "Iso value", 0.5f, 0.0f, 1.0f)

    , classificationMode_("classificationMode", "Classification", PropertyOwner::INVALID_RESOURCES)
    , gradientComputationMode_("gradientComputationMode", "Gradient computation", PropertyOwner::INVALID_RESOURCES)
    , shadingMode_("shadingMode", "Shading", PropertyOwner::INVALID_RESOURCES)
    , compositingMode_("compositingMode", "Compositing", PropertyOwner::INVALID_RESOURCES)

    , lightPosition_("lightPosition", "Position", vec3(0.0f, 0.7071f, 0.7071f) , vec3(-10,-10,-10) , vec3(10,10,10))
    , lightColorAmbient_("lightColorAmbient", "Ambient color", vec3(0.5f, 0.5f, 0.5f))
    , lightColorDiffuse_("lightColorDiffuse", "Diffuse color", vec3(0.5f, 0.5f, 0.5f))
    , lightColorSpecular_("lightColorSpecular", "Specular color", vec3(0.5f, 0.5f, 0.5f))
    , lightSpecularExponent_("materialShininess", "Material shininess", 110, 0, 180)
    , applyLightAttenuation_("applyLightAttenuation", "Light attenuation", false)
    , lightAttenuation_("lightAttenuation", "Light attenuation values", vec3(0.5f, 0.5f, 0.5f))

    , camera_("camera", "Camera", vec3(0.0f, 0.0f, 3.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)) {
    addBasicProperties();
}

VolumeRaycasterGL::VolumeRaycasterGL(std::string programFileName)
    : ProcessorGL()
    , shader_(0)
    , shaderFileName_(programFileName)
    , samplingRate_("samplingRate", "Sampling rate", 2.0f, 1.0f, 10.0f)
    , isoValue_("isoValue", "Iso value", 0.5f, 0.0f, 1.0f)

    , classificationMode_("classificationMode", "Classification", PropertyOwner::INVALID_RESOURCES)
    , gradientComputationMode_("gradientComputationMode", "Gradient computation", PropertyOwner::INVALID_RESOURCES)
    , shadingMode_("shadingMode", "Shading", PropertyOwner::INVALID_RESOURCES)
    , compositingMode_("compositingMode", "Compositing", PropertyOwner::INVALID_RESOURCES)

    , lightPosition_("lightPosition", "Position", vec3(0.0f, 0.7071f, 0.7071f) , vec3(-10,-10,-10) , vec3(10,10,10))
    , lightColorAmbient_("lightColorAmbient", "Ambient color", vec3(0.5f, 0.5f, 0.5f))
    , lightColorDiffuse_("lightColorDiffuse", "Diffuse color", vec3(0.5f, 0.5f, 0.5f))
    , lightColorSpecular_("lightColorSpecular", "Specular color", vec3(0.5f, 0.5f, 0.5f))
    , lightSpecularExponent_("materialShininess", "Material shininess", 110, 0, 180)
    , applyLightAttenuation_("applyLightAttenuation", "Light attenuation", false)
    , lightAttenuation_("lightAttenuation", "Light attenuation values", vec3(0.5f, 0.5f, 0.5f))

    , camera_("camera", "Camera", vec3(0.0f, 0.0f, -3.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)) {
    addBasicProperties();
}

void VolumeRaycasterGL::addBasicProperties() {
    addProperty(samplingRate_);
    classificationMode_.addOption("none", "None");
    classificationMode_.addOption("transfer-function", "Transfer function");
    classificationMode_.set("transfer-function");
    addProperty(classificationMode_);
    gradientComputationMode_.addOption("none", "None");
    gradientComputationMode_.addOption("forward", "Forward differences");
    gradientComputationMode_.addOption("backward", "Backward differences");
    gradientComputationMode_.addOption("central", "Central differences");
    gradientComputationMode_.addOption("central-higher", "Higher order central differences");
    gradientComputationMode_.set("forward");
    addProperty(gradientComputationMode_);
    // light properties are only initialized here and need to be added by derived raycasters
    shadingMode_.addOption("none", "No Shading");
    shadingMode_.addOption("ambient", "Ambient");
    shadingMode_.addOption("diffuse", "Diffuse");
    shadingMode_.addOption("specular", "Specular");
    shadingMode_.addOption("phong", "Phong");
    shadingMode_.set("phong");
    lightPosition_.setSemantics(PropertySemantics::LightPosition);
    lightColorAmbient_.setSemantics(PropertySemantics::Color);
    lightColorDiffuse_.setSemantics(PropertySemantics::Color);
    lightColorSpecular_.setSemantics(PropertySemantics::Color);
    compositingMode_.addOption("dvr", "Direct volume rendering");
    compositingMode_.addOption("mip", "Maximum intensity projection");
    compositingMode_.addOption("fhp", "First hit points");
    compositingMode_.addOption("fhn", "First hit normals");
    compositingMode_.addOption("iso", "Iso surface rendering");
    compositingMode_.addOption("ison", "Iso surface normal rendering");
    compositingMode_.set("dvr");
    addProperty(compositingMode_);
    addProperty(camera_);
}

void VolumeRaycasterGL::addShadingProperties() {
    // add shading properties
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
    Property::setGroupDisplayName("lighting", "Lighting Parameters");
}

void VolumeRaycasterGL::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader(shaderFileName_, false);
    initializeResources();
}

void VolumeRaycasterGL::deinitialize() {
    if (shader_) delete shader_;

    shader_ = 0;
    ProcessorGL::deinitialize();
}


void VolumeRaycasterGL::initializeResources() {
    // deprecated: the following two lines have been added for downward compability
    shader_->getFragmentShaderObject()->addShaderDefine("RC_BEGIN_COMPOSITING");
    shader_->getFragmentShaderObject()->addShaderDefine("RC_END_COMPOSITING");
    // basic loop defines
    std::string beginLoop = "while (t < tEnd)";
    shader_->getFragmentShaderObject()->addShaderDefine("RC_BEGIN_LOOP", beginLoop);
    std::string endLoop = "if (tDepth == -1.0) tDepth = 1.0; gl_FragDepth = tDepth;";
    shader_->getFragmentShaderObject()->addShaderDefine("RC_END_LOOP", endLoop);
    // gradient computation defines
    std::string gradientComputationKey =
        "RC_CALC_GRADIENTS(voxel, samplePos, volume_, volumeStruct_, t, rayDirection, entryPoints_, entryParameters_)";
    std::string gradientComputationValue = "";

    if (gradientComputationMode_.isSelected("none"))
        gradientComputationValue = "voxel.xyz;";
    else if (gradientComputationMode_.isSelected("forward"))
        gradientComputationValue = "gradientForwardDiff(voxel.r, volume_, volumeStruct_, samplePos);";
    else if (gradientComputationMode_.isSelected("central"))
        gradientComputationValue = "gradientCentralDiff(voxel.r, volume_, volumeStruct_, samplePos);";
    else if (gradientComputationMode_.isSelected("central-higher"))
        gradientComputationValue = "gradientCentralDiffH(voxel.r, volume_, volumeStruct_, samplePos);";
    else if (gradientComputationMode_.isSelected("backward"))
        gradientComputationValue = "gradientBackwardDiff(voxel.r, volume_, volumeStruct_, samplePos);";

    shader_->getFragmentShaderObject()->addShaderDefine(gradientComputationKey, gradientComputationValue);
    // classification defines
    std::string classificationKey = "RC_APPLY_CLASSIFICATION(transferFunc_, voxel)";
    std::string classificationValue = "";

    if (classificationMode_.isSelected("none"))
        classificationValue = "vec4(voxel.r);";
    else if (classificationMode_.isSelected("transfer-function"))
        classificationValue = "applyTF(transferFunc_, voxel);";

    shader_->getFragmentShaderObject()->addShaderDefine(classificationKey, classificationValue);
    // shading defines
    std::string shadingKey = "RC_APPLY_SHADING(colorAmb, colorDiff, colorSpec, samplePos, gradient, lightPos, cameraPos)";
    std::string shadingValue = "";

    if (shadingMode_.isSelected("none"))
        shadingValue = "colorAmb;";
    else if (shadingMode_.isSelected("ambient"))
        shadingValue = "shadeAmbient(colorAmb);";
    else if (shadingMode_.isSelected("diffuse"))
        shadingValue = "shadeDiffuse(colorDiff, samplePos, gradient, lightPos);";
    else if (shadingMode_.isSelected("specular"))
        shadingValue = "shadeSpecular(colorSpec, samplePos, gradient, lightPos, cameraPos);";
    else if (shadingMode_.isSelected("phong"))
        shadingValue = "shadePhong(colorAmb, colorDiff, colorSpec, samplePos, gradient, lightPos, cameraPos);";

    shader_->getFragmentShaderObject()->addShaderDefine(shadingKey, shadingValue);
    // compositing defines
    std::string compositingKey = "RC_APPLY_COMPOSITING(result, color, samplePos, gradient, t, tDepth, tIncr)";
    std::string compositingValue = "";

    if (compositingMode_.isSelected("dvr"))
        compositingValue = "compositeDVR(result, color, t, tDepth, tIncr);";
    else if (compositingMode_.isSelected("mip"))
        compositingValue = "compositeMIP(result, color, t, tDepth);";
    else if (compositingMode_.isSelected("fhp"))
        compositingValue = "compositeFHP(result, color, samplePos, t, tDepth);";
    else if (compositingMode_.isSelected("fhn"))
        compositingValue = "compositeFHN(result, color, gradient, t, tDepth);";
    else if (compositingMode_.isSelected("iso"))
        compositingValue = "compositeISO(result, color, t, tDepth, tIncr, isoValue_);";
    else if (compositingMode_.isSelected("ison"))
        compositingValue = "compositeISON(result, color, gradient, t, tDepth, isoValue_);";

    shader_->getFragmentShaderObject()->addShaderDefine(compositingKey, compositingValue);
    shader_->build();
}

void VolumeRaycasterGL::bindTransferFunction(const TransferFunction& tf, GLenum tfTexUnit) {
    const Layer* tfLayer = tf.getData();
    const LayerGL* transferFunctionGL = tfLayer->getRepresentation<LayerGL>();
    transferFunctionGL->bindTexture(tfTexUnit);
}

void VolumeRaycasterGL::bindVolume(const VolumeInport& inport, GLenum volTexUnit) {
    const Volume* volume = inport.getData();
    const VolumeGL* volumeGL = volume->getRepresentation<VolumeGL>();
    volumeGL->bindTexture(volTexUnit);
}

void VolumeRaycasterGL::setVolumeParameters(const VolumeInport& inport, Shader* shader, const std::string samplerID) {
    const VolumeGL* volGL = inport.getData()->getRepresentation<VolumeGL>();
    volGL->setVolumeUniforms(inport.getData(), shader, samplerID);
    mat4 viewToVoxel = inport.getData()->getCoordinateTransformer().getWorldToTextureMatrix();
    shader->setUniform("viewToVoxel_", viewToVoxel);
    shader->setUniform("viewMatrix_", camera_.viewMatrix());
}

void VolumeRaycasterGL::setGlobalShaderParameters(Shader* shader) {
    ProcessorGL::setGlobalShaderParameters(shader);
    // sampling uniform
    shader->setUniform("samplingRate_", samplingRate_.get());
    // camera uniform
    shader->setUniform("cameraPosition_", camera_.getLookFrom());
    // illumination uniforms
    shader->setUniform("lightPosition_", lightPosition_.get());
    shader->setUniform("lightColorAmbient_", lightColorAmbient_.get());
    shader->setUniform("lightColorDiffuse_", lightColorDiffuse_.get());
    shader->setUniform("lightColorSpecular_", lightColorSpecular_.get());
    shader->setUniform("lightSpecularExponent_", lightSpecularExponent_.get());
    // depth computation uniforms
    shader->setUniform("zNear_", camera_.getNearPlaneDist());
    shader->setUniform("zFar_", camera_.getFarPlaneDist());
}

} // namespace
