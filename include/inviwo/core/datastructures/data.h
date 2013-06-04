#ifndef IVW_DATA_H
#define IVW_DATA_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/datastructures/datarepresentation.h>
#include <inviwo/core/datastructures/representationconverterfactory.h>
#include <inviwo/core/metadata/metadatamap.h>

namespace inviwo {

class IVW_CORE_API Data {

public:
    Data();
    virtual ~Data();
    Data(const Data& rhs) {
        *this = rhs;
    }
    Data& operator=(const Data& rhs) {
        if (this != &rhs) {
            rhs.copyRepresentations(this);
        }
        return *this;
    };
    //Representations
    template<class T>
    const T* getRepresentation() const;

    template<class T>
    T* getEditableRepresentation();

    template<class T>
    bool hasRepresentation() const;
    bool hasRepresentations() const;

    void addRepresentation(DataRepresentation* representation);
    void clearRepresentations();
    void copyRepresentations(Data* targetData) const;

    //MetaData
    template<typename T, typename U>
    void setMetaData(std::string key, U value);

    //param val is required to deduce the template argument
    template<typename T, typename U>
    U getMetaData(std::string key, U val) const;

    void copyMetaData(Data* targetData) const;

    void setDataFormat(DataFormatBase format);
    DataFormatBase getDataFormat() const;

    //Others
    virtual Data* clone() const = 0;

    typedef unsigned int TYPE1D;
    typedef uvec2 TYPE2D;
    typedef uvec3 TYPE3D;
    typedef uvec4 TYPE4D;

protected:
    virtual void createDefaultRepresentation() const = 0;

    template<class T>
    void invalidateAllOther();

    mutable std::vector<DataRepresentation*> representations_;
    MetaDataMap metaData_;
    DataFormatBase dataFormatBase_;
};

template<class T>
const T* Data::getRepresentation() const{
    if (!hasRepresentations()) {
        createDefaultRepresentation();
    }
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
            representations_.push_back(result);
            return dynamic_cast<T*>(result);
        }
    }
    //A one-2-one converter could not be found, thus we want to find the smallest package of converters to get to our destination
    RepresentationConverterPackage<T>* converterPackage = NULL;
    for (size_t i=0; i<representations_.size(); i++) {                
        RepresentationConverterPackage<T>* currentConverterPackage = representationConverterFactory->getRepresentationConverterPackage<T>(representations_[i]);
        if (currentConverterPackage){
            if (converterPackage){
                if (currentConverterPackage->getNumberOfConverters() < converterPackage->getNumberOfConverters()){
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
            representations_.push_back(result);
        }
        return dynamic_cast<T*>(result);
    }

    return NULL;
}

template<class T>
T* Data::getEditableRepresentation() {
    T* result = const_cast<T*>(getRepresentation<T>());
    if (representations_.size()>1)
        invalidateAllOther<T>();
    return result;
}

template<class T>
bool Data::hasRepresentation() const {
    for (size_t i=0; i<representations_.size(); i++) {
        T* representation = dynamic_cast<T*>(representations_[i]);
        if (representation) return true;
    }
    return false;
}

template<class T>
void Data::invalidateAllOther(){
    std::vector<DataRepresentation*>::iterator it = representations_.begin();
    for( ; it != representations_.end(); ) {
        T* representation = dynamic_cast<T*>((*it));
        if (!representation){
            (*it)->invalidate();
            //TODO : make updates (VolumeGL2RAMUpdate) existing representations instead of deleting old ones.
            delete (*it);
            it = representations_.erase(it);
        }
        else{
            ++it;
        }
    }
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

//param val is required to deduce the template argument
template<typename T, typename U>
U Data::getMetaData(std::string key, U val) const {
    const MetaData* baseMetaData = metaData_.get(key);

    const T* derivedMetaData = 0;
    if (baseMetaData) {
        derivedMetaData = dynamic_cast<const T*>(baseMetaData);
        //if not an instance of valid meta data, forcefully replace with valid one
        if (!derivedMetaData) {
            return val;
        }
        return derivedMetaData->get();
    }
    else {
        return val;
    }
}

/*---------------------------------------------------------------*/

/*
* T represents template argument of class
* U, V represents template arguments of member functions
*/

template <typename T>
class IVW_CORE_API DataDimension : public Data {
public:
    DataDimension(){}
    virtual ~DataDimension(){}
protected:
    template<typename U, typename V>
    U getDimension(U dimension) const;

    template<typename U, typename V>
    void setDimension(U dimension);
};

template <typename T> template<typename U, typename V>
void DataDimension<T>::setDimension(U dim) {
    Data::setMetaData<V>("dimension", dim);
}

template <typename T> template<typename U, typename V>
U DataDimension<T>::getDimension(U dimension) const {
    return Data::getMetaData<V>("dimension", dimension);
}

/*---------------------------------------------------------------*/

class Data3D : public DataDimension<Data::TYPE3D> {
public :
    typedef DataDimension<Data::TYPE3D> PARENT;
    Data3D(Data::TYPE3D dimension, DataFormatBase format);
    virtual ~Data3D();
    uvec3 getDimension() const;
    void setDimension(uvec3 dim);
    DataFormatBase getDataFormat() const;
};

/*---------------------------------------------------------------*/

class Data2D : public DataDimension<Data::TYPE2D> {
public :
    typedef DataDimension<Data::TYPE2D> PARENT;
    Data2D(Data::TYPE2D dimension, DataFormatBase format);
    virtual ~Data2D();
    uvec2 getDimension() const;
    void setDimension(uvec2 dim);
    DataFormatBase getDataFormat() const;
};

} // namespace

#endif // IVW_DATA_H
