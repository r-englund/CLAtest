#include <modules/fontrendering/fontrenderingmodule.h>

#include <modules/fontrendering/processors/imageoverlay.h>

namespace inviwo {

FontrenderingModule::FontrenderingModule() : InviwoModule() {
    setIdentifier("Fontrendering");
    setXMLFileName("fontrendering/fontrenderingmodule.xml");

	registerProcessor(ImageOverlay);
}

} // namespace
