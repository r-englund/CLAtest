#ifndef IVW_REPRESENTATIONCONVERTER_H
#define IVW_REPRESENTATIONCONVERTER_H

#include "inviwo/core/datastructures/datarepresentation.h"

namespace inviwo {

    class RepresentationConverter {

    public:
        RepresentationConverter();
        virtual ~RepresentationConverter();

        virtual bool canConvert(DataRepresentation* source) = 0;
        virtual DataRepresentation* convert(DataRepresentation* source) = 0;        
    };

    template <typename T>
    class RepresentationConverterType : public RepresentationConverter{};

    template <typename T>
    class RepresentationConverterPackage : public RepresentationConverter{
    public:
        RepresentationConverterPackage() : RepresentationConverter() {};
        ~RepresentationConverterPackage() {
            for (std::vector<RepresentationConverter*>::iterator it = converters_.begin() ; it != converters_.end(); ++it){
                delete (*it);
            }
        }
        bool canConvert(DataRepresentation* source){
            for (std::vector<RepresentationConverter*>::iterator it = converters_.begin() ; it != converters_.end(); ++it){
                if((*it)->canConvert(source))
                    return true;
            }
            return false;
        }
        DataRepresentation* convert(DataRepresentation* source){
            for (std::vector<RepresentationConverter*>::iterator it = converters_.begin() ; it != converters_.end(); ++it){
                if((*it)->canConvert(source))
                    return (*it)->convert(source);
            }
            return NULL;
        }  
        void addConverter(RepresentationConverter* converter) { converters_.push_back(converter); }
        size_t getNumberOfConverters() { return converters_.size(); }
    private:
        std::vector<RepresentationConverter*> converters_;
    };

} // namespace

#endif // IVW_REPRESENTATIONCONVERTER_H
