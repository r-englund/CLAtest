#include <modules/cvie3d/cvie3dmodule.h>
#include <modules/cvie3d/cvie3dprocessor.h>

namespace inviwo {

CVIE3DModule::CVIE3DModule() : InviwoModule() {
    setIdentifier("CVIE3D");
    setXMLFileName("cvie3d/cvie3dmodule.xml");

    registerProcessor(CVIE3DProcessor);
	
}

} // namespace
             