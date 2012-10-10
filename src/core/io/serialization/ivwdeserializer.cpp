#include "inviwo/core/io/serialization/ivwdeserializer.h"
#include "inviwo/core/inviwofactorybase.h"
#include "inviwo/core/processors/processorfactory.h"
#include "inviwo/core/io/serialization/ivwserializable.h"
#include "inviwo/core/inviwo.h"


namespace inviwo {

IvwDeserializer::IvwDeserializer(IvwDeserializer &s, bool allowReference)
    : IvwSerializeBase(s.getFileName(), allowReference)
{
    registerFactories();

    try {
        readFile(std::cout);

        TxElement *inviwoTree = _doc.FirstChildElement();

        //Comment is not a tree element
        //TxElement *comment = inviwoTree->FirstChildElement();

        _root = inviwoTree; 

    }
    catch(TxException& ex) {
        
    }
}

IvwDeserializer::IvwDeserializer(std::string fileName, bool allowReference)
    : IvwSerializeBase(fileName, allowReference) {
    
    registerFactories();

    try {
        readFile(std::cout);

        TxElement *inviwoTree = _doc.FirstChildElement();

        //Comment is not a tree element
        //TxElement *comment = inviwoTree->FirstChildElement();

        _root = inviwoTree; 

    }
    catch(TxException& ex) {
        
    }
}

IvwDeserializer::~IvwDeserializer() {
}

void IvwDeserializer::deserialize(const std::string &key, IvwSerializable &sObj) {
    sObj.deserialize(*this) ; 
}

/*
inline void IvwDeserializer::deserialize(const std::string& key, IvwSerializable* & data) {
    
    TxElement* keyNode = _root->FirstChildElement(); 
    if(!keyNode) return;

    //NodeSwitch tempNodeSwitch(*this, keyNode);
    std::string attr("");
    try {
        keyNode->GetAttribute( IvwSerializeConstants::TYPE_ATTRIBUTE, &attr );
    }
    catch(TxException& ex) {

    }

    if(!_allowReference) {
        //_allowReference = true;
        if(!data) {
            if(!attr.empty()) {
                data = IvwSerializeBase::getRegisteredType(attr);     
                if(data) {
                    _root = keyNode;
                    (*data).deserialize(*this) ;
                }
            }
            else
            {
                //Possibly a non-registered type
            }
        }
        else {
            _root = keyNode;
            (*data).deserialize(*this) ;
        }
        
        return;
    }    
}
*/

void IvwDeserializer::deserializeAttributes(const std::string &key, std::string &data) {

    TxElement* keyNode = _root->FirstChildElement(); 
    //if(!keyNode) return;   

    _root->GetAttribute(key, &data);
    TxAttribute* currAttribute = 0;

    try {
        
        //lastAttribute = _root->LastAttribute();
        TxAIt attribute;
        TxAIt lastAttribute;
        for ( attribute = attribute.begin( _root ); attribute != attribute.end(); attribute++ ) {
            lastAttribute = attribute;
        }

        if( lastAttribute->Name() == key)
        {
            if( lastAttribute->Value() == data)
            {
                if(keyNode && keyNode->FirstAttribute())
                {
                    if(IvwSerializeConstants::isReversvedAttribute(keyNode->FirstAttribute()->Name())) {
                        //If next element has valid attributes
                        _root = keyNode; 
                    }
                }                   
            }
        }
       

    }
    catch(TxException& ex) {
    }
}

void IvwDeserializer::deserializePrimitives(const std::string &key, std::string &data) {

    TxElement* keyNode = _root->FirstChildElement(key); 
    //if(!keyNode) return;   

    keyNode->GetAttribute(IvwSerializeConstants::CONTENT_ATTRIBUTE, &data);
    
}

void IvwDeserializer::deserialize(const std::string &key, std::string &data, const bool asAttribute) {
    if(asAttribute) {
        deserializeAttributes(key, data) ;
    }
    else {
        deserializePrimitives(key, data) ;
    }
}

void IvwDeserializer::deserialize(const std::string &key, float &data) {
    deserializePrimitives<float>(key, data) ;
}

void IvwDeserializer::deserialize(const std::string &key, int &data) {
    deserializePrimitives<int>(key, data) ;
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


void IvwDeserializer::readFile(std::ostream& stream) {

    try {   
        _doc.LoadFile(); 
        //stream<<_doc;
    }
    catch(TxException& ex) {
        
    }

}


} //namespace
