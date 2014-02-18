/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Peter Steneteg, Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/core/io/serialization/ivwdeserializer.h>
#include <inviwo/core/util/factory.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/io/serialization/ivwserializable.h>
#include <inviwo/core/util/exception.h>


namespace inviwo {

IvwDeserializer::IvwDeserializer(IvwDeserializer& s, bool allowReference)
    : IvwSerializeBase(s.getFileName(), allowReference) {
    registerFactories();

    try {
        readFile();
    } catch (TxException& e) {
        throw SerializationException(e.what());
    }
}

IvwDeserializer::IvwDeserializer(std::string fileName, bool allowReference)
    : IvwSerializeBase(fileName, allowReference) {
    registerFactories();

    try {
        readFile();
    } catch (TxException& e) {
        throw SerializationException(e.what());
    }
}

IvwDeserializer::~IvwDeserializer() {
}

void IvwDeserializer::deserialize(const std::string& key, IvwSerializable& sObj) {
    try {
        TxElement* keyNode;

        if (getChild_)
            keyNode = rootElement_->FirstChildElement(key);
        else
            keyNode = rootElement_;

        NodeSwitch tempNodeSwitch(*this, keyNode);
        sObj.deserialize(*this);
    } catch (TxException&) {}
}

void IvwDeserializer::deserializeAttributes(const std::string& key, std::string& data) {
    try {
        rootElement_->GetAttribute(key, &data);
    } catch (TxException&) {}
}

void IvwDeserializer::deserializePrimitives(const std::string& key, std::string& data) {
    if (getChild_) {
        rootElement_->FirstChildElement(key)->GetAttribute(
            IvwSerializeConstants::CONTENT_ATTRIBUTE, &data);
    } else
        rootElement_->GetAttribute(IvwSerializeConstants::CONTENT_ATTRIBUTE, &data);
}

void IvwDeserializer::deserialize(const std::string& key,
                                  std::string& data,
                                  const bool asAttribute) {
    if (asAttribute)
        deserializeAttributes(key, data);
    else {
        try {
            deserializePrimitives(key, data);
        } catch (TxException&) {
            //Try one more time to deserialize as string attribute (content attribute)
            try {
                deserializeAttributes(IvwSerializeConstants::CONTENT_ATTRIBUTE, data);
            }
            catch (TxException&) {}
        }
    }
}


void IvwDeserializer::deserialize(const std::string& key, bool& data) {
    deserializePrimitives<bool>(key, data);
}
void IvwDeserializer::deserialize(const std::string& key, float& data) {
    deserializePrimitives<float>(key, data);
}
void IvwDeserializer::deserialize(const std::string& key, double& data) {
    deserializePrimitives<double>(key, data);
}
void IvwDeserializer::deserialize(const std::string& key, signed char& data) {
    deserializePrimitives<signed char>(key, data);
}
void IvwDeserializer::deserialize(const std::string& key, unsigned char& data) {
    deserializePrimitives<unsigned char>(key, data);
}
void IvwDeserializer::deserialize(const std::string& key, char& data) {
    deserializePrimitives<char>(key, data);
}
void IvwDeserializer::deserialize(const std::string& key, short& data) {
    deserializePrimitives<short>(key, data);
}
void IvwDeserializer::deserialize(const std::string& key, unsigned short& data) {
    deserializePrimitives<unsigned short>(key, data);
}
void IvwDeserializer::deserialize(const std::string& key, int& data) {
    deserializePrimitives<int>(key, data);
}
void IvwDeserializer::deserialize(const std::string& key, unsigned int& data) {
    deserializePrimitives<unsigned int>(key, data);
}
void IvwDeserializer::deserialize(const std::string& key, long& data) {
    deserializePrimitives<long>(key, data);
}
void IvwDeserializer::deserialize(const std::string& key, long long& data) {
    deserializePrimitives<long long>(key, data);
}
void IvwDeserializer::deserialize(const std::string& key, unsigned long long& data) {
    deserializePrimitives<unsigned long long>(key, data);
}

void IvwDeserializer::readFile() {
    try {
        doc_.LoadFile();
        rootElement_ = doc_.FirstChildElement();
    } catch (TxException&) {
        std::stringstream ss;
        ss << "Error reading file: " << IvwSerializeBase::fileName_;
        throw (AbortException(ss.str()));
    }
}

} //namespace
