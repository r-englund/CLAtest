/**********************************************************************
 * Copyright (C) 2013-2014 Scientific Visualization Group - Linköping University
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

#include "lightvolumegl.h"
#include <modules/opengl/volume/volumegl.h>
#include <inviwo/core/datastructures/light/diffuselight.h>
#include <inviwo/core/datastructures/light/directionallight.h>

namespace inviwo {

ProcessorClassName(LightVolumeGL, "LightVolumeGL"); 
ProcessorCategory(LightVolumeGL, "Illumination");
ProcessorCodeState(LightVolumeGL, CODE_STATE_EXPERIMENTAL);

static const int faceAxis_[6] = { 
    0, 0, 1, 1, 2, 2 
};

static const vec3 faceCenters_[6] = {
    vec3(1.f, 0.5f, 0.5f),
    vec3(0.f, 0.5f, 0.5f),
    vec3(0.5f, 1.f, 0.5f),
    vec3(0.5f, 0.f, 0.5f),
    vec3(0.5f, 0.5f, 1.f),
    vec3(0.5f, 0.5f, 0.f)
};

static const vec3 faceNormals_[6] = {
    vec3(1.f, 0.f, 0.f),
    vec3(-1.f, 0.f, 0.f),
    vec3(0.f, 1.f, 0.f),
    vec3(0.f, -1.f, 0.f),
    vec3(0.f, 0.f, 1.f),
    vec3(0.f, 0.f, -1.f)
};

//Defines permutation axis based on face index for chosen propagation axis.
inline void definePermutationMatrices(mat4& permMat, mat4& permLightMat, int permFaceIndex) { 
    permMat = mat4(0.f);
    permMat[3][3] = 1.f;

    //Permutation of x and y
    switch(faceAxis_[permFaceIndex]){
        case 0:
            permMat[0][2] = 1.f;
            permMat[1][1] = 1.f;
            break;
        case 1:
            permMat[0][0] = 1.f;
            permMat[1][2] = 1.f;
            break;
        case 2:
            permMat[0][0] = 1.f;
            permMat[1][1] = 1.f;
            break;
    }
    permLightMat = permMat;

    //Permutation of z
    float closestAxisZ = (faceNormals_[permFaceIndex])[faceAxis_[permFaceIndex]];
    permMat[2][faceAxis_[permFaceIndex]] = closestAxisZ;
    permLightMat[2][faceAxis_[permFaceIndex]] = glm::abs<float>(closestAxisZ);

    //For reverse axis-aligned
    if(closestAxisZ < 0.f){
        permMat[3][faceAxis_[permFaceIndex]] = 1.f;
    }
}

LightVolumeGL::LightVolumeGL()
    : ProcessorGL(),
      inport_("inport"),
      outport_("outport"),
      lightSource_("lightSource"),
      supportColoredLight_("supportColoredLight", "Support Light Color", false),
      volumeSizeOption_("volumeSizeOption", "Light Volume Size"),
      transferFunction_("transferFunction", "Transfer function", TransferFunction()),
      propagationShader_(NULL),
      mergeShader_(NULL),
      mergeFBO_(NULL),
      internalVolumesInvalid_(false),
      volumeDimOut_(uvec3(0)),
      lightPos_(vec3(0.f)),
      lightColor_(vec4(1.f)),
      calculatedOnes_(false)
{
    addPort(inport_);
    addPort(outport_);

    addPort(lightSource_);

    supportColoredLight_.onChange(this, &LightVolumeGL::supportColoredLightChanged);
    addProperty(supportColoredLight_);

    volumeSizeOption_.addOption("1","Full of incoming volume", 1);
    volumeSizeOption_.addOption("1/2","Half of incoming volume", 2);
    volumeSizeOption_.addOption("1/4","Quarter of incoming volume", 4);
    volumeSizeOption_.setSelectedOption(1);
    volumeSizeOption_.onChange(this, &LightVolumeGL::volumeSizeOptionChanged);
    addProperty(volumeSizeOption_);

    addProperty(transferFunction_);
}

LightVolumeGL::~LightVolumeGL() {
}

void LightVolumeGL::initialize() {
    ProcessorGL::initialize();

    propagationShader_ = new Shader("lighting/lightpropagation.vert", "lighting/lightpropagation.geom", "lighting/lightpropagation.frag", true);
    mergeShader_ = new Shader("lighting/lightvolumeblend.vert", "lighting/lightvolumeblend.geom", "lighting/lightvolumeblend.frag", true);

    for(int i=0; i<2; ++i){
        propParams_[i].fbo = new FrameBufferObject();
    }

    mergeFBO_ = new FrameBufferObject();

    supportColoredLightChanged();
}

void LightVolumeGL::deinitialize() {
    ProcessorGL::deinitialize();

    delete propagationShader_;
    propagationShader_ = NULL;

    delete mergeShader_;
    mergeShader_ = NULL;

    for(int i=0; i<2; ++i){
        delete propParams_[i].fbo;
        propParams_[i].fbo = NULL;
    }

    delete mergeFBO_;
    mergeFBO_ = NULL;
}

void LightVolumeGL::propagation3DTextureParameterFunction(Texture*){
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_BORDER);
    borderColorTextureParameterFunction();
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void LightVolumeGL::process() {
    bool lightColorChanged = false;
    if(lightSource_.getInvalidationLevel() >= INVALID_OUTPUT) {
        lightColorChanged = lightSourceChanged();
    }

    bool reattach = false;
    if(internalVolumesInvalid_ || lightColorChanged || inport_.getInvalidationLevel() >= INVALID_OUTPUT) {
        reattach = volumeChanged(lightColorChanged);
    }

    VolumeGL* outVolumeGL = outport_.getData()->getEditableRepresentation<VolumeGL>();

    TextureUnit volUnit;
    const VolumeGL* inVolumeGL = inport_.getData()->getRepresentation<VolumeGL>();
    inVolumeGL->bindTexture(volUnit.getEnum());

    TextureUnit transFuncUnit;
    const Layer* tfLayer = transferFunction_.get().getData();
    const LayerGL* transferFunctionGL = tfLayer->getRepresentation<LayerGL>();
    transferFunctionGL->bindTexture(transFuncUnit.getEnum());

    TextureUnit lightVolUnit[2];
    propParams_[0].vol->bindTexture(lightVolUnit[0].getEnum());
    propParams_[1].vol->bindTexture(lightVolUnit[1].getEnum());

    propagationShader_->activate();

    propagationShader_->setUniform("volume_", volUnit.getUnitNumber());
    propagationShader_->setUniform("volumeParameters_.dimensions_", volumeDimInF_);
    propagationShader_->setUniform("volumeParameters_.dimensionsRCP_", volumeDimInFRCP_);

    propagationShader_->setUniform("transferFunc_", transFuncUnit.getUnitNumber());

    propagationShader_->setUniform("lightVolumeParameters_.dimensions_", volumeDimOutF_);
    propagationShader_->setUniform("lightVolumeParameters_.dimensionsRCP_", volumeDimOutFRCP_);

    CanvasGL::enableDrawImagePlaneRect();

    //Perform propagation passes
    for(int i=0; i<2; ++i){
        propParams_[i].fbo->activate();
        glViewport(0, 0, volumeDimOut_.x, volumeDimOut_.y);

        if(reattach)
            propParams_[i].fbo->attachColorTexture(propParams_[i].vol->getTexture(), 0);

        propagationShader_->setUniform("lightVolume_", lightVolUnit[i].getUnitNumber());
        propagationShader_->setUniform("permutationMatrix_", propParams_[i].axisPermutation);
        propagationShader_->setUniform("permutedLightDirection_", propParams_[i].permutedLightDirection);

        for(unsigned int z=0; z<volumeDimOut_.z; ++z){
            glFramebufferTexture3DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_3D, propParams_[i].vol->getTexture()->getID(), 0, z);
            propagationShader_->setUniform("sliceNum_", static_cast<GLint>(z));
            CanvasGL::singleDrawImagePlaneRect();
        }

        propParams_[i].fbo->deactivate();
    }

    CanvasGL::disableDrawImagePlaneRect();

    propagationShader_->deactivate();

    mergeShader_->activate();

    mergeShader_->setUniform("lightVolume_", lightVolUnit[0].getUnitNumber());
    mergeShader_->setUniform("lightVolumeSec_", lightVolUnit[1].getUnitNumber());

    mergeShader_->setUniform("lightVolumeParameters_.dimensions_", volumeDimOutF_);
    mergeShader_->setUniform("lightVolumeParameters_.dimensionsRCP_", volumeDimOutFRCP_);

    mergeShader_->setUniform("permMatInv_", propParams_[0].axisPermutationINV);
    mergeShader_->setUniform("permMatInvSec_", propParams_[1].axisPermutationINV);

    mergeShader_->setUniform("blendingFactor_", blendingFactor_);

    //Perform merge pass
    mergeFBO_->activate();
    glViewport(0, 0, volumeDimOut_.x, volumeDimOut_.y);

    if(reattach)
        mergeFBO_->attachColorTexture(outVolumeGL->getTexture(), 0);

    CanvasGL::renderImagePlaneRect(static_cast<int>(volumeDimOut_.z));

    mergeShader_->deactivate();

    mergeFBO_->deactivate();
}

bool LightVolumeGL::lightSourceChanged(){
    updatePermuationMatrices(lightSource_.getData()->getLightDirection(), &propParams_[0], &propParams_[1]);

    vec3 color = vec3(1.f);

    const DirectionalLight* directionLight = dynamic_cast<const DirectionalLight*>(lightSource_.getData());
    if(directionLight){
        color = directionLight->getIntensity();
    }
    else{
        const DiffuseLight* diffuseLight = dynamic_cast<const DiffuseLight*>(lightSource_.getData());
        if(diffuseLight){
            color = diffuseLight->getIntensity();
        }
    }

    if(color != lightColor_.xyz()){
        lightColor_.x = color.x;
        lightColor_.y = color.y;
        lightColor_.z = color.z;
        return true;
    }

    return false;
}

bool LightVolumeGL::volumeChanged(bool lightColorChanged){
    const Volume* input = inport_.getData();
    glm::uvec3 inDim = input->getDimension();
    glm::uvec3 outDim = uvec3(inDim.x/volumeSizeOption_.get(), inDim.y/volumeSizeOption_.get(), inDim.z/volumeSizeOption_.get());

    if(internalVolumesInvalid_ || (volumeDimOut_ != outDim)){
        volumeDimOut_ = outDim;
        volumeDimOutF_ = vec3(volumeDimOut_);
        volumeDimOutFRCP_ = vec3(1.0f)/volumeDimOutF_;
        volumeDimInF_ = vec3(inDim);
        volumeDimInFRCP_ = vec3(1.0f)/volumeDimInF_;

        const DataFormatBase* format;
        if(supportColoredLight_.get())
            format = DataVec4UINT8::get();
        else
            format = DataUINT8::get();

        for(int i=0; i<2; ++i){
            if(!propParams_[i].vol || propParams_[i].vol->getDataFormat() != format){
                if(propParams_[i].vol)
                    delete propParams_[i].vol;

                propParams_[i].vol = new VolumeGL(volumeDimOut_, format);
                propParams_[i].vol->getTexture()->setTextureParameterFunction(this, &LightVolumeGL::propagation3DTextureParameterFunction);
                propParams_[i].vol->getTexture()->initialize(NULL);
            }
            else{
                propParams_[i].vol->setDimension(volumeDimOut_);
            }
        }

        Volume* volume = new Volume(volumeDimOut_, format);
        outport_.setData(volume);

        internalVolumesInvalid_ = false;

        return true;
    }
    else if(lightColorChanged){
        for(int i=0; i<2; ++i){
            propParams_[i].vol->getTexture()->bind();
            borderColorTextureParameterFunction();
            propParams_[i].vol->getTexture()->unbind();
        }
    }

    return false;
}

void LightVolumeGL::volumeSizeOptionChanged(){
    if(inport_.hasData()){
        if((inport_.getData()->getDimension()/uvec3(volumeSizeOption_.get())) != volumeDimOut_){
            internalVolumesInvalid_ = true;
        }
    }
}

void LightVolumeGL::supportColoredLightChanged(){
    if(propagationShader_){
        if(supportColoredLight_.get())
            propagationShader_->getFragmentShaderObject()->addShaderDefine("SUPPORT_LIGHT_COLOR");
        else
            propagationShader_->getFragmentShaderObject()->removeShaderDefine("SUPPORT_LIGHT_COLOR");

        propagationShader_->build();
    }

    if(outport_.hasData()){
        int components = outport_.getData()->getDataFormat()->getComponents();
        if((components < 3 && supportColoredLight_.get()) || (components > 1 && !supportColoredLight_.get()))
            internalVolumesInvalid_ = true;
    }
}

void LightVolumeGL::borderColorTextureParameterFunction(){
    glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(lightColor_));
}

void LightVolumeGL::updatePermuationMatrices(const vec3& lightPos, PropagationParameters* closest, PropagationParameters* secondClosest){
    if(calculatedOnes_ && lightPos==lightPos_)
        return;

    vec3 invertedLightPos = -lightPos;
    vec3 invertedLightPosNorm = glm::normalize(invertedLightPos);

    //Calculate closest and second closest axis-aligned face.
    float thisVal = glm::dot(faceNormals_[0], invertedLightPosNorm - faceCenters_[0]);
    float closestVal = thisVal;
    float secondClosestVal = 0.f;
    int closestFace = 0;
    int secondClosestFace = -1;
    for(int i=1; i<6; ++i){
        thisVal = glm::dot(faceNormals_[i], invertedLightPosNorm - faceCenters_[i]);
        if(thisVal < closestVal){
            secondClosestVal = closestVal;
            secondClosestFace = closestFace;
            closestVal = thisVal;
            closestFace = i;
        }
        else if(thisVal < secondClosestVal){
            secondClosestVal = thisVal;
            secondClosestFace = i;
        }
    }

    vec4 tmpLightDir = vec4(lightPos.x, lightPos.y, lightPos.z, 1.f);

    //Perform permutation calculation for closest face
    definePermutationMatrices(closest->axisPermutation, closest->axisPermutationLight, closestFace);
    //LogInfo("1st Axis Permutation: " << closest->axisPermutation);
    closest->axisPermutationINV = glm::inverse(closest->axisPermutation);
    closest->permutedLightDirection = closest->axisPermutationLight * tmpLightDir;

    //Perform permutation calculation for second closest face
    definePermutationMatrices(secondClosest->axisPermutation, secondClosest->axisPermutationLight, secondClosestFace);
    //LogInfo("2nd Axis Permutation: " << secondClosest->axisPermutation);
    secondClosest->axisPermutationINV = glm::inverse(secondClosest->axisPermutation);
    secondClosest->permutedLightDirection = secondClosest->axisPermutationLight * tmpLightDir;

    //Calculate the blending factor
    blendingFactor_ = static_cast<float>(1.f-(2.f*glm::acos<float>(glm::dot(invertedLightPosNorm, -faceNormals_[closestFace]))/M_PI));
    //LogInfo("Blending Factor: " << blendingFactor_);

    calculatedOnes_ = true;
}

} // namespace
