#ifndef IVW_REPRESENTATIONCONVERTER_H
#define IVW_REPRESENTATIONCONVERTER_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/datastructures/datarepresentation.h>

namespace inviwo {

class IVW_CORE_API RepresentationConverter {

public:
    RepresentationConverter();
    virtual ~RepresentationConverter();

    virtual bool canConvert(const DataRepresentation* source) = 0;
    virtual DataRepresentation* createFrom(const DataRepresentation* source) = 0;    
    //virtual void update(const DataRepresentation* source, DataRepresentation* destination) = 0;
};

template <typename T>
class IVW_CORE_API RepresentationConverterType : public RepresentationConverter{};

template <typename T>
class RepresentationConverterPackage : public RepresentationConverter{
public:
    RepresentationConverterPackage() : RepresentationConverter() {};
    ~RepresentationConverterPackage() {
        for (std::vector<RepresentationConverter*>::iterator it = converters_.begin() ; it != converters_.end(); ++it){
            delete (*it);
        }
    }
    bool canConvert(const DataRepresentation* source){
        for (std::vector<RepresentationConverter*>::iterator it = converters_.begin() ; it != converters_.end(); ++it){
            if ((*it)->canConvert(source))
                return true;
        }
        return false;
    }
    DataRepresentation* createFrom(const DataRepresentation* source){
        for (std::vector<RepresentationConverter*>::iterator it = converters_.begin() ; it != converters_.end(); ++it){
            if ((*it)->canConvert(source))
                return (*it)->createFrom(source);
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
