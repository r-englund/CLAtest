#include "inviwo/core/inviwoapplication.h"
#include "inviwo/core/inviwomodule.h"
#include "inviwo/core/datastructures/representationconverterfactory.h"

namespace inviwo {

    RepresentationConverterFactory* RepresentationConverterFactory::factory_ = 0;

    RepresentationConverterFactory::RepresentationConverterFactory() {
        factory_ = this;
    }

    void RepresentationConverterFactory::initialize() {
        //TODO: check that inviwoapp is initialized

        InviwoApplication* inviwoApp = InviwoApplication::app();
        for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
            std::vector<RepresentationConverter*> repConverterList = inviwoApp->getModules()[curModuleId]->getRepresentationConverters();
            for (size_t repId=0; repId<repConverterList.size(); repId++)
                registerRepresentationConverter(repConverterList[repId]);
        }
    }

    RepresentationConverterFactory::~RepresentationConverterFactory() {}

    void RepresentationConverterFactory::deinitialize() {
    }

    void RepresentationConverterFactory::registerRepresentationConverter(RepresentationConverter* representationConverter) {
       if (std::find(representationConverters_.begin(), representationConverters_.end(), representationConverter) == representationConverters_.end())
            representationConverters_.push_back(representationConverter);
    }    


} // namespace
