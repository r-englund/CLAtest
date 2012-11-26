#include "entryexitpoints.h"

namespace inviwo {

EntryExitPoints::EntryExitPoints()
    : ProcessorGL(),
    volumePort_(Port::INPORT, "volume"),
    entryPort_(Port::OUTPORT, "entry-points"),
    exitPort_(Port::OUTPORT, "exit-points"),
    camera_("camera", "Camera", vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 1.0f, 0.0f))
{
    addPort(volumePort_);
    addPort(entryPort_);
    addPort(exitPort_);

    addProperty(camera_);
}

EntryExitPoints::~EntryExitPoints() {}

Processor* EntryExitPoints::create() const {
    return new EntryExitPoints();
}

void EntryExitPoints::initialize() {
    ProcessorGL::initialize();
    shader_ = new Shader("eepgeneration.frag");

    addInteractionHandler(new Trackball(&camera_));

    // compute bounding box dimensions
    // TODO: change upon volume change
    //ivec3 volDim = ivec3(256);
    //unsigned int maxDim = max(volDim.x, max(volDim.y, volDim.z));
    //vec3 ratio = vec3(maxDim)/volDim;

    vec3 llf = vec3(-1.0, -1.0, -1.0);
    vec3 urb = vec3( 1.0,  1.0,  1.0);
    listID_ = glGenLists(1);
    glNewList(listID_, GL_COMPILE);
    renderBoundingBox(llf, urb);
    glEndList();
}

void EntryExitPoints::deinitialize() {
    delete shader_;
    ProcessorGL::deinitialize();
}

void EntryExitPoints::renderBoundingBox(vec3 llf, vec3 urb) {
    glBegin(GL_QUADS);
	    // back face
	    glTexCoord3f(0.0f, 1.0f, 1.0f); glVertex3f(llf[0], urb[1], urb[2]);
	    glTexCoord3f(0.0f, 0.0f, 1.0f); glVertex3f(llf[0], llf[1], urb[2]);
	    glTexCoord3f(1.0f, 0.0f, 1.0f); glVertex3f(urb[0], llf[1], urb[2]);
	    glTexCoord3f(1.0f, 1.0f, 1.0f); glVertex3f(urb[0], urb[1], urb[2]);
	    // front face
	    glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(urb[0], llf[1], llf[2]);
	    glTexCoord3f(0.0f, 0.0f, 0.0f); glVertex3f(llf[0], llf[1], llf[2]);
	    glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(llf[0], urb[1], llf[2]);
	    glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(urb[0], urb[1], llf[2]);
	    // top face
	    glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(urb[0], urb[1], llf[2]);
	    glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(llf[0], urb[1], llf[2]);
	    glTexCoord3f(0.0f, 1.0f, 1.0f); glVertex3f(llf[0], urb[1], urb[2]);
	    glTexCoord3f(1.0f, 1.0f, 1.0f); glVertex3f(urb[0], urb[1], urb[2]);
	    // bottom face
	    glTexCoord3f(0.0f, 0.0f, 1.0f); glVertex3f(llf[0], llf[1], urb[2]);
	    glTexCoord3f(0.0f, 0.0f, 0.0f); glVertex3f(llf[0], llf[1], llf[2]);
	    glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(urb[0], llf[1], llf[2]);
	    glTexCoord3f(1.0f, 0.0f, 1.0f); glVertex3f(urb[0], llf[1], urb[2]);
	    // right face
	    glTexCoord3f(1.0f, 0.0f, 1.0f); glVertex3f(urb[0], llf[1], urb[2]);
	    glTexCoord3f(1.0f, 0.0f, 0.0f); glVertex3f(urb[0], llf[1], llf[2]);
	    glTexCoord3f(1.0f, 1.0f, 0.0f); glVertex3f(urb[0], urb[1], llf[2]);
	    glTexCoord3f(1.0f, 1.0f, 1.0f); glVertex3f(urb[0], urb[1], urb[2]);
	    // left face
	    glTexCoord3f(0.0f, 1.0f, 0.0f); glVertex3f(llf[0], urb[1], llf[2]);
	    glTexCoord3f(0.0f, 0.0f, 0.0f); glVertex3f(llf[0], llf[1], llf[2]);
	    glTexCoord3f(0.0f, 0.0f, 1.0f); glVertex3f(llf[0], llf[1], urb[2]);
	    glTexCoord3f(0.0f, 1.0f, 1.0f); glVertex3f(llf[0], urb[1], urb[2]);
    glEnd();
}

void EntryExitPoints::process() {
    
    glEnable(GL_CULL_FACE);    

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(camera_.projectionMatrix()));

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glLoadMatrixf(glm::value_ptr(camera_.viewMatrix()));

    // generate entry points
    activateTarget(entryPort_);
    shader_->activate();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_BACK);
    glCallList(listID_);
    shader_->deactivate();
    deactivateCurrentTarget();

    // generate exit points
    activateTarget(exitPort_);
    shader_->activate();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCullFace(GL_FRONT);
    glCallList(listID_);
    shader_->deactivate();
    deactivateCurrentTarget();

    glPopMatrix();    
    
    glDisable(GL_CULL_FACE);
}

} // namespace
