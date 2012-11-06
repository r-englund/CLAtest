#ifndef IVW_REPRESENTATIONCONVERTERFACTORY_H
#define IVW_REPRESENTATIONCONVERTERFACTORY_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/datastructures/representationconverter.h"
#include "inviwo/core/inviwofactorybase.h"

namespace inviwo {

    class RepresentationConverterFactory : public InviwoFactoryBase  {

    public:
        RepresentationConverterFactory();
        virtual ~RepresentationConverterFactory();

        virtual void initialize();
        virtual void deinitialize();
        void registerRepresentationConverter(RepresentationConverter* representationConverter);
        RepresentationConverter* getRepresentationConverter(DataRepresentation* source, DataRepresentation* destination);

    protected:
        static RepresentationConverterFactory* factory_;

    private:
        std::vector<RepresentationConverter*> representationConverters_;
    };

} // namespace

#endif // IVW_REPRESENTATIONCONVERTERFACTORY_H
