#include "shaderutils.h"

namespace inviwo {

namespace utilgl {

void addShaderDefines(Shader* shader, const SimpleLightingProperty& property) {
    // This version is depricated. // opengl/glsl/include/inc_shading.frag
    std::string shadingKey =
        "APPLY_SHADING(colorAmb, colorDiff, colorSpec, samplePos, gradient, lightPos, "
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


    // New version // opengl/glsl/utils/shading.frag
    shadingKey =
        "APPLY_LIGHTING(light, camera, volume, colorAmb, colorDiff, colorSpec, samplePos, gradient)";
    shadingValue = "";

    if (property.shadingMode_.isSelectedIdentifier("none"))
        shadingValue = "colorAmb;";
    else if (property.shadingMode_.isSelectedIdentifier("ambient"))
        shadingValue = "shadeAmbient(light, colorAmb);";
    else if (property.shadingMode_.isSelectedIdentifier("diffuse"))
        shadingValue = "shadeDiffuse(light, camera, volume, colorDiff, samplePos, gradient);";
    else if (property.shadingMode_.isSelectedIdentifier("specular"))
        shadingValue =
            "shadeSpecular(light, camera, volume, colorSpec, samplePos, gradient);";
    else if (property.shadingMode_.isSelectedIdentifier("phong"))
        shadingValue =
            "shadePhong(light, camera, volume, colorAmb, colorDiff, colorSpec, samplePos, gradient);";

    shader->getFragmentShaderObject()->addShaderDefine(shadingKey, shadingValue);
}

void setShaderUniforms(Shader* shader, const SimpleLightingProperty& property) {
    shader->setUniform("lightPosition_", property.lightPosition_.get());
    shader->setUniform("lightColorAmbient_", property.lightColorAmbient_.get());
    shader->setUniform("lightColorDiffuse_", property.lightColorDiffuse_.get());
    shader->setUniform("lightColorSpecular_", property.lightColorSpecular_.get());
    shader->setUniform("lightSpecularExponent_", property.lightSpecularExponent_.get());
}

void setShaderUniforms(Shader* shader, const SimpleLightingProperty& property, std::string name) {
    shader->setUniform(name + ".lightPosition_", property.lightPosition_.get());
    shader->setUniform(name + ".lightColorAmbient_", property.lightColorAmbient_.get());
    shader->setUniform(name + ".lightColorDiffuse_", property.lightColorDiffuse_.get());
    shader->setUniform(name + ".lightColorSpecular_", property.lightColorSpecular_.get());
    shader->setUniform(name + ".lightSpecularExponent_", property.lightSpecularExponent_.get());
}

void addShaderDefines(Shader* shader, const CameraProperty& property) {
}

void setShaderUniforms(Shader* shader, const CameraProperty& property) {
    shader->setUniform("viewMatrix_", property.viewMatrix());
    shader->setUniform("cameraPosition_", property.getLookFrom());
    shader->setUniform("zNear_", property.getNearPlaneDist());
    shader->setUniform("zFar_", property.getFarPlaneDist());
}

void setShaderUniforms(Shader* shader, const CameraProperty& property, std::string name) {
    shader->setUniform(name + ".viewMatrix_", property.viewMatrix());
    shader->setUniform(name + ".cameraPosition_", property.getLookFrom());
    shader->setUniform(name + ".zNear_", property.getNearPlaneDist());
    shader->setUniform(name + ".zFar_", property.getFarPlaneDist());
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
        "COMPUTE_GRADIENT_FOR_CHANNEL(voxel, samplePos, volume, volumeStruct, t, rayDirection, "
        "entryPoints, entryParameters, channel)";
    gradientComputationValue = "";

    if (property.gradientComputationMode_.isSelectedIdentifier("none"))
        gradientComputationValue = "voxel.xyz;";
    else if (property.gradientComputationMode_.isSelectedIdentifier("forward"))
        gradientComputationValue =
        "gradientForwardDiff(voxel, volume, volumeStruct, samplePos, channel);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("central"))
        gradientComputationValue =
        "gradientCentralDiff(voxel, volume, volumeStruct, samplePos, channel);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("central-higher"))
        gradientComputationValue =
        "gradientCentralDiffH(voxel, volume, volumeStruct, samplePos, channel);";
    else if (property.gradientComputationMode_.isSelectedIdentifier("backward"))
        gradientComputationValue =
        "gradientBackwardDiff(voxel, volume, volumeStruct, samplePos, channel);";
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
        "APPLY_COMPOSITING(result, color, samplePos, voxel, gradient, t, tDepth, tIncr)";
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

} // namspace utilgl

}  // namespace
