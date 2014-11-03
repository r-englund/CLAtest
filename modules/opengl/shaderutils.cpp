/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 * Contact: Erik Sundén
 *
 *********************************************************************************/

#include "shaderutils.h"

namespace inviwo {

namespace utilgl {

void addShaderDefines(Shader* shader, const SimpleLightingProperty& property) {
    // implementations in  modules/opengl/glsl/utils/shading.glsl
    std::string shadingKey =
        "APPLY_LIGHTING(lighting, materialAmbientColor, materialDiffuseColor, materialSpecularColor, position, normal, toCameraDir)";
    std::string shadingValue = "";

    if (property.shadingMode_.isSelectedIdentifier("none"))
        shadingValue = "materialAmbientColor;";
    else if (property.shadingMode_.isSelectedIdentifier("ambient"))
        shadingValue = "shadeAmbient(lighting, materialAmbientColor);";
    else if (property.shadingMode_.isSelectedIdentifier("diffuse"))
        shadingValue = "shadeDiffuse(lighting, materialDiffuseColor, position, normal);";
    else if (property.shadingMode_.isSelectedIdentifier("specular"))
        shadingValue =
            "shadeSpecular(lighting, materialSpecularColor, position, normal, toCameraDir);";
    else if (property.shadingMode_.isSelectedIdentifier("phong"))
        shadingValue =
            "shadePhong(lighting, materialAmbientColor, materialDiffuseColor, materialSpecularColor, position, normal, toCameraDir);";

    shader->getFragmentShaderObject()->addShaderDefine(shadingKey, shadingValue);
}

void setShaderUniforms(Shader* shader, const SimpleLightingProperty& property) {
    shader->setUniform("lightPosition_", property.lightPosition_.get());
    shader->setUniform("lightColor_", property.lightColor_.get());
    shader->setUniform("materialAmbientColor_", property.materialAmbientColor_.get());
    shader->setUniform("materialSpecularColor_", property.materialSpecularColor_.get());
    shader->setUniform("materialSpecularExponent_", property.materialSpecularExponent_.get());
}

void setShaderUniforms(Shader* shader, const SimpleLightingProperty& property, std::string name) {
    shader->setUniform(name + ".lightPosition_", property.lightPosition_.get());
    shader->setUniform(name + ".lightColor_", property.lightColor_.get());
    shader->setUniform(name + ".materialAmbientColor_", property.materialAmbientColor_.get());
    shader->setUniform(name + ".materialSpecularColor_", property.materialSpecularColor_.get());
    shader->setUniform(name + ".materialSpecularExponent_", property.materialSpecularExponent_.get());
}

void addShaderDefines(Shader* shader, const CameraProperty& property) {
}

void setShaderUniforms(Shader* shader, const CameraProperty& property) {
    shader->setUniform("viewMatrix_", property.viewMatrix());
    shader->setUniform("projectionMatrix_", property.projectionMatrix());
    shader->setUniform("worldToClipMatrix_", property.projectionMatrix()*property.viewMatrix()); 
    shader->setUniform("cameraPosition_", property.getLookFrom());
    shader->setUniform("zNear_", property.getNearPlaneDist());
    shader->setUniform("zFar_", property.getFarPlaneDist());
}

void setShaderUniforms(Shader* shader, const CameraProperty& property, std::string name) {
    shader->setUniform(name + ".viewMatrix_", property.viewMatrix());
    shader->setUniform(name + ".projectionMatrix_", property.projectionMatrix());
    shader->setUniform(name + ".worldToClipMatrix_", property.projectionMatrix()*property.viewMatrix()); 
    shader->setUniform(name + ".cameraPosition_", property.getLookFrom());
    shader->setUniform(name + ".zNear_", property.getNearPlaneDist());
    shader->setUniform(name + ".zFar_", property.getFarPlaneDist());
}


void setShaderUniforms(Shader* shader, const CameraProperty& property, const SpatialEntity<3>& object) {
    mat4 modelViewMatrix = property.viewMatrix()*object.getCoordinateTransformer().getModelToWorldMatrix();
    shader->setUniform("modelToViewMatrix_", modelViewMatrix);
    shader->setUniform("modelToClipMatrix_", property.projectionMatrix()*modelViewMatrix);
    shader->setUniform("modelToViewNormalMatrix_", glm::mat3(glm::transpose(glm::inverse(modelViewMatrix))));
}


IVW_MODULE_OPENGL_API void setShaderUniforms(Shader* shader, const SpatialEntity<3>& object) {
    mat4 modelToWorldMatrix = object.getCoordinateTransformer().getModelToWorldMatrix();
    shader->setUniform("modelToWorldMatrix_", modelToWorldMatrix);
    shader->setUniform("modelToWorldNormalMatrix_", glm::mat3(glm::transpose(glm::inverse(modelToWorldMatrix))));
}

IVW_MODULE_OPENGL_API void setShaderUniforms(Shader* shader, const SpatialEntity<3>& object, const std::string& name) {
    mat4 modelToWorldMatrix = object.getCoordinateTransformer().getModelToWorldMatrix();
    shader->setUniform(name + ".modelToWorldMatrix_", modelToWorldMatrix);
    shader->setUniform(name + ".modelToWorldNormalMatrix_", glm::mat3(glm::transpose(glm::inverse(modelToWorldMatrix))));
}





void addShaderDefines(Shader* shader, const SimpleRaycastingProperty& property) {
    std::string gradientComputationKey = "";
    std::string gradientComputationValue = "";

    // Compute the gradient for channel 1;
    gradientComputationKey =
        "COMPUTE_GRADIENT(voxel, volume, volumeParams, samplePos)";
    gradientComputationValue = "";

    if (property.gradientComputationMode_.isSelectedIdentifier("none"))
        gradientComputationValue = "voxel.xyz;";
    else if (property.gradientComputationMode_.isSelectedIdentifier("forward"))
        gradientComputationValue =
        "gradientForwardDiff(voxel, volume, volumeParams, samplePos, 0);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("central"))
        gradientComputationValue =
        "gradientCentralDiff(voxel, volume, volumeParams, samplePos, 0);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("central-higher"))
        gradientComputationValue =
        "gradientCentralDiffH(voxel, volume, volumeParams, samplePos, 0);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("backward"))
        gradientComputationValue =
        "gradientBackwardDiff(voxel, volume, volumeParams, samplePos, 0);";

    shader->getFragmentShaderObject()->addShaderDefine(gradientComputationKey,
                                                       gradientComputationValue);


    gradientComputationKey =
        "COMPUTE_GRADIENT_FOR_CHANNEL(voxel, volume, volumeParams, samplePos, channel)";
    gradientComputationValue = "";

    if (property.gradientComputationMode_.isSelectedIdentifier("none"))
        gradientComputationValue = "voxel.xyz;";
    else if (property.gradientComputationMode_.isSelectedIdentifier("forward"))
        gradientComputationValue =
        "gradientForwardDiff(voxel, volume, volumeParams, samplePos, channel);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("central"))
        gradientComputationValue =
        "gradientCentralDiff(voxel, volume, volumeParams, samplePos, channel);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("central-higher"))
        gradientComputationValue =
        "gradientCentralDiffH(voxel, volume, volumeParams, samplePos, channel);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("backward"))
        gradientComputationValue =
        "gradientBackwardDiff(voxel, volume, volumeParams, samplePos, channel);";
    shader->getFragmentShaderObject()->addShaderDefine(gradientComputationKey,
                                                        gradientComputationValue);


    gradientComputationKey =
        "COMPUTE_ALL_GRADIENTS(voxel, volume, volumeParams, samplePos)";
    gradientComputationValue = "";

    if (property.gradientComputationMode_.isSelectedIdentifier("none"))
        gradientComputationValue = "mat4x3(0)";
    else if (property.gradientComputationMode_.isSelectedIdentifier("forward"))
        gradientComputationValue =
        "gradientAllForwardDiff(voxel, volume, volumeParams, samplePos);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("central"))
        gradientComputationValue =
        "gradientAllCentralDiff(voxel, volume, volumeParams, samplePos);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("central-higher"))
        gradientComputationValue =
        "gradientAllCentralDiffH(voxel, volume, volumeParams, samplePos);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("backward"))
        gradientComputationValue =
        "gradientAllBackwardDiff(voxel, volume, volumeParams, samplePos);";
    shader->getFragmentShaderObject()->addShaderDefine(gradientComputationKey,
                                                       gradientComputationValue);


    // classification defines
    std::string classificationKey = "APPLY_CLASSIFICATION(transferFunc, voxel)";
    std::string classificationValue = "";
    if (property.classificationMode_.isSelectedIdentifier("none"))
        classificationValue = "vec4(voxel);";
    else if (property.classificationMode_.isSelectedIdentifier("transfer-function"))
        classificationValue = "applyTF(transferFunc, voxel);";
    shader->getFragmentShaderObject()->addShaderDefine(classificationKey, classificationValue);

    // compositing defines
    std::string compositingKey =
        "APPLY_COMPOSITING(result, color, samplePos, voxel, gradient, camera, isoValue, t, tDepth, tIncr)";
    std::string compositingValue;

    if (property.compositingMode_.isSelectedIdentifier("dvr"))
        compositingValue = "compositeDVR(result, color, t, tDepth, tIncr);";
    else if (property.compositingMode_.isSelectedIdentifier("mip"))
        compositingValue = "compositeMIP(result, color, t, tDepth);";
    else if (property.compositingMode_.isSelectedIdentifier("fhp"))
        compositingValue = "compositeFHP(result, color, samplePos, t, tDepth);";
    else if (property.compositingMode_.isSelectedIdentifier("fhn"))
        compositingValue = "compositeFHN(result, color, gradient, t, tDepth);";
    else if (property.compositingMode_.isSelectedIdentifier("fhnvs"))
        compositingValue = "compositeFHN_VS(result, color, gradient, t, camera, tDepth);";
    else if (property.compositingMode_.isSelectedIdentifier("fhd"))
        compositingValue = "compositeFHD(result, color, t, tDepth);";
    else if (property.compositingMode_.isSelectedIdentifier("iso"))
        compositingValue = "compositeISO(result, color, voxel.r, t, tDepth, tIncr, isoValue);";
    else if (property.compositingMode_.isSelectedIdentifier("ison"))
        compositingValue = "compositeISON(result, color, voxel.r, gradient, t, tDepth, isoValue);";

    shader->getFragmentShaderObject()->addShaderDefine(compositingKey, compositingValue);
}
void setShaderUniforms(Shader* shader, const SimpleRaycastingProperty& property) {
    shader->setUniform("samplingRate_", property.samplingRate_.get());
    shader->setUniform("isoValue_", property.isoValue_.get());
}

} // namspace utilgl

}  // namespace
