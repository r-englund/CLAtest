#include "inviwo/qt/widgets/inviwoapplicationqt.h"
#include "inviwo/qt/widgets/properties/propertywidgetfactoryqt.h"


namespace inviwo {

InviwoApplicationQt::InviwoApplicationQt(std::string displayName, std::string basePath,
                                         int argc, char** argv)
                                     : InviwoApplication(displayName, basePath),
                                       QApplication(argc, argv)
{
    PropertyWidgetFactoryQt::init();
}

InviwoApplicationQt::~InviwoApplicationQt() {}

} // namespace
