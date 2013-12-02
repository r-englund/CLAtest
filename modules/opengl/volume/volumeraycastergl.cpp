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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include "volumeraycastergl.h"
#include <inviwo/core/interaction/trackball.h>

namespace inviwo {

VolumeRaycasterGL::VolumeRaycasterGL()
    : ProcessorGL()
    , raycastPrg_(0)
    , samplingRate_("samplingRate", "Sampling rate", 2.0f, 1.0f, 10.0f)
    , isoValue_("isoValue", "Iso value", 0.5f, 0.0f, 1.0f)

    , classificationMode_("classificationMode", "Classification", PropertyOwner::INVALID_RESOURCES)
    , gradientComputationMode_("gradientComputationMode", "Gradient computation", PropertyOwner::INVALID_RESOURCES)
    , shadingMode_("shadingMode", "Shading", PropertyOwner::INVALID_RESOURCES)
    , compositingMode_("compositingMode", "Compositing", PropertyOwner::INVALID_RESOURCES)

    , lightPosition_("lightPosition", "Position", vec3(0.5f, 0.5f, 0.5f))
    , lightColorAmbient_("lightColorAmbient", "Ambient color", vec3(0.5f, 0.5f, 0.5f))
    , lightColorDiffuse_("lightColorDiffuse", "Diffuse color", vec3(0.5f, 0.5f, 0.5f))
    , lightColorSpecular_("lightColorSpecular", "Specular color", vec3(0.5f, 0.5f, 0.5f))
    , lightSpecularExponent_("materialShininess", "Material shininess", 110, 0, 180)

    , applyLightAttenuation_("applyLightAttenuation", "Light attenuation", false)
    , lightAttenuation_("lightAttenuation", "Light attenuation values", vec3(0.5f, 0.5f, 0.5f))

    , camera_("camera", "Camera", vec3(0.0f, 0.0f, 3.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))
{
    VolumeRaycasterGL("rc_simple.frag");

    addProperty(camera_);
    addInteractionHandler(new Trackball(&camera_));
}

VolumeRaycasterGL::VolumeRaycasterGL(std::string programFileName)
    : ProcessorGL()
    , raycastPrg_(0)
    , programFileName_(programFileName)
    , samplingRate_("samplingRate", "Sampling rate", 2.0f, 1.0f, 10.0f)
    , isoValue_("isoValue", "Iso value", 0.5f, 0.0f, 1.0f)

    , classificationMode_("classificationMode", "Classification", PropertyOwner::INVALID_RESOURCES)
    , gradientComputationMode_("gradientComputationMode", "Gradient computation", PropertyOwner::INVALID_RESOURCES)
    , shadingMode_("shadingMode", "Shading", PropertyOwner::INVALID_RESOURCES)
    , compositingMode_("compositingMode", "Compositing", PropertyOwner::INVALID_RESOURCES)

    , lightPosition_("lightPosition", "Position", vec3(0.5f, 0.5f, 0.5f))
    , lightColorAmbient_("lightColorAmbient", "Ambient color", vec3(0.5f, 0.5f, 0.5f))
    , lightColorDiffuse_("lightColorDiffuse", "Diffuse color", vec3(0.5f, 0.5f, 0.5f))
    , lightColorSpecular_("lightColorSpecular", "Specular color", vec3(0.5f, 0.5f, 0.5f))
    , lightSpecularExponent_("materialShininess", "Material shininess", 110, 0, 180)

    , applyLightAttenuation_("applyLightAttenuation", "Light attenuation", false)
    , lightAttenuation_("lightAttenuation", "Light attenuation values", vec3(0.5f, 0.5f, 0.5f))

    , camera_("camera", "Camera", vec3(0.0f, 0.0f, -3.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))
{
    addProperty(samplingRate_);

    classificationMode_.addOption("none", "None");
    classificationMode_.addOption("transfer-function", "Transfer function");
    classificationMode_.set("transfer-function");
    addProperty(classificationMode_);

    gradientComputationMode_.addOption("none", "None");
    gradientComputationMode_.addOption("forward", "Forward differences");
    gradientComputationMode_.addOption("central", "Central differences");
    gradientComputationMode_.addOption("central-higher", "Higher Order Central differences");
    gradientComputationMode_.addOption("backward", "Backward differences");
    gradientComputationMode_.set("forward");
    addProperty(gradientComputationMode_);

    // light properties are only initialized here and need to be added by derived raycasters
    shadingMode_.addOption("none", "No Shading");
    shadingMode_.addOption("ambient", "Ambient");
    shadingMode_.addOption("diffuse", "Diffuse");
    shadingMode_.addOption("specular", "Specular");
    shadingMode_.set("diffuse");
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

    //camera_.setVisible(false);
    addProperty(camera_);
    addInteractionHandler(new Trackball(&camera_));
}

void VolumeRaycasterGL::addShadingProperties() {
    // shading
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
    //setPropertyGroupGuiName("lighting", "Lighting Parameters");
}

void VolumeRaycasterGL::initialize() {
    ProcessorGL::initialize();
    raycastPrg_ = new Shader(programFileName_, false);
    initializeResources();
}

void VolumeRaycasterGL::deinitialize() {
    if (raycastPrg_) delete raycastPrg_;
    raycastPrg_ = 0;
    ProcessorGL::deinitialize();
}


void VolumeRaycasterGL::initializeResources() {
    // DEPRECATED: add some empty defines for Voreen compatibility
    raycastPrg_->getFragmentShaderObject()->addShaderDefine("RC_BEGIN_COMPOSITING");
    raycastPrg_->getFragmentShaderObject()->addShaderDefine("RC_END_COMPOSITING");

    // basic loop defines
    std::string beginLoop = "while (t < tEnd)";
    raycastPrg_->getFragmentShaderObject()->addShaderDefine("RC_BEGIN_LOOP", beginLoop);
    std::string endLoop = "if (tDepth == -1.0) tDepth = 1.0; gl_FragDepth = tDepth;";
    raycastPrg_->getFragmentShaderObject()->addShaderDefine("RC_END_LOOP", endLoop);

    // gradient computation defines
    std::string gradientComputationKey = "RC_CALC_GRADIENTS(voxel, samplePos, volume_, volumeStruct_, t, rayDirection, entryPoints_, entryParameters_)";
    std::string gradientComputationValue = "";
    if (gradientComputationMode_.isSelected("none"))
        gradientComputationValue = "voxel.xyz;";
    if (gradientComputationMode_.isSelected("forward"))
        gradientComputationValue = "gradientForwardDiff(voxel.a, volume_, volumeStruct_, samplePos);";
    else if (gradientComputationMode_.isSelected("central"))
        gradientComputationValue = "gradientCentralDiff(voxel.a, volume_, volumeStruct_, samplePos);";
    else if (gradientComputationMode_.isSelected("central-higher"))
        gradientComputationValue = "gradientCentralDiffH(voxel.a, volume_, volumeStruct_, samplePos);";
    else if (gradientComputationMode_.isSelected("backward"))
        gradientComputationValue = "gradientBackwardDiff(voxel.a, volume_, volumeStruct_, samplePos);";

    raycastPrg_->getFragmentShaderObject()->addShaderDefine(gradientComputationKey, gradientComputationValue);

    // classification defines
    std::string classificationKey = "RC_APPLY_CLASSIFICATION(transferFunc_, voxel)";
    std::string classificationValue = "";
    if (classificationMode_.isSelected("none"))
        classificationValue = "vec4(voxel.a);";
    if (classificationMode_.isSelected("transfer-function"))
        classificationValue = "applyTF(transferFunc_, voxel);";
    raycastPrg_->getFragmentShaderObject()->addShaderDefine(classificationKey, classificationValue);

    // shading defines
    std::string shadingKey = "RC_APPLY_SHADING(colorAmb, colorDiff, colorSpec, gradient, lightPos, cameraPos)";
    std::string shadingValue = "";
    if (shadingMode_.isSelected("none"))
        shadingValue = "colorAmb;";
    if (shadingMode_.isSelected("ambient"))
        shadingValue = "shadeAmbient(colorAmb);";
    if (shadingMode_.isSelected("diffuse"))
        shadingValue = "shadeDiffuse(colorDiff, gradient, lightPos);";
    if (shadingMode_.isSelected("specular"))
        shadingValue = "shadeSpecular(colorSpec, gradient, lightPos, cameraPos);";
    raycastPrg_->getFragmentShaderObject()->addShaderDefine(shadingKey, shadingValue);

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
    raycastPrg_->getFragmentShaderObject()->addShaderDefine(compositingKey, compositingValue);

    raycastPrg_->build();
}

void VolumeRaycasterGL::bindTransferFunction(const TransferFunction& tf, GLenum tfTexUnit) {
    const Image* tfImage = tf.getData();
    const ImageGL* transferFunctionGL = tfImage->getRepresentation<ImageGL>();
    transferFunctionGL->bindColorTexture(tfTexUnit);
}

void VolumeRaycasterGL::bindVolume(const VolumeInport& inport, GLenum volTexUnit) {
    const Volume* volume = inport.getData();
    const VolumeGL* volumeGL = volume->getRepresentation<VolumeGL>();
    volumeGL->bindTexture(volTexUnit);
}

void VolumeRaycasterGL::setVolumeParameters(const VolumeInport& inport, Shader* shader, const std::string samplerID) {
    vec3 dimensions = vec3(inport.getData()->getRepresentation<VolumeGL>()->getDimensions());
    shader->setUniform(samplerID + ".dimensions_", dimensions);
    shader->setUniform(samplerID + ".dimensionsRCP_", vec3(1.0f)/dimensions);
}

void VolumeRaycasterGL::setGlobalShaderParameters(Shader* shader) {
    ProcessorGL::setGlobalShaderParameters(shader);
    shader->setUniform("samplingRate_", samplingRate_.get());

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
