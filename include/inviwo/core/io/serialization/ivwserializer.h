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
#include <inviwo/core/util/exception.h>

namespace inviwo {

class IvwSerializable;

class IVW_CORE_API IvwSerializer : public IvwSerializeBase {
public:
    IvwSerializer(IvwSerializer& s, bool allowReference=true);
    IvwSerializer(std::string fileName, bool allowReference=true);
    virtual ~IvwSerializer();

    virtual void writeFile();

    // std containers
    template <typename T>
    void serialize(const std::string& key,
                   const std::vector<T>& sVector,
                   const std::string& itemKey);

    template <typename K, typename V, typename C, typename A>
    void serialize(const std::string& key,
                   const std::map<K,V,C,A>& sMap,
                   const std::string& itemKey);


    // strings
    void serialize(const std::string& key,
                   const std::string& data,
                   const bool asAttribute=false);

    // primitive types
    void serialize(const std::string& key, const bool& data);
    void serialize(const std::string& key, const float& data);
    void serialize(const std::string& key, const double& data);
    void serialize(const std::string& key, const int& data);
    void serialize(const std::string& key, const unsigned int& data);
    void serialize(const std::string& key, const long& data);
    void serialize(const std::string& key, const long long& data);
    void serialize(const std::string& key, const unsigned long long& data);

    // glm vector types
    template<class T>
    void serialize(const std::string& key, const glm::detail::tvec4<T>& data);
    template<class T>
    void serialize(const std::string& key, const glm::detail::tvec3<T>& data);
    template<class T>
    void serialize(const std::string& key, const glm::detail::tvec2<T>& data);
    // glm matrix types
    template<class T>
    void serialize(const std::string& key, const glm::detail::tmat4x4<T>& data);
    template<class T>
    void serialize(const std::string& key, const glm::detail::tmat3x3<T>& data);
    template<class T>
    void serialize(const std::string& key, const glm::detail::tmat2x2<T>& data);

    // serializable classes
    void serialize(const std::string& key, const IvwSerializable& sObj);

    // pointers to something of the above.
    template<class T>
    void serialize(const std::string& key, const T* const& data);

protected:
    friend class NodeSwitch;

private:
    template<typename T>
    void serializePrimitives(const std::string& key, const T& data);

    template<class T>
    void serializeVector(const std::string& key, const T& vector, const bool& isColor=false);

    void initialize();

};


template <typename T>
void IvwSerializer::serialize(const std::string& key,
                              const std::vector<T>& sVector,
                              const std::string& itemKey) {
    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);
    NodeSwitch tempNodeSwitch(*this, newNode);

    for (typename std::vector<T>::const_iterator it = sVector.begin();
         it != sVector.end(); ++it)
        serialize(itemKey, (*it));

    delete newNode;
}


template <typename K, typename V, typename C, typename A>
void IvwSerializer::serialize(const std::string& key,
                              const std::map<K,V,C,A>& sMap,
                              const std::string& itemKey) {
    if (!isPrimitiveType(typeid(K)))
        throw SerializationException("Error: map key has to be a primitive type");

    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);
    NodeSwitch tempNodeSwitch(*this, newNode);

    for (typename std::map<K, V, C, A>::const_iterator it = sMap.begin();
         it != sMap.end(); ++it) {
        serialize(itemKey, it->second);
        rootElement_->LastChild()->ToElement()->SetAttribute(IvwSerializeConstants::KEY_ATTRIBUTE,
                it->first);
    }

    delete newNode;
}


template<class T>
inline void IvwSerializer::serialize(const std::string& key, const T* const& data) {
    if (!allowRef_)
        serialize(key, *data);
    else {
        if (refDataContainer_.find(data)) {
            TxElement* newNode = refDataContainer_.nodeCopy(data);
            rootElement_->LinkEndChild(newNode);
            refDataContainer_.insert(data, newNode);
        } else {
            serialize(key, *data);
            refDataContainer_.insert(data, rootElement_->LastChild()->ToElement(), false);
        }
    }
}

template<class T>
inline void IvwSerializer::serializePrimitives(const std::string& key, const T& data) {
    TxElement* node = new TxElement(key);
    rootElement_->LinkEndChild(node);
    node->SetAttribute(IvwSerializeConstants::CONTENT_ATTRIBUTE, data);
    delete node;
}

template<class T>
void IvwSerializer::serialize(const std::string& key, const glm::detail::tvec4<T>& data) {
    serializeVector(key, data);
}
template<class T>
void IvwSerializer::serialize(const std::string& key, const glm::detail::tvec3<T>& data) {
    serializeVector(key, data);
}
template<class T>
void IvwSerializer::serialize(const std::string& key, const glm::detail::tvec2<T>& data) {
    serializeVector(key, data);
}

template<class T>
void IvwSerializer::serialize(const std::string& key, const glm::detail::tmat4x4<T>& data) {
    glm::detail::tvec4<T> rowVec;
    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);
    NodeSwitch tempNodeSwitch(*this, newNode);

    for (size_t i=0; i<4; i++) {
        std::stringstream key;
        key << "row" << i;
        rowVec = glm::detail::tvec4<T>(data[i][0], data[i][1], data[i][2], data[i][3]);
        serializeVector(key.str(), rowVec);
    }

    delete newNode;
}
template<class T>
void IvwSerializer::serialize(const std::string& key, const glm::detail::tmat3x3<T>& data) {
    glm::detail::tvec3<T> rowVec;
    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);
    NodeSwitch tempNodeSwitch(*this, newNode);

    for (size_t i=0; i<3; i++) {
        std::stringstream key;
        key << "row" << i;
        rowVec = glm::detail::tvec3<T>(data[i][0], data[i][1], data[i][2]);
        serializeVector(key.str(), rowVec);
    }

    delete newNode;
}
template<class T>
void IvwSerializer::serialize(const std::string& key, const glm::detail::tmat2x2<T>& data) {
    glm::detail::tvec2<T> rowVec;
    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);
    NodeSwitch tempNodeSwitch(*this, newNode);

    for (size_t i=0; i<2; i++) {
        std::stringstream key;
        key << "row" << i;
        rowVec = glm::detail::tvec2<T>(data[i][0], data[i][1]);
        serializeVector(key.str(), rowVec);
    }

    delete newNode;
}

template<class T>
inline void IvwSerializer::serializeVector(const std::string& key,
        const T& vector,
        const bool& isColor) {
    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);
    std::stringstream ss;
    ss.precision(IvwSerializeConstants::STRINGSTREAM_PRECISION);
    ss<<(isColor ? static_cast<int>(vector[0]) : vector[0]);
    newNode->SetAttribute(isColor ? IvwSerializeConstants::COLOR_R_ATTRIBUTE :
                          IvwSerializeConstants::VECTOR_X_ATTRIBUTE,
                          ss.str());

    if (vector.length() >= 2) {
        ss.str(std::string());
        ss<<(isColor ? static_cast<int>(vector[1]) : vector[1]);
        newNode->SetAttribute(isColor ? IvwSerializeConstants::COLOR_G_ATTRIBUTE :
                              IvwSerializeConstants::VECTOR_Y_ATTRIBUTE,
                              ss.str());
    }

    if (vector.length() >= 3) {
        ss.str(std::string());
        ss<<(isColor ? static_cast<int>(vector[2]) : vector[2]);
        newNode->SetAttribute(isColor ? IvwSerializeConstants::COLOR_B_ATTRIBUTE :
                              IvwSerializeConstants::VECTOR_Z_ATTRIBUTE,
                              ss.str());
    }

    if (vector.length() >= 4) {
        ss.str(std::string());
        ss<<(isColor ? static_cast<int>(vector[3]) : vector[3]);
        newNode->SetAttribute(isColor ? IvwSerializeConstants::COLOR_A_ATTRIBUTE :
                              IvwSerializeConstants::VECTOR_W_ATTRIBUTE,
                              ss.str());
    }

    delete newNode;
}

} //namespace
#endif
