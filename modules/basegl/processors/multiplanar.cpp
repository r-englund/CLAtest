#include "multiplanar.h"
#include <inviwo/core/rendering/geometryrendererfactory.h>
#include <inviwo/core/datastructures/geometry/simplemeshcreator.h>
#include <modules/opengl/volume/volumegl.h>

namespace inviwo {

ProcessorClassName(MultiPlanar, "MultiPlanar");
ProcessorCategory(MultiPlanar, "Volume Rendering");
ProcessorCodeState(MultiPlanar, CODE_STATE_EXPERIMENTAL);

MultiPlanar::MultiPlanar()
    : ProcessorGL()
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
    ProcessorGL::initialize();
    shader_ = new Shader("standard.vert", "multiplanar.frag", false);

    shader_->build();
}

void MultiPlanar::deinitialize() {
    delete shader_;
    ProcessorGL::deinitialize();
}

Geometry* MultiPlanar::createSliceXYGeometry() {
    SimpleMesh* rectangle = new SimpleMesh();
    rectangle->initialize();
    //rectangle->setBasisAndOffset(volumePort_.getData()->getBasisAndOffset());
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

    rectangle->setIndicesInfo(TRIANGLES, STRIP);
    rectangle->addIndex(1);
    rectangle->addIndex(0);
    rectangle->addIndex(2);
    rectangle->addIndex(3);
    return rectangle;
}

Geometry* MultiPlanar::createSliceXZGeometry() {
    SimpleMesh* rectangle = new SimpleMesh();
    rectangle->initialize();
    //rectangle->setBasisAndOffset(volumePort_.getData()->getBasisAndOffset());
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

    rectangle->setIndicesInfo(TRIANGLES, STRIP);
    rectangle->addIndex(1);
    rectangle->addIndex(0);
    rectangle->addIndex(2);
    rectangle->addIndex(3);
    return rectangle;
}

Geometry* MultiPlanar::createSliceYZGeometry() {
    SimpleMesh* rectangle = new SimpleMesh();
    rectangle->initialize();
    //rectangle->setBasisAndOffset(volumePort_.getData()->getBasisAndOffset());
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

    rectangle->setIndicesInfo(TRIANGLES, STRIP);
    rectangle->addIndex(1);
    rectangle->addIndex(0);
    rectangle->addIndex(2);
    rectangle->addIndex(3);
    return rectangle;
}

void MultiPlanar::process() {

    /*
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.projectionMatrix()));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.viewMatrix()));
    */
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    activateAndClearTarget(outport_);

    shader_->activate();
    setGlobalShaderParameters(shader_);
    //mat4 modelMatrix = geom->getWorldTransform()*geom->getBasisAndOffset();
    mat4 modelMatrix = mat4(1.0f);//volumePort_.getData()->getBasisAndOffset();
    shader_->setUniform("modelViewProjectionMatrix_", camera_.projectionMatrix()*camera_.viewMatrix()*modelMatrix);

    TextureUnit sliceUnit;
    shader_->setUniform("sliceTex_", sliceUnit.getUnitNumber());
    if (showSliceXY_.get()) {
        bindColorTexture(sliceXYPort_, sliceUnit.getEnum());
        setTextureParameters(sliceXYPort_, shader_, "sliceTexParameters_");
        Geometry* sliceXYGeom = createSliceXYGeometry();
        GeometryRenderer* sliceXYRenderer = GeometryRendererFactory::getPtr()->create(sliceXYGeom);
        sliceXYRenderer->render();
    }
    if (showSliceXZ_.get()) {
        bindColorTexture(sliceXZPort_, sliceUnit.getEnum());
        setTextureParameters(sliceXZPort_, shader_, "sliceTexParameters_");
        Geometry* sliceXZGeom = createSliceXZGeometry();
        GeometryRenderer* sliceXZRenderer = GeometryRendererFactory::getPtr()->create(sliceXZGeom);
        sliceXZRenderer->render();
    }
    if (showSliceYZ_.get()) {
        bindColorTexture(sliceYZPort_, sliceUnit.getEnum());
        setTextureParameters(sliceYZPort_, shader_, "sliceTexParameters_");
        Geometry* sliceYZGeom = createSliceYZGeometry();
        GeometryRenderer* sliceYZRenderer = GeometryRendererFactory::getPtr()->create(sliceYZGeom);
        sliceYZRenderer->render();
    }
    shader_->deactivate();
    deactivateCurrentTarget();

    /*
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    */
}

} // namespace

