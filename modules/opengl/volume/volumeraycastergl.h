#ifndef IVW_VOLUMERAYCASTERGL_H
#define IVW_VOLUMERAYCASTERGL_H

#include <modules/opengl/openglmoduledefine.h>
#include <modules/opengl/inviwoopengl.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/volumeport.h>
#include <modules/opengl/processorgl.h>
#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>

namespace inviwo {

class IVW_MODULE_OPENGL_API VolumeRaycasterGL : public ProcessorGL {

public:
    VolumeRaycasterGL();
    VolumeRaycasterGL(std::string programFileName);

    virtual void initialize();
    virtual void deinitialize();

    // overloaded method called when INVALID_RESOURCE property is set
    virtual void initializeResources();

    void bindTransferFunction(const TransferFunction& tf, GLenum tfTexUnit);
    void bindVolume(const VolumeInport& inport, GLenum volTexUnit);
    void setVolumeParameters(const VolumeInport& inport, Shader* shader, const std::string samplerID);
    void setGlobalShaderParameters(Shader* shader);

protected:
    Shader* raycastPrg_;
    std::string programFileName_;

    FloatProperty samplingRate_;
    FloatProperty isoValue_;

    OptionPropertyString classificationMode_;
    OptionPropertyString gradientComputationMode_;
    OptionPropertyString shadingMode_;
    OptionPropertyString compositingMode_;

    FloatVec3Property lightPosition_;
    FloatVec3Property lightColorAmbient_;
    FloatVec3Property lightColorDiffuse_;
    FloatVec3Property lightColorSpecular_;
    IntProperty lightSpecularExponent_;

    BoolProperty applyLightAttenuation_;
    FloatVec3Property lightAttenuation_;

    CameraProperty camera_;

    void addShadingProperties();
};

} // namespace

#endif // IVW_VOLUMERAYCASTERGL_H
