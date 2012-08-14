#include "inviwo/qt/widgets/inviwoapplicationqt.h"


namespace inviwo {

    InviwoApplicationQt::InviwoApplicationQt(std::string displayName, std::string basePath,
                                             int argc, char** argv)
                                         : InviwoApplication(displayName, basePath),
                                           QApplication(argc, argv)
    {}

    InviwoApplicationQt::~InviwoApplicationQt() {}

} // namespace
