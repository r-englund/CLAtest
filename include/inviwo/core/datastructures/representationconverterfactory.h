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
        
        template <typename T>
        RepresentationConverter* getRepresentationConverter(DataRepresentation* source) {
            // TODO: optimize performance, e.g., by using a hash table
            for (size_t i=0; i<representationConverters_.size(); i++) {
                RepresentationConverterType<T>* repConverterTyped = dynamic_cast<RepresentationConverterType<T>*>(representationConverters_[i]);
                if(repConverterTyped){
                    if (repConverterTyped->canConvert(source)){
                        return representationConverters_[i];
                    }
                }
            }
            return NULL; //TODO: throw an exception here
        }

        template <typename T>
        RepresentationConverterPackage<T>* getRepresentationConverterPackage(DataRepresentation* source) {
            // TODO: optimize performance, e.g., by using a hash table
            RepresentationConverterPackage<T>* currentConverterPackage = NULL; 
            for (size_t i=0; i<representationConverters_.size(); i++) {
                RepresentationConverterPackage<T>* repConverterPackage = dynamic_cast<RepresentationConverterPackage<T>*>(representationConverters_[i]);
                if(repConverterPackage){
                    if (repConverterPackage->canConvert(source)){
                        if(currentConverterPackage)
                            currentConverterPackage = (repConverterPackage->getNumberOfConverters() < currentConverterPackage->getNumberOfConverters() ? repConverterPackage : currentConverterPackage);
                    }
                }
            }
            return currentConverterPackage; //TODO: throw an exception here
        }

    protected:
        static RepresentationConverterFactory* factory_;

    private:
        std::vector<RepresentationConverter*> representationConverters_;
        int largestPac;
    };

} // namespace

#endif // IVW_REPRESENTATIONCONVERTERFACTORY_H
