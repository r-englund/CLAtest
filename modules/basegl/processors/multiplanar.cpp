#include "multiplanar.h"
#include <inviwo/core/rendering/geometryrendererfactory.h>
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>
#include <modules/opengl/volume/volumegl.h>
#include <modules/opengl/glwrap/shader.h>
#include <modules/opengl/textureutils.h>

namespace inviwo {

ProcessorClassIdentifier(MultiPlanar, "org.inviwo.MultiPlanar");
ProcessorDisplayName(MultiPlanar,  "Multi Planar");
ProcessorTags(MultiPlanar, Tags::GL);
ProcessorCategory(MultiPlanar, "Volume Rendering");
ProcessorCodeState(MultiPlanar, CODE_STATE_EXPERIMENTAL);

MultiPlanar::MultiPlanar()
    : Processor()
    , sliceXYPort_("inport.sliceXY")
    , sliceXZPort_("inport.sliceXZ")
    , sliceYZPort_("inport.sliceYZ")
    , outport_("image.outport")
    , camera_("camera", "Camera", vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f))
    , showSliceXY_("showSliceXY", "Show slice XY", true)
    , showSliceXZ_("showSliceXZ", "Show slice XZ", true)
    , showSliceYZ_("showSliceYZ", "Show slice YZ", true)
    , sliceXYPos_("sliceXY", "Slice XY", 1, 1, 255)
    , sliceXZPos_("sliceXZ", "Slice XZ", 1, 1, 255)
    , sliceYZPos_("sliceYZ", "Slice YZ", 1, 1, 255)
    , shader_(NULL) {
    addPort(sliceXYPort_);
    addPort(sliceXZPort_);
    addPort(sliceYZPort_);
    addPort(outport_);
    addProperty(camera_);
    addProperty(showSliceXY_);
    addProperty(showSliceXZ_);
    addProperty(showSliceYZ_);
    addProperty(sliceXYPos_);
    addProperty(sliceXZPos_);
    addProperty(sliceYZPos_);
}

void MultiPlanar::initialize() {
    Processor::initialize();
    shader_ = new Shader("standard.vert", "multiplanar.frag", false);
    shader_->build();
}

void MultiPlanar::deinitialize() {
    delete shader_;
    Processor::deinitialize();
}

Geometry* MultiPlanar::createSliceXYGeometry() {
    SimpleMesh* rectangle = new SimpleMesh();
    rectangle->initialize();
    rectangle->setBasisAndOffset(mat4(1.0f));

    vec3 posLl(0.0, 0.0, (float)sliceXYPos_.get()/(float)sliceXYPos_.getMaxValue());
    vec3 posUr(1.0, 1.0, (float)sliceXYPos_.get()/(float)sliceXYPos_.getMaxValue());

    vec3 vertexCoord = posLl;
    rectangle->addVertex(vertexCoord, vec3(0.0, 0.0, 0.0), vec4(vertexCoord, 1.0));
    vertexCoord.x = posUr.x;
    rectangle->addVertex(vertexCoord, vec3(1.0, 0.0, 0.0), vec4(vertexCoord, 1.0));
    vertexCoord.y = posUr.y;
    rectangle->addVertex(vertexCoord, vec3(1.0, 1.0, 0.0), vec4(vertexCoord, 1.0));
    vertexCoord.x = posLl.x;
    rectangle->addVertex(vertexCoord, vec3(0.0, 1.0, 0.0), vec4(vertexCoord, 1.0));

    rectangle->setIndicesInfo(GeometryEnums::TRIANGLES, GeometryEnums::STRIP);
    rectangle->addIndex(1);
    rectangle->addIndex(0);
    rectangle->addIndex(2);
    rectangle->addIndex(3);
    return rectangle;
}

Geometry* MultiPlanar::createSliceXZGeometry() {
    SimpleMesh* rectangle = new SimpleMesh();
    rectangle->initialize();
    rectangle->setBasisAndOffset(mat4(1.0f));

    vec3 posLl(0.0, (float)sliceXZPos_.get()/(float)sliceXZPos_.getMaxValue(), 0.0);
    vec3 posUr(1.0, (float)sliceXZPos_.get()/(float)sliceXZPos_.getMaxValue(), 1.0);

    vec3 vertexCoord = posLl;
    rectangle->addVertex(vertexCoord, vec3(0.0, 0.0, 0.0), vec4(vertexCoord, 1.0));
    vertexCoord.x = posUr.x;
    rectangle->addVertex(vertexCoord, vec3(1.0, 0.0, 0.0), vec4(vertexCoord, 1.0));
    vertexCoord.z = posUr.z;
    rectangle->addVertex(vertexCoord, vec3(1.0, 1.0, 0.0), vec4(vertexCoord, 1.0));
    vertexCoord.x = posLl.x;
    rectangle->addVertex(vertexCoord, vec3(0.0, 1.0, 0.0), vec4(vertexCoord, 1.0));

    rectangle->setIndicesInfo(GeometryEnums::TRIANGLES, GeometryEnums::STRIP);
    rectangle->addIndex(1);
    rectangle->addIndex(0);
    rectangle->addIndex(2);
    rectangle->addIndex(3);
    return rectangle;
}

Geometry* MultiPlanar::createSliceYZGeometry() {
    SimpleMesh* rectangle = new SimpleMesh();
    rectangle->initialize();
    rectangle->setBasisAndOffset(mat4(1.0f));

    vec3 posLl((float)sliceYZPos_.get()/(float)sliceYZPos_.getMaxValue(), 0.0, 0.0);
    vec3 posUr((float)sliceYZPos_.get()/(float)sliceYZPos_.getMaxValue(), 1.0, 1.0);

    vec3 vertexCoord = posLl;
    rectangle->addVertex(vertexCoord, vec3(0.0, 0.0, 0.0), vec4(vertexCoord, 1.0));
    vertexCoord.y = posUr.y;
    rectangle->addVertex(vertexCoord, vec3(1.0, 0.0, 0.0), vec4(vertexCoord, 1.0));
    vertexCoord.z = posUr.z;
    rectangle->addVertex(vertexCoord, vec3(1.0, 1.0, 0.0), vec4(vertexCoord, 1.0));
    vertexCoord.y = posLl.y;
    rectangle->addVertex(vertexCoord, vec3(0.0, 1.0, 0.0), vec4(vertexCoord, 1.0));

    rectangle->setIndicesInfo(GeometryEnums::TRIANGLES, GeometryEnums::STRIP);
    rectangle->addIndex(1);
    rectangle->addIndex(0);
    rectangle->addIndex(2);
    rectangle->addIndex(3);
    return rectangle;
}

void MultiPlanar::process() {
    glDepthFunc(GL_LESS);

    utilgl::activateAndClearTarget(outport_);

    shader_->activate();
    
    vec2 dim = static_cast<vec2>(outport_.getDimension());
    shader_->setUniform("screenDim_", dim);
    shader_->setUniform("screenDimRCP_", vec2(1.0f, 1.0f) / dim);

    mat4 modelMatrix = mat4(1.0f);
    shader_->setUniform("modelViewProjectionMatrix_", camera_.projectionMatrix()*camera_.viewMatrix()*modelMatrix);

    TextureUnit sliceUnit;
    shader_->setUniform("sliceTex_", sliceUnit.getUnitNumber());
    if (showSliceXY_.get()) {
        utilgl::bindColorTexture(sliceXYPort_, sliceUnit.getEnum());
        utilgl::setShaderUniforms(shader_, sliceXYPort_, "sliceTexParameters_");
        Geometry* sliceXYGeom = createSliceXYGeometry();
        GeometryRenderer* sliceXYRenderer = GeometryRendererFactory::getPtr()->create(sliceXYGeom);
        sliceXYRenderer->render();
        delete sliceXYGeom;
        delete sliceXYRenderer;
    }
    if (showSliceXZ_.get()) {
        utilgl::bindColorTexture(sliceXZPort_, sliceUnit.getEnum());
        utilgl::setShaderUniforms(shader_, sliceXZPort_, "sliceTexParameters_");
        Geometry* sliceXZGeom = createSliceXZGeometry();
        GeometryRenderer* sliceXZRenderer = GeometryRendererFactory::getPtr()->create(sliceXZGeom);
        sliceXZRenderer->render();
        delete sliceXZGeom;
        delete sliceXZRenderer;
    }
    if (showSliceYZ_.get()) {
        utilgl::bindColorTexture(sliceYZPort_, sliceUnit.getEnum());
        utilgl::setShaderUniforms(shader_, sliceYZPort_, "sliceTexParameters_");
        Geometry* sliceYZGeom = createSliceYZGeometry();
        GeometryRenderer* sliceYZRenderer = GeometryRendererFactory::getPtr()->create(sliceYZGeom);
        sliceYZRenderer->render();
        delete sliceYZGeom;
        delete sliceYZRenderer;
    }
    shader_->deactivate();
    utilgl::deactivateCurrentTarget();
}

} // namespace

