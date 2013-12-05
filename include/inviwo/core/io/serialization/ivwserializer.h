/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_SERIALIZER_H
#define IVW_SERIALIZER_H

#include <inviwo/core/io/serialization/ivwserializebase.h>

namespace inviwo {

class IvwSerializable;

class IVW_CORE_API IvwSerializer : public IvwSerializeBase {
public:
    IvwSerializer(IvwSerializer &s, bool allowReference=true);
    IvwSerializer(std::string fileName, bool allowReference=true);
    virtual ~IvwSerializer();

    virtual void writeFile();

    template <typename T>
    void serialize(const std::string &key, const std::vector<T> &sVector, const std::string &itemKey);
    template <typename K, typename V, typename C>
    void serialize(const std::string &key, const std::map<K,V,C> &sMap, const std::string &itemKey);
    template <typename K, typename V, typename C>
    void serialize(const std::string &key, const std::map<K,V,C> &sMap);
    void serialize(const std::string &key, const std::string &data, const bool asAttribute=false);
    void serialize(const std::string &key, const float &data);
    void serialize(const std::string &key, const double &data);
    void serialize(const std::string &key, const bool &data);
    void serialize(const std::string &key, const int &data);
    void serialize(const std::string &key, const unsigned int &data);
    void serialize(const std::string &key, const long &data);
    void serialize(const std::string &key, const long long &data);
    void serialize(const std::string &key, const vec2 &data);
    void serialize(const std::string &key, const vec3 &data);
    void serialize(const std::string &key, const vec4 &data);
    void serialize(const std::string &key, const ivec2 &data);
    void serialize(const std::string &key, const ivec3 &data);
    void serialize(const std::string &key, const ivec4 &data);
    void serialize(const std::string &key, const uvec2 &data);
    void serialize(const std::string &key, const uvec3 &data);
    void serialize(const std::string &key, const uvec4 &data);
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

    template <typename T>
    void serializeSTL_Map(const std::string &key, const T &sMap, const std::string &itemKey);

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

template <typename K, typename V, typename C>
inline void IvwSerializer::serialize(const std::string &key, const std::map<K,V,C> &sMap, const std::string &itemKey) {
    serializeSTL_Map(key, sMap, itemKey);
}

template <typename T>
inline void IvwSerializer::serializeSTL_Vector(const std::string &key, const T &sVector, const std::string &itemKey) {
    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);

    NodeSwitch tempNodeSwitch(*this, newNode);

    for (typename T::const_iterator it = sVector.begin(); it != sVector.end(); ++it) {
        if (!isPrimitiveType(typeid(typename T::value_type)) && !isPrimitivePointerType(typeid(typename T::value_type)) ) {
            //Handle non primitive cases if necessary
        }
        serialize(itemKey, (*it));
    }
}

template <typename T>
inline void IvwSerializer::serializeSTL_Map(const std::string &key, const T &sMap, const std::string &itemKey) {
    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);

    NodeSwitch tempNodeSwitch(*this, newNode);

    for (typename T::const_iterator it = sMap.begin(); it != sMap.end(); ++it) {
        if (!isPrimitiveType(typeid(typename T::mapped_type)) && !isPrimitivePointerType(typeid(typename T::mapped_type)) ) {        
            //Handle non primitive cases if necessary
        }
        serialize(itemKey, it->second);
    }
}

template <typename K, typename V, typename C>
void IvwSerializer::serialize(const std::string &key, const std::map<K,V,C>& sMap) {
    if(!isPrimitiveType(typeid(K))){
        throw SerializationException("Error: map key has to be a primitive type");
    }

    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);

    NodeSwitch tempNodeSwitch(*this, newNode);

    for (std::map<typename K, typename V, typename C>::const_iterator it = sMap.begin(); it != sMap.end(); ++it) {
        serialize(it->first, it->second);
        //rootElement_->LastChild()->SetAttribute(IvwSerializeConstants::TYPE_ATTRIBUTE,  it->second->getClassName());
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