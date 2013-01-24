#ifndef IVW_METADATA_H
#define IVW_METADATA_H

#include "inviwo/core/inviwocoredefine.h"
#include "inviwo/core/inviwo.h"

namespace inviwo {

class IVW_CORE_API MetaData : public IvwSerializable {

public:
    MetaData();
    virtual ~MetaData();

    //TODO: Make this class abstract
    //      Now its not possible due to serializer/deserializer templating
    virtual std::string getClassName() const;
    virtual MetaData* create() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

protected:
    static const std::string logSource_; ///< Source string to be displayed for log messages.

};

/*---------------------------------------------------------------------*/

template <typename T>
class IVW_CORE_API MetaDataPrimitiveType : public MetaData {
public:
    MetaDataPrimitiveType(T value);
    void set(T value);
    T get() const;    
protected:
    T value_;
};

template <typename T>
MetaDataPrimitiveType<T>::MetaDataPrimitiveType(T value) : value_(value) {

}

template <typename T>
void MetaDataPrimitiveType<T>::set(T value) {
    value_ = value;
}

template <typename T>
T MetaDataPrimitiveType<T>::get() const {
    return value_;
}

/*---------------------------------------------------------------------*/

class IVW_CORE_API BoolMetaData : public MetaDataPrimitiveType<bool> {
public:
    BoolMetaData();
    BoolMetaData(bool value);
    virtual std::string getClassName() const;
    virtual MetaData* create() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/

class IVW_CORE_API IntMetaData : public MetaDataPrimitiveType<int> {
public:
    IntMetaData();
    IntMetaData(int value);
    virtual std::string getClassName() const;
    virtual MetaData* create() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/

class IVW_CORE_API IVec2MetaData : public MetaDataPrimitiveType<ivec2> {
public:
    IVec2MetaData();
    IVec2MetaData(ivec2 value);
    virtual std::string getClassName() const;
    virtual MetaData* create() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/

class IVW_CORE_API IVec3MetaData : public MetaDataPrimitiveType<ivec3> {
public:
    IVec3MetaData();
    IVec3MetaData(ivec3 value);
    virtual std::string getClassName() const;
    virtual MetaData* create() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/

class IVW_CORE_API FloatMetaData : public MetaDataPrimitiveType<float> {
public:
    FloatMetaData();
    FloatMetaData(float value);
    virtual std::string getClassName() const;
    virtual MetaData* create() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/

class IVW_CORE_API Vec2MetaData : public MetaDataPrimitiveType<vec2> {
public:
    Vec2MetaData();
    Vec2MetaData(vec2 value);
    virtual std::string getClassName() const;
    virtual MetaData* create() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/

class IVW_CORE_API Vec3MetaData : public MetaDataPrimitiveType<vec3> {
public:
    Vec3MetaData();
    Vec3MetaData(vec3 value);
    virtual std::string getClassName() const;
    virtual MetaData* create() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/

class StringMetaData : public MetaDataPrimitiveType<std::string> {
public:
    StringMetaData();
    StringMetaData(std::string value);
    virtual std::string getClassName() const;
    virtual MetaData* create() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/

} // namespace

#endif // IVW_PROCESSOR_H
