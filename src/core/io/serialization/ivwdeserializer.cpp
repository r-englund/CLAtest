#include <inviwo/core/io/serialization/ivwdeserializer.h>
#include <inviwo/core/util/inviwofactorybase.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/io/serialization/ivwserializable.h>
#include <inviwo/core/common/inviwo.h>


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

void IvwDeserializer::deserialize(const std::string &key, IvwSerializable &sObj) {
    
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
            keyNode = 0;
            std::string rootKey("");
            keyNode = rootElement_->FirstChildElement();
            rootKey = keyNode->Value();
            while(rootKey!=key) {      
                keyNode = keyNode->NextSiblingElement();
                rootKey = keyNode->Value();
            };
        }
        catch (TxException& ) {
            keyNode = 0;
            return;
        }
    }

     if (keyNode) {
        NodeSwitch tempNodeSwitch(*this, keyNode);
        sObj.deserialize(*this);
     }
}

void IvwDeserializer::deserializeAttributes(const std::string &key, std::string &data) {

    TxElement* keyNode = rootElement_->FirstChildElement(); 
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

void IvwDeserializer::deserializePrimitives(const std::string &key, std::string &data) {
    TxElement* keyNode = rootElement_->FirstChildElement(key);
    //if (!keyNode) return;   
    keyNode->GetAttribute(IvwSerializeConstants::CONTENT_ATTRIBUTE, &data);
}

void IvwDeserializer::deserialize(const std::string &key, std::string &data, const bool asAttribute) {
    if (asAttribute) deserializeAttributes(key, data);
    else deserializePrimitives(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, float &data) {
    deserializePrimitives<float>(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, bool &data) {
    deserializePrimitives<bool>(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, int &data) {
    deserializePrimitives<int>(key, data);
}


void IvwDeserializer::deserialize(const std::string &key, double &data) {
    deserializePrimitives<double>(key, data);
}


void IvwDeserializer::deserialize(const std::string &key, long &data) {
    deserializePrimitives<long>(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, ivec2 &data) {
    deserializeVector(key, data);
}
void IvwDeserializer::deserialize(const std::string &key, ivec3 &data) {
    deserializeVector(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, ivec4 &data) {
    deserializeVector(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, vec2 &data) {
    deserializeVector(key, data);
}
void IvwDeserializer::deserialize(const std::string &key, vec3 &data) {
    deserializeVector(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, vec4 &data) {
    deserializeVector(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, dvec2 &data) {
    deserializeVector(key, data);
}
void IvwDeserializer::deserialize(const std::string &key, dvec3 &data) {
    deserializeVector(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, dvec4 &data) {
    deserializeVector(key, data);
}

void IvwDeserializer::deserialize(const std::string &key, mat2 &data) {
    
}
void IvwDeserializer::deserialize(const std::string &key, mat3 &data) {
    
}

void IvwDeserializer::deserialize(const std::string &key, mat4 &data) {
    
}

void IvwDeserializer::readFile() {
    try {   
        doc_.LoadFile();         
    } catch (TxException& ) {}
}

} //namespace
