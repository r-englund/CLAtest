#include "entryexitpoints.h"
#include <modules/opengl/geometry/geometrygl.h>
#include <modules/opengl/geometry/meshglrenderer.h>
#include <inviwo/core/interaction/trackball.h>

namespace inviwo {

ProcessorClassName(EntryExitPoints, "EntryExitPoints"); 
ProcessorCategory(EntryExitPoints, "Geometry Rendering");
ProcessorCodeState(EntryExitPoints, CODE_STATE_STABLE);

EntryExitPoints::EntryExitPoints()
    : ProcessorGL(),
    geometryPort_("geometry"),
    entryPort_("entry-points"),
    exitPort_("exit-points"),
    camera_("camera", "Camera", vec3(0.0f, 0.0f, -2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)),
	capNearClipping_("capNearClipping", "Cap near plane clipping", true)
{
    addPort(geometryPort_);
    addPort(entryPort_, "ImagePortGroup1");
    addPort(exitPort_, "ImagePortGroup1");

    addProperty(camera_);
	addProperty(capNearClipping_);
	addInteractionHandler(new Trackball(&camera_));
    entryPort_.addResizeEventListener(&camera_);
}

EntryExitPoints::~EntryExitPoints() {}

void EntryExitPoints::initialize() {
    ProcessorGL::initialize();
	capNearClippingPrg_ = new Shader("capnearclipping.frag");
}

void EntryExitPoints::deinitialize() {
    ProcessorGL::deinitialize();
}

void EntryExitPoints::process() {
    const Geometry* geom = geometryPort_.getData();
    const Mesh* mesh = dynamic_cast<const Mesh*>(geom);
    if (!mesh) return;
    MeshGLRenderer renderer(mesh);

    glEnable(GL_CULL_FACE);    

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.projectionMatrix()));
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadMatrixf(glm::value_ptr(camera_.viewMatrix()));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);

	// generate exit points
	activateAndClearTarget(exitPort_);
	glCullFace(GL_FRONT);
    renderer.render();
	deactivateCurrentTarget();

	// generate entry points
    Image* tmpEntryPoints;
    ImageGL* tmpEntryPointsGL;
	if (capNearClipping_.get()) {
		tmpEntryPoints = new Image(exitPort_.getDimensions());
		tmpEntryPointsGL = tmpEntryPoints->getEditableRepresentation<ImageGL>();
		tmpEntryPointsGL->activateBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	} else {
		activateAndClearTarget(entryPort_);
	}
	glCullFace(GL_BACK);
	renderer.render();
	deactivateCurrentTarget();

	if (capNearClipping_.get()) {
		// render an image plane aligned quad to cap the proxy geometry
		activateAndClearTarget(entryPort_);

		TextureUnit entryColorUnit, entryDepthUnit, exitColorUnit, exitDepthUnit;
		tmpEntryPointsGL->bindColorTexture(entryColorUnit.getEnum());
		tmpEntryPointsGL->bindDepthTexture(entryDepthUnit.getEnum());
		bindTextures(exitPort_, exitColorUnit.getEnum(), exitDepthUnit.getEnum());

		capNearClippingPrg_->activate();
		setGlobalShaderParameters(capNearClippingPrg_);

		capNearClippingPrg_->setUniform("entryColorTex_", entryColorUnit.getUnitNumber());
		capNearClippingPrg_->setUniform("entryDepthTex_", entryDepthUnit.getUnitNumber());
		setTextureParameters(entryPort_, capNearClippingPrg_, "entryParameters_");
		capNearClippingPrg_->setUniform("exitColorTex_", exitColorUnit.getUnitNumber());
		capNearClippingPrg_->setUniform("exitDepthTex_", exitDepthUnit.getUnitNumber());
		setTextureParameters(exitPort_, capNearClippingPrg_, "exitParameters_");

		// the rendered plane is specified in camera coordinates
		// thus we must transform from camera to world to texture coordinates
		capNearClippingPrg_->setUniform("inverseViewMat_", camera_.inverseViewMatrix());
		mat4 worldToTexMat = geom->getCoordinateTransformer().getWorldToTextureMatrix();
		capNearClippingPrg_->setUniform("world2TexMat_", worldToTexMat);

		renderImagePlaneRect();

		capNearClippingPrg_->deactivate();

        delete tmpEntryPoints;
	}

    glDepthFunc(GL_LESS);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();

    
    glDisable(GL_CULL_FACE);
}

} // namespace
