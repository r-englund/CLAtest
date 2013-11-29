#include "volumeslicegl.h"
#include <modules/opengl/volume/volumegl.h>

namespace inviwo {

ProcessorClassName(VolumeSliceGL, "VolumeSliceGL"); 
ProcessorCategory(VolumeSliceGL, "Volume Operation");
ProcessorCodeState(VolumeSliceGL, CODE_STATE_STABLE);

VolumeSliceGL::VolumeSliceGL()
    : ProcessorGL(),
    inport_("volume.inport"),
    outport_("image.outport", COLOR_ONLY),
    coordinatePlane_("coordinatePlane", "Coordinate Plane"),
    sliceNumber_("sliceNumber", "Slice Number", 1, 1, 256),
    transferFunction_("transferFunction", "Transfer function", TransferFunction())
{
    addPort(inport_);
    addPort(outport_);

    coordinatePlane_.addOption("xy", "XY Plane", XY);
    coordinatePlane_.addOption("xz", "XZ Plane", XZ);
    coordinatePlane_.addOption("yz", "YZ Plane", YZ);
    coordinatePlane_.set(XY);
    coordinatePlane_.onChange(this, &VolumeSliceGL::coordinatePlaneChanged);
    addProperty(coordinatePlane_);

    addProperty(sliceNumber_);

    addProperty(transferFunction_);

    volumeDimensions_ = uvec3(256);
}

VolumeSliceGL::~VolumeSliceGL() {}

void VolumeSliceGL::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("vol_slice.frag", false);
    coordinatePlaneChanged();
    volumeDimensionChanged();
}

void VolumeSliceGL::deinitialize() {
    delete shader_;
    ProcessorGL::deinitialize();
}

void VolumeSliceGL::process(){
    if(volumeDimensions_ != inport_.getData()->getDimension()){
        volumeDimensions_ = inport_.getData()->getDimension();
        volumeDimensionChanged();
    }

    TextureUnit volUnit;
    const VolumeGL* volumeGL = inport_.getData()->getRepresentation<VolumeGL>();
    volumeGL->bindTexture(volUnit.getEnum());

    TextureUnit transFuncUnit;
    const Image* tfImage = transferFunction_.get().getData();
    const ImageGL* transferFunctionGL = tfImage->getRepresentation<ImageGL>();
    transferFunctionGL->bindColorTexture(transFuncUnit.getEnum());

    float sliceNum = static_cast<float>(sliceNumber_.get())/glm::max<float>(static_cast<float>(sliceNumber_.getMaxValue()-1), 1.f);

    activateAndClearTarget(outport_);

    shader_->activate();
    setGlobalShaderParameters(shader_);

    shader_->setUniform("volume_", volUnit.getUnitNumber());
    vec3 dimensions = vec3(volumeGL->getDimensions());
    shader_->setUniform("volumeParameters_.dimensions_", dimensions);
    shader_->setUniform("volumeParameters_.dimensionsRCP_", vec3(1.0f)/dimensions);

    shader_->setUniform("dimension_", vec2(1.f / outport_.getDimensions()[0], 1.f / outport_.getDimensions()[1]));
    shader_->setUniform("transferFunc_", transFuncUnit.getUnitNumber());
    shader_->setUniform("sliceNum_", sliceNum);

    renderImagePlaneRect();

    shader_->deactivate();

    deactivateCurrentTarget();
}

void VolumeSliceGL::coordinatePlaneChanged(){
    switch(coordinatePlane_.get())
    {
    case XY:
        shader_->getFragmentShaderObject()->addShaderDefine("coordPlanePermute(x,y,z)", "x,y,z");
        break;
    case XZ:
        shader_->getFragmentShaderObject()->addShaderDefine("coordPlanePermute(x,y,z)", "x,z,y");
        break;
    case YZ:
        shader_->getFragmentShaderObject()->addShaderDefine("coordPlanePermute(x,y,z)", "y,z,x");
        break;
    }
    shader_->rebuild();
}

void VolumeSliceGL::volumeDimensionChanged(){
    switch(coordinatePlane_.get())
    {
    case XY:
        sliceNumber_.setMaxValue(static_cast<int>(volumeDimensions_.z));
        break;
    case XZ:
        sliceNumber_.setMaxValue(static_cast<int>(volumeDimensions_.y));
        break;
    case YZ:
        sliceNumber_.setMaxValue(static_cast<int>(volumeDimensions_.x));
        break;
    }
}

} // inviwo namespace
