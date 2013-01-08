#ifndef IVW_DATA_H
#define IVW_DATA_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/datastructures/datarepresentation.h"
#include "inviwo/core/datastructures/representationconverterfactory.h"
#include "inviwo/core/metadata/metadatamap.h"

namespace inviwo {

    class Data {

    public:
        Data();
        virtual ~Data();

        //Representations
        template<class T>
        bool hasRepresentation() const;
        template<class T>
        T* getRepresentation();
        void addRepresentation(DataRepresentation* representation);
        void clearRepresentations();
        void copyRepresentations(Data* targetData);

        //MetaData
        template<typename T, typename U>
        void setMetaData(std::string key, U value);

        //Others
        virtual Data* clone()=0;

    protected:
        std::vector<DataRepresentation*> representations_;
        MetaDataMap metaData_;

    };

    template<class T>
    T* Data::getRepresentation() {
        if (representations_.size() > 0) {
            // check if a representation exists and return it
            for (size_t i=0; i<representations_.size(); i++) {
                T* representation = dynamic_cast<T*>(representations_[i]);
                if (representation) {
                    return representation;
                }
            }
            // no representation exists, so we try to create one
            DataRepresentation* result = 0;
            RepresentationConverterFactory* representationConverterFactory = new RepresentationConverterFactory();
            //TODO: static variable does not exist anymore in this library,
            //hence representationConverterFactory->initialize() is required. this problem is common.
            //also try to use InviwoFactoryBase::instance<RepresentationConverterFactory>()
            representationConverterFactory->initialize();
            for (size_t i=0; i<representations_.size(); i++) {                
                RepresentationConverter* converter = representationConverterFactory->getRepresentationConverter<T>(representations_[i]);
                if (converter) {
                    result = converter->convert(representations_[i]);
                    addRepresentation(result);
                    return dynamic_cast<T*>(result);
                }
            }
            //A one-2-one converter could not be found, thus we want to find the smallest package of converters to get to our destination
            RepresentationConverterPackage<T>* converterPackage = NULL;
            for (size_t i=0; i<representations_.size(); i++) {                
                RepresentationConverterPackage<T>* currentConverterPackage = representationConverterFactory->getRepresentationConverterPackage<T>(representations_[i]);
                if(currentConverterPackage){
                    if(converterPackage){
                        if(currentConverterPackage->getNumberOfConverters() < converterPackage->getNumberOfConverters()){
                            converterPackage = currentConverterPackage;
                            result = representations_[i];
                        }
                    }
                    else{
                        converterPackage = currentConverterPackage;
                        result = representations_[i];
                    }
                }
                
            }
            //Go-through the conversion package
            if (converterPackage) {
                for (size_t i=0; i<converterPackage->getNumberOfConverters(); i++) { 
                    result = converterPackage->convert(result);
                    addRepresentation(result);
                }
                return dynamic_cast<T*>(result);
            }
            return NULL;
        } else {
            // no representation exists, so create one
            // using representation factory
        }

        return NULL;
    }

    template<class T>
    bool Data::hasRepresentation() const {
        for (size_t i=0; i<representations_.size(); i++) {
            T* representation = dynamic_cast<T*>(representations_[i]);
            if (representation) return true;
        }
        return false;
    }

    template<typename T, typename U>
    void Data::setMetaData(std::string key, U value) {
        MetaData* baseMetaData = metaData_.get(key);

        T* derivedMetaData = 0;
        if (baseMetaData) {
            derivedMetaData = dynamic_cast<T*>(baseMetaData);
            //if not an instance of valid meta data, forcefully replace with valid one
            if (!derivedMetaData) {
                metaData_.remove(key);
                derivedMetaData = new T();
                metaData_.add(key, derivedMetaData);
            }
            derivedMetaData->set(value);
        }
        else {
            derivedMetaData = new T();
            metaData_.add(key, derivedMetaData);
            derivedMetaData->set(value);
        }
    }
} // namespace

#endif // IVW_DATA_H
