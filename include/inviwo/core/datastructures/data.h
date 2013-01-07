#ifndef IVW_DATA_H
#define IVW_DATA_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/datastructures/datarepresentation.h"
#include "inviwo/core/datastructures/representationconverterfactory.h"

namespace inviwo {

    class Data {

    public:
        Data();
        virtual ~Data();

        template<class T>
        bool hasRepresentation() const;
        template<class T>
        T* getRepresentation();
        void addRepresentation(DataRepresentation* representation);
        void clearRepresentations();
        void copyRepresentations(Data* targetData);
        virtual Data* clone()=0;

    protected:
        std::vector<DataRepresentation*> representations_;

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
        } else {
            // no representation exists, so create one
            // using representation factory
        }

        return 0;
    }

    template<class T>
    bool Data::hasRepresentation() const {
        for (size_t i=0; i<representations_.size(); i++) {
            T* representation = dynamic_cast<T*>(representations_[i]);
            if (representation) return true;
        }
        return false;
    }
} // namespace

#endif // IVW_DATA_H
