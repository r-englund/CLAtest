#include "volumeraycastergl.h"

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

   // , camera_("camera", "Camera", vec3(0.0f, 0.0f, 3.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))
{
    VolumeRaycasterGL("rc_simple.frag");
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

  //  , camera_("camera", "Camera", vec3(0.0f, 0.0f, -3.5f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))
{
    addProperty(samplingRate_);

    classificationMode_.addOption("none", "None");
    classificationMode_.addOption("transFunc", "Transfer function");
    classificationMode_.set("transFunc");
    addProperty(classificationMode_);

    gradientComputationMode_.addOption("none", "None");
    gradientComputationMode_.addOption("forward", "Forward differences");
    gradientComputationMode_.set("forward");
    addProperty(gradientComputationMode_);

    // light properties are only initialized here and need to be added by derived raycasters
    shadingMode_.addOption("ambient", "Ambient");
    shadingMode_.addOption("diffuse", "Diffuse");
    shadingMode_.addOption("specular", "Specular");
    shadingMode_.set("diffuse");
    lightColorAmbient_.setSemantics(PropertySemantics::Color);
    lightColorDiffuse_.setSemantics(PropertySemantics::Color);
    lightColorSpecular_.setSemantics(PropertySemantics::Color);

    compositingMode_.addOption("dvr", "Direct volume rendering");
    compositingMode_.addOption("iso", "Iso surface rendering");
    compositingMode_.addOption("mip", "Maximum intensity projection");
    compositingMode_.addOption("fhp", "First hit points");
    compositingMode_.addOption("fhn", "First hit normals");
    compositingMode_.set("dvr");
    addProperty(compositingMode_);

  //  camera_.setVisible(false);
 //   addProperty(camera_);
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
    // add some empty defines for Voreen compatibility
    raycastPrg_->getFragmentShaderObject()->addShaderDefine("RC_BEGIN_COMPOSITING");
    raycastPrg_->getFragmentShaderObject()->addShaderDefine("RC_END_COMPOSITING");

    // basic loop defines
    std::string beginLoop = "for (int sampleNum=0; sampleNum<1024; sampleNum++)";
    raycastPrg_->getFragmentShaderObject()->addShaderDefine("RC_BEGIN_LOOP", beginLoop);
    std::string endLoop = "";
    raycastPrg_->getFragmentShaderObject()->addShaderDefine("RC_END_LOOP(result)", endLoop);

    // classification defines
    std::string classificationKey = "RC_APPLY_CLASSIFICATION(transferFunc_, voxel)";
    std::string classificationValue = "";
    if (classificationMode_.get() == "none")
        classificationValue = "vec4(voxel.a);";
    if (classificationMode_.get() == "transFunc")
        classificationValue = "applyTF(transferFunc_, voxel);";
    raycastPrg_->getFragmentShaderObject()->addShaderDefine(classificationKey, classificationValue);

    // shading defines
    std::string shadingKey = "RC_APPLY_SHADING(gradient, samplePos, volumeStruct, ka, kd, ks)";
    std::string shadingValue = "";
    if (shadingMode_.get() == "ambient")
        shadingValue = "shadeAmbient(ka, ka);";
    if (shadingMode_.get() == "diffuse")
        shadingValue = "shadeDiffuse(kd, kd, gradient, vec3(0.5));";
    if (shadingMode_.get() == "specular")
        //FIXME: use variable values for shininess etc.
        shadingValue = "shadeSpecular(ks, ks, 0.5, gradient, vec3(0.5), vec3(0.0));";
    raycastPrg_->getFragmentShaderObject()->addShaderDefine(shadingKey, shadingValue);

    // compositing defines
    std::string compositingKey = "RC_APPLY_COMPOSITING(result, color, samplePos, gradient, t, tDepth)";
    std::string compositingValue = "";
    if (compositingMode_.get() == "dvr")
        compositingValue = "compositeDVR(result, color, t, tDepth);";
    else if (compositingMode_.get() == "mip")
        compositingValue = "compositeMIP(result, color, t, tDepth);";
    else if (compositingMode_.get() == "iso")
        compositingValue = "compositeISO(result, color, t, tDepth, isoValue_);";
    else if (compositingMode_.get() == "fhp")
        compositingValue = "compositeFHP(samplePos, result, t, tDepth);";
    else if (compositingMode_.get() == "fhn")
        compositingValue = "compositeFHN(gradient, result, t, tDepth);";
    raycastPrg_->getFragmentShaderObject()->addShaderDefine(compositingKey, compositingValue);

    raycastPrg_->build();
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

} // namespace
