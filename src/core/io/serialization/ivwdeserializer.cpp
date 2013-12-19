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

#include <inviwo/core/io/serialization/ivwdeserializer.h>
#include <inviwo/core/util/inviwofactorybase.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/io/serialization/ivwserializable.h>
#include <inviwo/core/util/exception.h>


namespace inviwo {

IvwDeserializer::IvwDeserializer(IvwDeserializer &s, bool allowReference)
    : IvwSerializeBase(s.getFileName(), allowReference){
    registerFactories();
    try {
        readFile();
        rootElement_ = doc_.FirstChildElement();
    } catch (TxException& e) {
        throw SerializationException(e.what());
    }
}

IvwDeserializer::IvwDeserializer(std::string fileName, bool allowReference)
    : IvwSerializeBase(fileName, allowReference) {
    registerFactories();
    try {
        readFile();
        rootElement_ = doc_.FirstChildElement();
    } catch (TxException& e) {
        throw SerializationException(e.what());
    }
}

IvwDeserializer::~IvwDeserializer() {
}

void IvwDeserializer::deserialize(const std::string &key, IvwSerializable &sObj) {
    try {
        TxElement* keyNode = rootElement_->FirstChildElement(key);
        NodeSwitch tempNodeSwitch(*this, keyNode);
        sObj.deserialize(*this);
    } catch(TxException&) {}
}

void IvwDeserializer::deserializeAttributes(const std::string &key, std::string &data) {
    try {
        rootElement_->GetAttribute(key, &data);
    } catch (TxException& ) {}
}

void IvwDeserializer::deserializePrimitives(const std::string &key, std::string &data){
    TxElement* keyNode = rootElement_->FirstChildElement(key);
    keyNode->GetAttribute(IvwSerializeConstants::CONTENT_ATTRIBUTE, &data);
}

void IvwDeserializer::deserialize(const std::string &key,
                                  std::string &data,
                                  const bool asAttribute) {
    if(asAttribute) {
        deserializeAttributes(key, data);
    } else {
        try {
            deserializePrimitives(key, data);
        } catch (TxException& ) {
            //Try one more time to deserialize as string attribute (content attribute)
            try {
                deserializeAttributes(IvwSerializeConstants::CONTENT_ATTRIBUTE, data);
            }
            catch (TxException& ) {}
        }
    }
}


void IvwDeserializer::deserialize(const std::string &key, bool &data) {
    deserializePrimitives<bool>(key, data);
}
void IvwDeserializer::deserialize(const std::string &key, float &data) {
    deserializePrimitives<float>(key, data);
}
void IvwDeserializer::deserialize(const std::string &key, double &data) {
    deserializePrimitives<double>(key, data);
}
void IvwDeserializer::deserialize(const std::string &key, int &data) {
    deserializePrimitives<int>(key, data);
}
void IvwDeserializer::deserialize(const std::string &key, unsigned int &data) {
    deserializePrimitives<unsigned int>(key, data);
}
void IvwDeserializer::deserialize(const std::string &key, long &data) {
    deserializePrimitives<long>(key, data);
}
void IvwDeserializer::deserialize(const std::string &key, long long &data) {
    deserializePrimitives<long long>(key, data);
}

void IvwDeserializer::readFile() {
    try {   
        doc_.LoadFile();         
    } catch (TxException& ) {}
}

} //namespace
