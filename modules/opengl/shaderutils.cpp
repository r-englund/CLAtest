#include "shaderutils.h"

namespace inviwo {

void addShaderDefines(Shader* shader, const SimpleLightingProperty& property) {
    std::string shadingKey =
        "RC_APPLY_SHADING(colorAmb, colorDiff, colorSpec, samplePos, gradient, lightPos, "
        "cameraPos)";
    std::string shadingValue = "";

    if (property.shadingMode_.isSelectedIdentifier("none"))
        shadingValue = "colorAmb;";
    else if (property.shadingMode_.isSelectedIdentifier("ambient"))
        shadingValue = "shadeAmbient(colorAmb);";
    else if (property.shadingMode_.isSelectedIdentifier("diffuse"))
        shadingValue = "shadeDiffuse(colorDiff, samplePos, gradient, lightPos);";
    else if (property.shadingMode_.isSelectedIdentifier("specular"))
        shadingValue = "shadeSpecular(colorSpec, samplePos, gradient, lightPos, cameraPos);";
    else if (property.shadingMode_.isSelectedIdentifier("phong"))
        shadingValue =
        "shadePhong(colorAmb, colorDiff, colorSpec, samplePos, gradient, lightPos, cameraPos);";

    shader->getFragmentShaderObject()->addShaderDefine(shadingKey, shadingValue);

}

void setShaderUniforms(Shader* shader, const SimpleLightingProperty& property) {
    shader->setUniform("lightPosition_", property.lightPosition_.get());
    shader->setUniform("lightColorAmbient_", property.lightColorAmbient_.get());
    shader->setUniform("lightColorDiffuse_", property.lightColorDiffuse_.get());
    shader->setUniform("lightColorSpecular_", property.lightColorSpecular_.get());
    shader->setUniform("lightSpecularExponent_", property.lightSpecularExponent_.get());
}


void addShaderDefines(Shader* shader, const CameraProperty& property) {}

void setShaderUniforms(Shader* shader, const CameraProperty& property) {
    shader->setUniform("viewMatrix_", property.viewMatrix());
    shader->setUniform("cameraPosition_", property.getLookFrom());
    shader->setUniform("zNear_", property.getNearPlaneDist());
    shader->setUniform("zFar_", property.getFarPlaneDist());
}


void addShaderDefines(Shader* shader, const SimpleRaycastingProperty& property) {
    // gradient computation defines
    std::string gradientComputationKey =
        "RC_CALC_GRADIENTS(voxel, samplePos, volume_, volumeStruct_, t, rayDirection, "
        "entryPoints_, entryParameters_)";
    std::string gradientComputationValue = "";

    if (property.gradientComputationMode_.isSelectedIdentifier("none"))
        gradientComputationValue = "voxel.xyz;";
    else if (property.gradientComputationMode_.isSelectedIdentifier("forward"))
        gradientComputationValue =
        "gradientForwardDiff(voxel.r, volume_, volumeStruct_, samplePos);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("central"))
        gradientComputationValue =
        "gradientCentralDiff(voxel.r, volume_, volumeStruct_, samplePos);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("central-higher"))
        gradientComputationValue =
        "gradientCentralDiffH(voxel.r, volume_, volumeStruct_, samplePos);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("backward"))
        gradientComputationValue =
        "gradientBackwardDiff(voxel.r, volume_, volumeStruct_, samplePos);";

    shader->getFragmentShaderObject()->addShaderDefine(gradientComputationKey,
                                                        gradientComputationValue);

    gradientComputationKey =
        "RC_CALC_GRADIENTS_FOR_CHANNEL(voxel, samplePos, volume_, volumeStruct_, t, rayDirection, "
        "entryPoints_, entryParameters_, channel_)";
    gradientComputationValue = "";

    if (property.gradientComputationMode_.isSelectedIdentifier("none"))
        gradientComputationValue = "voxel.xyz;";
    else if (property.gradientComputationMode_.isSelectedIdentifier("forward"))
        gradientComputationValue =
        "gradientForwardDiff(voxel.r, volume_, volumeStruct_, samplePos, channel_);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("central"))
        gradientComputationValue =
        "gradientCentralDiff(voxel.r, volume_, volumeStruct_, samplePos, channel_);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("central-higher"))
        gradientComputationValue =
        "gradientCentralDiffH(voxel.r, volume_, volumeStruct_, samplePos, channel_);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("backward"))
        gradientComputationValue =
        "gradientBackwardDiff(voxel.r, volume_, volumeStruct_, samplePos, channel_);";
    shader->getFragmentShaderObject()->addShaderDefine(gradientComputationKey,
                                                        gradientComputationValue);


    // classification defines
    std::string classificationKey = "RC_APPLY_CLASSIFICATION(transferFunc_, voxel)";
    std::string classificationValue = "";
    if (property.classificationMode_.isSelectedIdentifier("none"))
        classificationValue = "vec4(voxel);";
    else if (property.classificationMode_.isSelectedIdentifier("transfer-function"))
        classificationValue = "applyTF(transferFunc_, voxel);";
    shader->getFragmentShaderObject()->addShaderDefine(classificationKey, classificationValue);

    classificationKey = "RC_APPLY_CLASSIFICATION_FOR_CHANNEL(transferFunc_, voxel, channel)";
    classificationValue = "";
    if (property.classificationMode_.isSelectedIdentifier("none"))
        classificationValue = "vec4(voxel);";
    else if (property.classificationMode_.isSelectedIdentifier("transfer-function"))
        classificationValue = "applyTF(transferFunc_, voxel, channel);";
    shader->getFragmentShaderObject()->addShaderDefine(classificationKey, classificationValue);


    // compositing defines
    std::string compositingKey =
        "RC_APPLY_COMPOSITING(result, color, samplePos, voxel, gradient, t, tDepth, tIncr)";
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
        compositingValue = "compositeFHN_VS(result, color, gradient, t, viewMatrix_, tDepth);";
    else if (property.compositingMode_.isSelectedIdentifier("fhd"))
        compositingValue = "compositeFHD(result, color, gradient, t, tDepth);";
    else if (property.compositingMode_.isSelectedIdentifier("iso"))
        compositingValue = "compositeISO(result, color, voxel.r, t, tDepth, tIncr, isoValue_);";
    else if (property.compositingMode_.isSelectedIdentifier("ison"))
        compositingValue = "compositeISON(result, color, voxel.r, gradient, t, tDepth, isoValue_);";

    shader->getFragmentShaderObject()->addShaderDefine(compositingKey, compositingValue);
}
void setShaderUniforms(Shader* shader, const SimpleRaycastingProperty& property) {
    shader->setUniform("samplingRate_", property.samplingRate_.get());
    shader->setUniform("isoValue_", property.isoValue_.get());
}

}  // namespace
