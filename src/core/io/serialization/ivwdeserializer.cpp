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
    : IvwSerializeBase(s.getFileName(), allowReference)
{
    registerFactories();

    try {
        readFile();

        TxElement *inviwoTree = doc_.FirstChildElement();
        //Comment is not a tree element
        //TxElement *comment = inviwoTree->FirstChildElement();
        rootElement_ = inviwoTree;
    }
    catch (TxException& ) {
        
    }
}

IvwDeserializer::IvwDeserializer(std::string fileName, bool allowReference)
    : IvwSerializeBase(fileName, allowReference) {
    
    registerFactories();

    try {
        readFile();

        TxElement *inviwoTree = doc_.FirstChildElement();
        //Comment is not a tree element
        //TxElement *comment = inviwoTree->FirstChildElement();
        rootElement_ = inviwoTree; 
    }
    catch (TxException& ) {
        
    }
}

IvwDeserializer::~IvwDeserializer() {
}

void IvwDeserializer::deserialize(const std::string &key, IvwSerializable &sObj) throw (SerializationException) {
    
    TxElement* keyNode = 0;
    
    try {
        std::string rootKey("");
        keyNode = rootElement_;        
        rootKey = keyNode->Value();

        while(rootKey!=key) { 
            keyNode = keyNode->NextSiblingElement();
            rootKey = keyNode->Value();
        };
    }
    catch (TxException& ) {
        try {
            //do not throw anymore exceptions, return silently if key not found
            keyNode = 0;
            std::string rootKey("");
            keyNode = rootElement_->FirstChildElement(false);
            if (keyNode) {
                rootKey = keyNode->Value();
                while(rootKey!=key) {      
                    keyNode = keyNode->NextSiblingElement(false);
                    if (!keyNode) break;
                    rootKey = keyNode->Value(); 
                };
            }
        }
        catch (TxException& ) {
            keyNode = 0;
            throw SerializationException("Deserialize Exception.");
        }
    }

     if (keyNode) {
        NodeSwitch tempNodeSwitch(*this, keyNode);
        sObj.deserialize(*this);
     }
}

void IvwDeserializer::deserializeAttributes(const std::string &key, std::string &data) throw (SerializationException) {

    TxElement* keyNode = rootElement_->FirstChildElement(false); 
    //if (!keyNode) return;      

    try {
        rootElement_->GetAttribute(key, &data);
        //lastAttribute = rootElement_->LastAttribute();
        TxAIt attribute;
        TxAIt lastAttribute;
        for ( attribute = attribute.begin( rootElement_ ); attribute != attribute.end(); attribute++ ) {
            lastAttribute = attribute;
        }

        if ( lastAttribute->Name() == key) {
            if ( lastAttribute->Value() == data) {
                if (keyNode && keyNode->FirstAttribute()) {
                    if (IvwSerializeConstants::isReversvedAttribute(keyNode->FirstAttribute()->Name())) {
                        //If next element has valid attributes
                        rootElement_ = keyNode; 
                    }
                }                   
            }
        }
    } catch (TxException& ) {}
}

void IvwDeserializer::deserializePrimitives(const std::string &key, std::string &data) throw (SerializationException) {
    TxElement* keyNode = rootElement_->FirstChildElement(key);
    //if (!keyNode) return;   
    keyNode->GetAttribute(IvwSerializeConstants::CONTENT_ATTRIBUTE, &data);
}

void IvwDeserializer::deserialize(const std::string &key, std::string &data, const bool asAttribute) throw (SerializationException) {
    if (asAttribute) deserializeAttributes(key, data);
    else {
        try {
            deserializePrimitives(key, data);
        }
        catch (TxException& ) {
            //Try one more time to deserialize as string attribute (content attribute)
            try {
                deserializeAttributes(IvwSerializeConstants::CONTENT_ATTRIBUTE, data);
            }
            catch (TxException& ) {}
        }
    }
}

void IvwDeserializer::deserialize(const std::string &key, float &data) throw (SerializationException) {
    deserializePrimitives<float>(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, bool &data) throw (SerializationException) {
    deserializePrimitives<bool>(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, int &data) throw (SerializationException) {
    deserializePrimitives<int>(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, unsigned int &data) throw (SerializationException) {
    deserializePrimitives<unsigned int>(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, double &data) throw (SerializationException) {
    deserializePrimitives<double>(key, data);
}


void IvwDeserializer::deserialize(const std::string &key, long &data) throw (SerializationException) {
    deserializePrimitives<long>(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, ivec2 &data) throw (SerializationException) {
    deserializeVector(key, data);
}
void IvwDeserializer::deserialize(const std::string &key, ivec3 &data) throw (SerializationException) {
    deserializeVector(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, ivec4 &data) throw (SerializationException) {
    deserializeVector(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, uvec2 &data) throw (SerializationException) {
    deserializeVector(key, data);
}
void IvwDeserializer::deserialize(const std::string &key, uvec3 &data) throw (SerializationException) {
    deserializeVector(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, uvec4 &data) throw (SerializationException) {
    deserializeVector(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, vec2 &data) throw (SerializationException) {
    deserializeVector(key, data);
}
void IvwDeserializer::deserialize(const std::string &key, vec3 &data) throw (SerializationException) {
    deserializeVector(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, vec4 &data) throw (SerializationException) {
    deserializeVector(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, dvec2 &data) throw (SerializationException) {
    deserializeVector(key, data);
}
void IvwDeserializer::deserialize(const std::string &key, dvec3 &data) throw (SerializationException) {
    deserializeVector(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, dvec4 &data) throw (SerializationException) {
    deserializeVector(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, mat2 &data) throw (SerializationException) {
    TxElement* keyNode = new TxElement(key);
    try {
        keyNode = rootElement_->FirstChildElement(key);
        keyNode->FirstChildElement();
    } catch (TxException&) {
        return;
    }

    NodeSwitch tempNodeSwitch(*this, keyNode);

    vec2 rowVec;
    for (size_t i=0; i<2; i++) {
        std::stringstream key;
        key << "row" << i;        
        deserializeVector(key.str(), rowVec);
        data[i][0] = rowVec[0]; data[i][1] = rowVec[1];
    }    
}
void IvwDeserializer::deserialize(const std::string &key, mat3 &data) throw (SerializationException) {
    TxElement* keyNode = new TxElement(key);
    try {
        keyNode = rootElement_->FirstChildElement(key);
        keyNode->FirstChildElement();
    } catch (TxException&) {
        return;
    }

    NodeSwitch tempNodeSwitch(*this, keyNode);

    vec3 rowVec;
    for (size_t i=0; i<3; i++) {
        std::stringstream key;
        key << "row" << i;        
        deserializeVector(key.str(), rowVec);
        data[i][0] = rowVec[0]; data[i][1] = rowVec[1]; data[i][2] = rowVec[2];
    }   
}

void IvwDeserializer::deserialize(const std::string &key, mat4 &data) throw (SerializationException) {
    TxElement* keyNode = new TxElement(key);
    try {
        keyNode = rootElement_->FirstChildElement(key);
        keyNode->FirstChildElement();
    } catch (TxException&) {
        return;
    }

    NodeSwitch tempNodeSwitch(*this, keyNode);

    vec4 rowVec;
    for (size_t i=0; i<4; i++) {
        std::stringstream key;
        key << "row" << i;        
        deserializeVector(key.str(), rowVec);
        data[i][0] = rowVec[0]; data[i][1] = rowVec[1]; data[i][2] = rowVec[2]; data[i][3] = rowVec[3];
    }     
}

void IvwDeserializer::readFile() throw (SerializationException) {
    try {   
        doc_.LoadFile();         
    } catch (TxException& ) {}
}

} //namespace
