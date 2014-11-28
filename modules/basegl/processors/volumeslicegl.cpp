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
 * Contact: Erik Sundén
 *
 *********************************************************************************/

#include "volumeslicegl.h"
#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/image/layergl.h>
#include <modules/opengl/rendering/meshrenderer.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/glwrap/textureunit.h>
#include <modules/opengl/textureutils.h>
#include <inviwo/core/datastructures/geometry/mesh.h>
#include <inviwo/core/datastructures/buffer/bufferramprecision.h>
#include <inviwo/core/interaction/events/gestureevent.h>

namespace inviwo {

ProcessorClassIdentifier(VolumeSliceGL, "org.inviwo.VolumeSliceGL");
ProcessorDisplayName(VolumeSliceGL, "Volume Slice");
ProcessorTags(VolumeSliceGL, Tags::GL);
ProcessorCategory(VolumeSliceGL, "Volume Operation");
ProcessorCodeState(VolumeSliceGL, CODE_STATE_STABLE);

VolumeSliceGL::VolumeSliceGL()
    : Processor()
    , inport_("volume.inport")
    , outport_("image.outport", COLOR_ONLY)
    , trafoGroup_("trafoGroup", "Transformations")
    , pickGroup_("pickGroup", "Position Selection")
    , tfGroup_("tfGroup", "Transfer Function")
    , sliceAlongAxis_("sliceAxis", "Slice along axis")
    , sliceX_("sliceX", "X Volume Position", 128, 1, 256)
    , sliceY_("sliceY", "Y Volume Position", 128, 1, 256)
    , sliceZ_("sliceZ", "Z Volume Position", 128, 1, 256)
    , worldPosition_("worldPosition_", "World Position", vec3(0.0f), vec3(-10.0f), vec3(10.0f), vec3(0.01f), VALID)
    , planeNormal_("planeNormal", "Slice Plane Normal", vec3(1.f,0.f,0.f), vec3(-1.f,-1.f,-1.f), vec3(1.f, 1.f, 1.f), vec3(0.01f, 0.01f, 0.01f))
    , planeOffset_("planeOffset", "Slice Plane Offset", 0.5f, 0.0f, 1.0f, 0.01f)
    , rotationAroundAxis_("rotation", "Rotation (ccw)")
    , flipHorizontal_("flipHorizontal", "Flip Horizontal View", false)
    , flipVertical_("flipVertical", "Flip Vertical View", false)
    , volumeWrapping_("volumeWrapping", "Volume Texture Wrapping")
    , posPicking_("posPicking", "Enable Picking", false)
    , showIndicator_("showIndicator", "Show Position Indicator", true)
    , indicatorColor_("indicatorColor", "Indicator Color", vec4(1.0f, 0.8f, 0.1f, 0.8f), vec4(0.0f),
                      vec4(1.0f))
    , tfMappingEnabled_("tfMappingEnabled", "Enable Transfer Function", true)
    , transferFunction_("transferFunction", "Transfer function", TransferFunction(), &inport_)
    , tfAlphaOffset_("alphaOffset", "Alpha Offset", 0.0f, 0.0f, 1.0f, 0.01f)
    , handleInteractionEvents_("handleEvents", "Handle interaction events", true)
    , shader_(NULL)
    , indicatorShader_(NULL)
    , meshCrossHair_(NULL)
    , meshBox_(NULL)
    , meshDirty_(true)
    , volumeDimensions_(8u) {
    addPort(inport_);
    addPort(outport_);

    // helper construct to access axis slices via index
    slices_[0] = &sliceX_;
    slices_[1] = &sliceY_;
    slices_[2] = &sliceZ_;
    slices_[3] = NULL;

    inport_.onChange(this, &VolumeSliceGL::updateMaxSliceNumber);
    sliceAlongAxis_.addOption("x", "y-z plane (X axis)", CoordinateEnums::X);
    sliceAlongAxis_.addOption("y", "z-x plane (Y axis)", CoordinateEnums::Y);
    sliceAlongAxis_.addOption("z", "x-y plane (Z axis)", CoordinateEnums::Z);
    sliceAlongAxis_.addOption("p", "plane equation", 3);
    sliceAlongAxis_.set(CoordinateEnums::X);
    sliceAlongAxis_.setCurrentStateAsDefault();
    sliceAlongAxis_.onChange(this, &VolumeSliceGL::sliceAxisChanged);
    addProperty(sliceAlongAxis_);

    addProperty(sliceX_);
    addProperty(sliceY_);
    addProperty(sliceZ_);
    // Invalidate selected voxel cursor when current slice changes
    sliceX_.onChange(this, &VolumeSliceGL::invalidateMesh);
    sliceY_.onChange(this, &VolumeSliceGL::invalidateMesh);
    sliceZ_.onChange(this, &VolumeSliceGL::invalidateMesh);

    worldPosition_.onChange(this, &VolumeSliceGL::updatePos);
    addProperty(worldPosition_);

    addProperty(planeNormal_);
    planeNormal_.onChange(this, &VolumeSliceGL::planeSettingsChanged);
    addProperty(planeOffset_);

    // Transformations
    rotationAroundAxis_.addOption("0", "0 deg", 0.f);
    rotationAroundAxis_.addOption("90", "90 deg", glm::radians(90.f));
    rotationAroundAxis_.addOption("180", "180 deg", glm::radians(180.f));
    rotationAroundAxis_.addOption("270", "270 deg", glm::radians(270.f));
    rotationAroundAxis_.set(0.f);
    rotationAroundAxis_.setCurrentStateAsDefault();
    rotationAroundAxis_.onChange(this, &VolumeSliceGL::planeSettingsChanged);
    trafoGroup_.addProperty(rotationAroundAxis_);
    flipHorizontal_.onChange(this, &VolumeSliceGL::planeSettingsChanged);
    trafoGroup_.addProperty(flipHorizontal_);
    flipVertical_.onChange(this, &VolumeSliceGL::planeSettingsChanged);
    trafoGroup_.addProperty(flipVertical_);
    volumeWrapping_.addOption("0", "Use incoming wrapping", 0);
    volumeWrapping_.addOption("1", "Clamp", GL_CLAMP);
    volumeWrapping_.addOption("2", "Clamp to invisible border", GL_CLAMP_TO_BORDER);
    volumeWrapping_.addOption("3", "Clamp to edge", GL_CLAMP_TO_EDGE);
    volumeWrapping_.addOption("4", "Mirrored repeat", GL_MIRRORED_REPEAT);
    volumeWrapping_.addOption("5", "Repeat", GL_REPEAT);
    volumeWrapping_.set(0);
    volumeWrapping_.setCurrentStateAsDefault();
    trafoGroup_.addProperty(volumeWrapping_);
    addProperty(trafoGroup_);

    // Position Selection
    posPicking_.onChange(this, &VolumeSliceGL::posPickingChanged);
    pickGroup_.addProperty(posPicking_);
    showIndicator_.setReadOnly(posPicking_.get());
    pickGroup_.addProperty(showIndicator_);
    indicatorColor_.setSemantics(PropertySemantics::Color);
    indicatorColor_.onChange(this, &VolumeSliceGL::invalidateMesh);
    pickGroup_.addProperty(indicatorColor_);
    addProperty(pickGroup_);

    // Transfer Function
    tfMappingEnabled_.onChange(this, &VolumeSliceGL::tfMappingEnabledChanged);
    tfGroup_.addProperty(tfMappingEnabled_);
    // Make sure that opacity does not affect the mapped color.
    if (transferFunction_.get().getNumPoints() > 0) {
        transferFunction_.get().getPoint(0)->setA(1.f);
    }
    transferFunction_.setCurrentStateAsDefault();
    tfGroup_.addProperty(transferFunction_);
    tfGroup_.addProperty(tfAlphaOffset_);
    addProperty(tfGroup_);

    addProperty(handleInteractionEvents_);
    addInteractionHandler(new VolumeSliceGLInteractionHandler(this));

    updateReadOnlyStates();
}

VolumeSliceGL::~VolumeSliceGL() {
    const std::vector<InteractionHandler*>& interactionHandlers = getInteractionHandlers();
    for (size_t i = 0; i < interactionHandlers.size(); ++i) {
        InteractionHandler* handler = interactionHandlers[i];
        removeInteractionHandler(handler);
        delete handler;
    }
}

void VolumeSliceGL::initialize() {
    Processor::initialize();
    shader_ = new Shader("img_texturequad.vert", "volumeslice.frag", true);
    indicatorShader_ = new Shader("standard.frag", true);
    planeSettingsChanged();
    tfMappingEnabledChanged();
    updateMaxSliceNumber();
}

void VolumeSliceGL::deinitialize() {
    delete meshBox_;
    delete meshCrossHair_;
    delete shader_;
    delete indicatorShader_;
    Processor::deinitialize();
}

void VolumeSliceGL::process() {
    if (volumeDimensions_ != inport_.getData()->getDimension()) {
        volumeDimensions_ = inport_.getData()->getDimension();
        updateMaxSliceNumber();
    }

    TextureUnit transFuncUnit;
    const Layer* tfLayer = transferFunction_.get().getData();
    const LayerGL* transferFunctionGL = tfLayer->getRepresentation<LayerGL>();
    transferFunctionGL->bindTexture(transFuncUnit.getEnum());
    TextureUnit volUnit;
    const VolumeGL* volumeGL = inport_.getData()->getRepresentation<VolumeGL>();
    volUnit.activate();
    volumeGL->getTexture()->bind();
    GLint wrapS, wrapT, wrapR;
    if(volumeWrapping_.get() > 0){
        glGetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, &wrapS);
        glGetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, &wrapT);
        glGetTexParameteriv(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, &wrapR);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, volumeWrapping_.get());
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, volumeWrapping_.get());
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, volumeWrapping_.get());
        if(volumeWrapping_.get() == GL_CLAMP_TO_BORDER)
            glTexParameterfv(GL_TEXTURE_3D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(vec4(0.f)));
    }
    TextureUnit::setZeroUnit();

    utilgl::activateAndClearTarget(outport_);
    shader_->activate();
    vec2 dim = static_cast<vec2>(outport_.getDimension());
    shader_->setUniform("screenDim_", dim);
    shader_->setUniform("screenDimRCP_", vec2(1.0f, 1.0f) / dim);

    if (tfMappingEnabled_.get()) {
        shader_->setUniform("transferFunc_", transFuncUnit.getUnitNumber());
        shader_->setUniform("alphaOffset_", tfAlphaOffset_.get());
    }

    shader_->setUniform("volume_", volUnit.getUnitNumber());
    volumeGL->setVolumeUniforms(inport_.getData(), shader_, "volumeParameters_");
    shader_->setUniform("dimension_", vec2(1.0f / outport_.getData()->getDimension().x,
                                           1.0f / outport_.getData()->getDimension().y));
    shader_->setUniform("sliceNum_", getNormalizedSliceNumber());
    utilgl::singleDrawImagePlaneRect();
    shader_->deactivate();

    if (posPicking_.get() && showIndicator_.get()) {
        renderPositionIndicator();
    }

    utilgl::deactivateCurrentTarget();

    if(volumeWrapping_.get() > 0){
        volUnit.activate();
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, wrapR);
        TextureUnit::setZeroUnit();
    }
}

int VolumeSliceGL::getSliceNumber() const {
    return slices_[getAxisIndex(sliceAlongAxis_.get())]->get();
}

float VolumeSliceGL::getNormalizedSliceNumber() const {
    if(sliceAlongAxis_.get() == 3)
        return glm::abs(planeOffset_.get());

    IntProperty* prop = slices_[getAxisIndex(sliceAlongAxis_.get())];
    return (static_cast<float>(prop->get()) - 0.5f) /
           glm::max<float>(static_cast<float>(prop->getMaxValue()), 1.f);
}

void VolumeSliceGL::renderPositionIndicator() {
    if (meshDirty_) {
        vec4 pos(static_cast<float>(sliceX_.get()), static_cast<float>(sliceY_.get()),
            static_cast<float>(sliceZ_.get()), 1.0f);

        mat4 trans = inport_.getData()->getCoordinateTransformer().getIndexToWorldMatrix();

        worldPosition_.set(vec3(trans*pos));

        updateIndicatorMesh();
    }

    MeshRenderer renderer(meshCrossHair_);
    MeshRenderer rendererBox(meshBox_);

    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLineWidth(2.5f);
    indicatorShader_->activate();

    vec2 dim = static_cast<vec2>(outport_.getDimension());
    indicatorShader_->setUniform("screenDim_", dim);
    indicatorShader_->setUniform("screenDimRCP_", vec2(1.0f, 1.0f) / dim);

    glDepthFunc(GL_ALWAYS);
    renderer.render();
    rendererBox.render();
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
    glDisable(GL_LINE_SMOOTH);
    indicatorShader_->deactivate();

    glLineWidth(1.0f);
}

void VolumeSliceGL::updateIndicatorMesh() {
    vec2 pos = getScreenPosFromVolPos();

    delete meshCrossHair_;
    delete meshBox_;
    meshCrossHair_ = new Mesh;
    meshCrossHair_->setBasisAndOffset(mat4(1.0f));
    meshBox_ = new Mesh;
    meshBox_->setBasisAndOffset(mat4(1.0f));

    uvec2 canvasSize(outport_.getDimension());
    const vec2 indicatorSize = vec2(4.0f / canvasSize.x, 4.0f / canvasSize.y);
    vec4 color(indicatorColor_.get());

    // add two vertical and two horizontal lines with a gap around the selected position
    Position2dBuffer* posBuf = new Position2dBuffer;
    Position2dBufferRAM* vertices = posBuf->getEditableRepresentation<Position2dBufferRAM>();

    // horizontal
    vertices->add(vec2(-0.5f, pos.y) * 2.0f - 1.0f);
    vertices->add(vec2(pos.x - indicatorSize.x, pos.y) * 2.0f - 1.0f);
    vertices->add(vec2(pos.x + indicatorSize.x, pos.y) * 2.0f - 1.0f);
    vertices->add(vec2(1.5f, pos.y) * 2.0f - 1.0f);
    // vertical
    vertices->add(vec2(pos.x, -0.5f) * 2.0f - 1.0f);
    vertices->add(vec2(pos.x, pos.y - indicatorSize.y) * 2.0f - 1.0f);
    vertices->add(vec2(pos.x, pos.y + indicatorSize.y) * 2.0f - 1.0f);
    vertices->add(vec2(pos.x, 1.5f) * 2.0f - 1.0f);

    ColorBuffer* colorBuf = new ColorBuffer;
    ColorBufferRAM* colors = colorBuf->getEditableRepresentation<ColorBufferRAM>();

    // indices for cross hair lines
    IndexBuffer* indexBuf = new IndexBuffer();
    IndexBufferRAM* indices = indexBuf->getEditableRepresentation<IndexBufferRAM>();
    for (unsigned int i = 0; i < 8; ++i) {
        colors->add(color);
        indices->add(i);
    }
    // clear up existing attribute buffers
    // meshCrossHair_->deinitialize();
    meshCrossHair_->addAttribute(posBuf);
    meshCrossHair_->addAttribute(colorBuf);
    meshCrossHair_->addIndicies(Mesh::AttributesInfo(GeometryEnums::LINES, GeometryEnums::NONE),
                                indexBuf);

    // mesh for center box
    posBuf = new Position2dBuffer;
    vertices = posBuf->getEditableRepresentation<Position2dBufferRAM>();
    colorBuf = new ColorBuffer;
    colors = colorBuf->getEditableRepresentation<ColorBufferRAM>();
    indexBuf = new IndexBuffer();
    indices = indexBuf->getEditableRepresentation<IndexBufferRAM>();
    // box
    vertices->add(vec2(pos.x - indicatorSize.x, pos.y - indicatorSize.y) * 2.0f - 1.0f);
    vertices->add(vec2(pos.x + indicatorSize.x, pos.y - indicatorSize.y) * 2.0f - 1.0f);
    vertices->add(vec2(pos.x + indicatorSize.x, pos.y + indicatorSize.y) * 2.0f - 1.0f);
    vertices->add(vec2(pos.x - indicatorSize.x, pos.y + indicatorSize.y) * 2.0f - 1.0f);
    for (unsigned int i = 0; i < 4; ++i) {
        colors->add(color);
        indices->add(i);
    }
    // clear up existing attribute buffers
    // meshBox_->deinitialize();
    meshBox_->addAttribute(posBuf);
    meshBox_->addAttribute(colorBuf);
    meshBox_->addIndicies(Mesh::AttributesInfo(GeometryEnums::LINES, GeometryEnums::LOOP),
                          indexBuf);

    meshDirty_ = false;
}

void VolumeSliceGL::posPickingChanged() {
    updateReadOnlyStates();
    invalidateMesh();
}

void VolumeSliceGL::invalidateMesh() { meshDirty_ = true; }

void VolumeSliceGL::shiftSlice(int shift) {
    int newSlice = getSliceNumber() + shift;
    IntProperty* prop = slices_[getAxisIndex(sliceAlongAxis_.get())];
    if ((newSlice >= prop->getMinValue()) && (newSlice <= prop->getMaxValue())) {
        prop->set(newSlice);
    }
}

void VolumeSliceGL::updateReadOnlyStates() {
    disableInvalidation();
    // enable/disable slice sliders accordingly
    if (posPicking_.get()) {
        for (int i = 0; i < 3; ++i) {
            slices_[i]->setReadOnly(false);
        }
        planeNormal_.setReadOnly(false);
        planeOffset_.setReadOnly(false);
    } else {
        IntProperty* activeSlice = NULL;
        if(sliceAlongAxis_.get() < 3)
            activeSlice = slices_[getAxisIndex(sliceAlongAxis_.get())];
        for (int i = 0; i < 3; ++i) {
            slices_[i]->setReadOnly(slices_[i] != activeSlice);
        }
        planeNormal_.setReadOnly(activeSlice != NULL);
        planeOffset_.setReadOnly(activeSlice != NULL);
    }
    showIndicator_.setReadOnly(!posPicking_.get());
    enableInvalidation();
}

void VolumeSliceGL::setVolPosFromScreenPos(vec2 pos) {
    if (!posPicking_.get()) return;  // position mode not enabled

    if ((pos.x < 0.0f) || (pos.x > 1.0f) || (pos.y < 0.0f) || (pos.y > 1.0f)) {
        // invalid position
        return;
    }

    // Rotation
    float rotationAngle = rotationAroundAxis_.get();
    mat2 m(cos(rotationAngle), -sin(rotationAngle), sin(rotationAngle), cos(rotationAngle));

    if (flipHorizontal_.get()) pos.x = 1.0f - pos.x;
    if (flipVertical_.get()) pos.y = 1.0f - pos.y;
    pos = m * (pos - 0.5f) + 0.5f;

    ivec3 curPos(-1);
    int slice = getSliceNumber();
    switch (sliceAlongAxis_.get()) {
        case CoordinateEnums::X:  // y-z plane
            curPos.x = slice;
            curPos.y = static_cast<float>(pos.x * volumeDimensions_.y + 0.5f);
            curPos.z = static_cast<float>(pos.y * volumeDimensions_.z + 0.5f);
            break;
        case CoordinateEnums::Y:  // x-z plane
            curPos.x = static_cast<float>(pos.y * volumeDimensions_.x + 0.5f);
            curPos.y = slice;
            curPos.z = static_cast<float>(pos.x * volumeDimensions_.z + 0.5f);
            break;
        case CoordinateEnums::Z:  // x-y plane
            curPos.x = static_cast<float>(pos.x * volumeDimensions_.x + 0.5f);
            curPos.y = static_cast<float>(pos.y * volumeDimensions_.y + 0.5f);
            curPos.z = slice;
            break;
    }
    disableInvalidation();
    sliceX_.set(curPos.x);
    sliceY_.set(curPos.y);
    sliceZ_.set(curPos.z);
    enableInvalidation();
}

vec2 VolumeSliceGL::getScreenPosFromVolPos() {
    ivec3 pos(sliceX_.get(), sliceY_.get(), sliceZ_.get());
    vec2 posF(-1.0f);
    switch (sliceAlongAxis_.get()) {
        case CoordinateEnums::X:  // y-z plane
            posF.x = (pos.y + 0.5f) / volumeDimensions_.y;
            posF.y = (pos.z + 0.5f) / volumeDimensions_.z;
            break;
        case CoordinateEnums::Y:  // x-z plane
            posF.x = (pos.z + 0.5f) / volumeDimensions_.z;
            posF.y = (pos.x + 0.5f) / volumeDimensions_.x;
            break;
        case CoordinateEnums::Z:  // x-y plane
            posF.x = (pos.x + 0.5f) / volumeDimensions_.x;
            posF.y = (pos.y + 0.5f) / volumeDimensions_.y;
            break;
    }

    float rotationAngle = -rotationAroundAxis_.get();
    mat2 m(cos(rotationAngle), -sin(rotationAngle), sin(rotationAngle), cos(rotationAngle));
    posF = m * (posF - 0.5f) + 0.5f;
    if (flipHorizontal_.get()) posF.x = 1.0f - posF.x;
    if (flipVertical_.get()) posF.y = 1.0f - posF.y;
    return posF;
}

void VolumeSliceGL::sliceAxisChanged() {
    updateReadOnlyStates();
    planeSettingsChanged();
}

void VolumeSliceGL::planeSettingsChanged() {
    std::string fH = (flipHorizontal_.get() ? "1.0-" : "");
    std::string fV = (flipVertical_.get() ? "1.0-" : "");
    // map (x, y, z) to volume texture space coordinates
    // Input:
    // z is the direction in which we slice
    // x is the horizontal direction and y is the vertical direction of the output image

    if (shader_) {
        switch (sliceAlongAxis_.get()) {
            case CoordinateEnums::X:
                shader_->getFragmentShaderObject()->addShaderDefine("coordPlanePermute(x,y,z)",
                                                                    "z," + fH + "x," + fV + "y");
                break;
            case CoordinateEnums::Y:
                shader_->getFragmentShaderObject()->addShaderDefine("coordPlanePermute(x,y,z)",
                                                                    fV + "y," + "z," + fH + "x");
                break;
            default:;
                shader_->getFragmentShaderObject()->addShaderDefine("coordPlanePermute(x,y,z)",
                                                                    fH + "x," + fV + "y,z");
                break;
        }
        shader_->getFragmentShaderObject()->addShaderDefine("COORD_PLANE_PERMUTE");

        shader_->getFragmentShaderObject()->build();
        shader_->link();
        // Rotation
        float rotationAngle = rotationAroundAxis_.get();
        vec3 sliceAxis = vec3(0.f, 0.f, -1.f);
        vec3 rotationOffset = vec3(0.f);

        mat4 rotationMat;

        //Rotation to plane equation
        if(sliceAlongAxis_.get() == 3){
            vec3 axisNormal = glm::normalize(sliceAxis);
            vec3 planeNormal = glm::normalize(planeNormal_.get());

            float cosTheta = glm::dot(axisNormal, planeNormal);

            quat rotationQuat;
            vec3 rotationAxis;
            if (cosTheta < -1 + 0.001f){
                rotationAxis = glm::cross(vec3(0.0f, 0.0f, 1.0f), axisNormal);
                if (glm::length2(rotationAxis) < 0.01 )
                    rotationAxis = glm::cross(vec3(1.0f, 0.0f, 0.0f), axisNormal);

                rotationAxis = glm::normalize(rotationAxis);
                rotationQuat = glm::angleAxis(180.0f, rotationAxis);
            }

            rotationAxis = glm::cross(axisNormal, planeNormal);

            float s = sqrt( (1+cosTheta)*2 );
            float invs = 1 / s;

            rotationQuat = quat(
                s * 0.5f, 
                rotationAxis.x * invs,
                rotationAxis.y * invs,
                rotationAxis.z * invs
                );

            //rotationMat = glm::rotate(rotationAngle, sliceAxis);
            rotationMat *= glm::toMat4(rotationQuat);
            
            // Offset during rotation to rotate around the center point
            rotationOffset = vec3(-0.5f) + 0.5f * rotationAxis;
            rotationMat = glm::translate(rotationMat, rotationOffset);
        }
        else{
            switch (sliceAlongAxis_.get()) {
                case CoordinateEnums::X:
                    sliceAxis = vec3(-1.f, 0.f, 0.f);
                    break;
                case CoordinateEnums::Y:
                    sliceAxis = vec3(0.f, -1.f, 0.f);
                    break;
                case CoordinateEnums::Z:
                    sliceAxis = vec3(0.f, 0.f, -1.f);
                    break;
                default:
                    break;
            }
            rotationMat = glm::rotate(rotationAngle, sliceAxis);

            // Offset during rotation to rotate around the center point
            rotationOffset = vec3(-0.5f) + 0.5f * sliceAxis;
            rotationMat = glm::translate(rotationMat, rotationOffset);
        }

        shader_->activate();
        shader_->setUniform("sliceAxisRotationMatrix_", rotationMat);
        // Translate back after rotation
        shader_->setUniform("rotationOffset_", -rotationOffset);
        shader_->deactivate();
    }

    invalidateMesh();
}

void VolumeSliceGL::tfMappingEnabledChanged() {
    if (shader_) {
        if (tfMappingEnabled_.get()) {
            shader_->getFragmentShaderObject()->addShaderDefine("TF_MAPPING_ENABLED");
            transferFunction_.setVisible(true);
            tfAlphaOffset_.setVisible(true);
        } else {
            shader_->getFragmentShaderObject()->removeShaderDefine("TF_MAPPING_ENABLED");
            transferFunction_.setVisible(false);
            tfAlphaOffset_.setVisible(false);
        }
        planeSettingsChanged();
    }
}

void VolumeSliceGL::updateMaxSliceNumber() {
    if (!inport_.hasData()) {
        return;
    }
    disableInvalidation();
    uvec3 dims = inport_.getData()->getDimension();
    if (dims.x != sliceX_.getMaxValue()) {
        sliceX_.setMaxValue(static_cast<int>(dims.x));
        sliceX_.set(static_cast<int>(dims.x) / 2);
    }
    if (dims.y != sliceY_.getMaxValue()) {
        sliceY_.setMaxValue(static_cast<int>(dims.y));
        sliceY_.set(static_cast<int>(dims.y) / 2);
    }
    if (dims.z != sliceZ_.getMaxValue()) {
        sliceZ_.setMaxValue(static_cast<int>(dims.z));
        sliceZ_.set(static_cast<int>(dims.z) / 2);
    }
    enableInvalidation();
}

void VolumeSliceGL::updatePos() {
    if (meshDirty_) return;

    mat4 trans = inport_.getData()->getCoordinateTransformer().getWorldToIndexMatrix();
    vec3 pos  = vec3(trans * vec4(worldPosition_.get().x, worldPosition_.get().y, worldPosition_.get().z, 1.0f));
    ivec3 ipos = static_cast<ivec3>(pos);

    ivec3 dim = static_cast<ivec3>(inport_.getData()->getDimension());

    sliceX_.set(std::min(std::max(0, ipos.x), dim.x-1));
    sliceY_.set(std::min(std::max(0, ipos.y), dim.y-1));
    sliceZ_.set(std::min(std::max(0, ipos.z), dim.z-1));
}

VolumeSliceGL::VolumeSliceGLInteractionHandler::VolumeSliceGLInteractionHandler(VolumeSliceGL* vs)
    : InteractionHandler()
    , wheelEvent_(MouseEvent::MOUSE_BUTTON_NONE, InteractionEvent::MODIFIER_NONE)
    , mousePressEvent_(MouseEvent::MOUSE_BUTTON_LEFT, InteractionEvent::MODIFIER_NONE)
    , upEvent_('W', InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS)
    , downEvent_('S', InteractionEvent::MODIFIER_NONE, KeyboardEvent::KEY_STATE_PRESS)
    , slicer_(vs) {}

void VolumeSliceGL::VolumeSliceGLInteractionHandler::invokeEvent(Event* event) {
    if (!slicer_->handleInteractionEvents_.get() || event->hasBeenUsed()) return;

    GestureEvent* gestureEvent = dynamic_cast<GestureEvent*>(event);
    if (gestureEvent) {
        if (gestureEvent->type() == GestureEvent::PAN) {
            if (gestureEvent->deltaPos().y < 0)
                slicer_->shiftSlice(1);
            else if (gestureEvent->deltaPos().y > 0)
                slicer_->shiftSlice(-1);
        }
        return;
    }

    MouseEvent* mouseEvent = dynamic_cast<MouseEvent*>(event);
    if (mouseEvent) {
        int state = mouseEvent->state();
        int modifier = mouseEvent->modifiers();

        if (modifier == wheelEvent_.modifiers() && state == MouseEvent::MOUSE_STATE_WHEEL) {
            int steps = mouseEvent->wheelSteps();
            slicer_->shiftSlice(steps);
        } else if ((modifier == mousePressEvent_.modifiers()) &&
                   (mouseEvent->button() == MouseEvent::MOUSE_BUTTON_LEFT) &&
                   ((state == MouseEvent::MOUSE_STATE_MOVE) ||
                    (state == MouseEvent::MOUSE_STATE_PRESS))) {
            vec2 mousePos(mouseEvent->posNormalized());
            slicer_->setVolPosFromScreenPos(vec2(mousePos.x, 1.0f - mousePos.y));
        }
        return;
    }

    KeyboardEvent* keyEvent = dynamic_cast<KeyboardEvent*>(event);
    if (keyEvent) {
        int button = keyEvent->button();
        int state = keyEvent->state();
        int modifier = keyEvent->modifiers();

        if (button == upEvent_.button() && modifier == upEvent_.modifiers() &&
            state == KeyboardEvent::KEY_STATE_PRESS)
            slicer_->shiftSlice(1);
        else if (button == downEvent_.button() && modifier == downEvent_.modifiers() &&
                 state == KeyboardEvent::KEY_STATE_PRESS)
            slicer_->shiftSlice(-1);

        return;
    }
}

}  // inviwo namespace
