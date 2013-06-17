#ifndef IVW_REPRESENTATIONCONVERTERFACTORY_H
#define IVW_REPRESENTATIONCONVERTERFACTORY_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/representationconverter.h>
#include <inviwo/core/util/inviwofactorybase.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

class IVW_CORE_API RepresentationConverterFactory : public Factory, 
                                                    public Singleton<RepresentationConverterFactory>  {

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
            if (repConverterTyped){
                if (repConverterTyped->canConvertFrom(source)){
                    return representationConverters_[i];
                }
            }
        }
        return NULL;
    }

    template <typename T>
    RepresentationConverterPackage<T>* getRepresentationConverterPackage(DataRepresentation* source) {
        // TODO: optimize performance, e.g., by using a hash table
        RepresentationConverterPackage<T>* currentConverterPackage = NULL; 
        for (size_t i=0; i<representationConverters_.size(); i++) {
            RepresentationConverterPackage<T>* repConverterPackage = dynamic_cast<RepresentationConverterPackage<T>*>(representationConverters_[i]);
            if (repConverterPackage){
                if (repConverterPackage->canConvertFrom(source)){
                    if (currentConverterPackage)
                        currentConverterPackage = (repConverterPackage->getNumberOfConverters() < currentConverterPackage->getNumberOfConverters() ? repConverterPackage : currentConverterPackage);
                    else
                        currentConverterPackage = repConverterPackage;
                }
            }
        }
        return currentConverterPackage;
    }

private:
    std::vector<RepresentationConverter*> representationConverters_;
};

} // namespace

#endif // IVW_REPRESENTATIONCONVERTERFACTORY_H
