#ifndef IVW_SERIALIZER_H
#define IVW_SERIALIZER_H

#ifndef TIXML_USE_TICPP
#  define TIXML_USE_TICPP
#endif

#include "inviwo/core/inviwocoredefine.h"
#include "ext/ticpp/ticpp.h"
#include "inviwo/core/io/serialization/ivwserializeconstants.h"
#include "inviwo/core/io/serialization/ivwserializebase.h"
#include "inviwo/core/inviwo.h"

namespace inviwo {

class IvwSerializable;

class IVW_CORE_API IvwSerializer : public  IvwSerializeBase {
public:
    IvwSerializer(IvwSerializer &s, bool allowReference=true);
    IvwSerializer(std::string fileName, bool allowReference=true);
    virtual ~IvwSerializer();

    virtual void writeFile(std::ostream& stream);    

    template <typename T>
    void serialize(const std::string &key, const std::vector<T> &sVector, const std::string &itemKey);
    void serialize(const std::string &key, const std::string &data, const bool asAttribute=false);
    void serialize(const std::string &key, const float &data);
    void serialize(const std::string &key, const double &data);
    void serialize(const std::string &key, const bool &data);
    void serialize(const std::string &key, const int &data);
    void serialize(const std::string &key, const long &data);
    void serialize(const std::string &key, const long long &data);
    void serialize(const std::string &key, const vec2 &data);
    void serialize(const std::string &key, const vec3 &data);
    void serialize(const std::string &key, const vec4 &data);
    void serialize(const std::string &key, const ivec2 &data);
    void serialize(const std::string &key, const ivec3 &data);
    void serialize(const std::string &key, const ivec4 &data);
    void serialize(const std::string &key, const dvec2 &data);
    void serialize(const std::string &key, const dvec3 &data);
    void serialize(const std::string &key, const dvec4 &data);
    void serialize(const std::string &key, const mat2 &data);
    void serialize(const std::string &key, const mat3 &data);
    void serialize(const std::string &key, const mat4 &data); 
    void serialize(const std::string &key, const IvwSerializable &sObj);
    template<class T>
    void serialize(const std::string& key, const T* const& data);
    
protected:
    friend class NodeSwitch;

private:


    template <typename T>
    void serializeSTL_Vector(const std::string &key, const T &sVector, const std::string &itemKey);

    void serializeAttributes(const std::string &key, const std::string &data);

    void serializePrimitives(const std::string &key, const std::string &data);
    
    template<typename T>
    void serializePrimitives(const std::string& key, const T& data);
    
    template<class T>
    void serializeVector(const std::string& key, const T& vector, const bool& isColor=false);

};

template <typename T>
inline void IvwSerializer::serialize(const std::string &key, const std::vector<T> &sVector, const std::string &itemKey) {
    serializeSTL_Vector(key, sVector, itemKey);
}

template <typename T>
inline void IvwSerializer::serializeSTL_Vector(const std::string &key, const T &sVector, const std::string &itemKey) {
    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);

    NodeSwitch tempNodeSwitch(*this, newNode);

    for (typename T::const_iterator it = sVector.begin(); it != sVector.end(); ++it) {
        if (!isPrimitiveType(typeid(typename T::value_type)) && !isPrimitivePointerType(typeid(typename T::value_type)) ) {
            serialize(itemKey, (*it));
        }
    }
}

template<class T>
inline void IvwSerializer::serialize(const std::string& key, const T* const& data) {
    
    if (!allowRef_) {
        //allowRef_ = true;
        serialize(key, *data);
        return;
    }
    else {
        if (refDataContainer_.find(data)) {
            //std::vector<TxElement*> nodeList = refDataContainer_.getNode(data);
            TxElement* newNode = refDataContainer_.nodeCopy(data);
            rootElement_->LinkEndChild(newNode);
            refDataContainer_.insert(data, newNode);
        }
        else {
            serialize(key, *data);
            refDataContainer_.insert(data, rootElement_->LastChild()->ToElement(), false);
        }
        return;
    }

    TxElement* newNode = new TxElement(key);

    if (data) {
        //std::string typeString = getTypeString(typeid(*data));
        //if (!typeString.empty()) {
        //  newNode->SetAttribute(IvwSerializeConstants::TYPE_ATTRIBUTE, typeid(*data).name());
        //}
    }
    rootElement_->LinkEndChild(newNode);
}

template<class T>
inline void IvwSerializer::serializePrimitives(const std::string& key, const T& data)
{
     TxElement* node = new TxElement(key);
     rootElement_->LinkEndChild(node);
     node->SetAttribute(IvwSerializeConstants::CONTENT_ATTRIBUTE, data);
}

template<class T>
inline void IvwSerializer::serializeVector(const std::string& key, const T& vector, const bool& isColor)
{
    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);

    std::stringstream ss;
    ss.precision(IvwSerializeConstants::STRINGSTREAM_PRECISION);
    ss<<(isColor ? static_cast<int>(vector[0]) : vector[0]);

    newNode->SetAttribute(
        isColor ? IvwSerializeConstants::COLOR_R_ATTRIBUTE : IvwSerializeConstants::VECTOR_X_ATTRIBUTE, ss.str());

    if (vector.length() >= 2) {
        ss.str(std::string());
        ss<<(isColor ? static_cast<int>(vector[1]) : vector[1]);
        newNode->SetAttribute(
            isColor ? IvwSerializeConstants::COLOR_G_ATTRIBUTE : IvwSerializeConstants::VECTOR_Y_ATTRIBUTE,ss.str());
    }

    if (vector.length() >= 3) {
        ss.str(std::string());
        ss<<(isColor ? static_cast<int>(vector[2]) : vector[2]);
        newNode->SetAttribute(
            isColor ? IvwSerializeConstants::COLOR_B_ATTRIBUTE : IvwSerializeConstants::VECTOR_Z_ATTRIBUTE,ss.str());
    }

    if (vector.length() >= 4) {
        ss.str(std::string());
        ss<<(isColor ? static_cast<int>(vector[3]) : vector[3]);
        newNode->SetAttribute(
            isColor ? IvwSerializeConstants::COLOR_A_ATTRIBUTE : IvwSerializeConstants::VECTOR_W_ATTRIBUTE,ss.str());
    }
}

} //namespace
#endif