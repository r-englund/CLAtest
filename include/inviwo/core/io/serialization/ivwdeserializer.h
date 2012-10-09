#ifndef IVW_DESERIALIZER_H
#define IVW_DESERIALIZER_H

#ifndef TIXML_USE_TICPP
#  define TIXML_USE_TICPP
#endif

#include "ticpp/ticpp.h"
#include "inviwo/core/io/serialization/ivwserializeconstants.h"
#include "inviwo/core/io/serialization/ivwserializebase.h"

namespace inviwo {

class IvwSerializable;

class IvwDeserializer : public  IvwSerializeBase {
public:
    IvwDeserializer(IvwDeserializer &s, bool allowReference=false) ;
    IvwDeserializer(std::string fileName, bool allowReference=false) ;
    virtual ~IvwDeserializer();

    virtual void readFile(std::ostream& stream);    

    template <typename T>
    void deserialize(const std::string &key, std::vector<T*> &sVector, const std::string &itemKey);
    void deserialize(const std::string &key, std::string &data, const bool asAttribute=false);    
    void deserialize(const std::string &key, float &data);
    void deserialize(const std::string &key, int &data);
    void deserialize(const std::string &key, vec2 &data);
    void deserialize(const std::string &key, vec3 &data);
    void deserialize(const std::string &key, vec4 &data);
    void deserialize(const std::string &key, ivec2 &data);
    void deserialize(const std::string &key, ivec3 &data);
    void deserialize(const std::string &key, ivec4 &data); 
    void deserialize(const std::string &key, IvwSerializable &sObj);
    
protected:
    friend class NodeSwitch;

private:

    template<class T>
    void deserialize(const std::string& key, T* & data);

    //void deserialize(const std::string& key, IvwSerializable* & data);

    template <typename T>
    void deserializeSTL_Vector(const std::string &key, std::vector<T*> &sVector, const std::string &itemKey);

    void deserializePrimitives(const std::string &key, std::string &data);

    void deserializeAttributes(const std::string &key, std::string &data);
    
    template<typename T>
    void deserializePrimitives(const std::string& key, T& data);
    
    template<class T>
    void deserializeVector(const std::string& key, T& vector, const bool& isColor=false);

};

template <typename T>
inline void IvwDeserializer::deserialize(const std::string &key, std::vector<T*> &sVector, const std::string &itemKey) {
    deserializeSTL_Vector(key, sVector, itemKey);
}

template <typename T>
inline void IvwDeserializer::deserializeSTL_Vector(const std::string &key, std::vector<T*> &sVector, const std::string &itemKey) {
    
    TxElement* keyNode;    
    TxElement* rootCopy = _root;  

     try {
        keyNode = _root->FirstChildElement();
        keyNode->FirstChildElement();
     }
    catch(TxException &ex) {
        try {
           keyNode = rootCopy->NextSiblingElement(); 
        }
        catch(TxException &ex) {
            keyNode = 0;
        }
        _root = keyNode;
        return;
    }
  
    typename T* item;
    std::vector<T*> tVector;

    //TODO: Add count attribute to store vector.size() if necessary
    int i=0;
    for( TxEIt child(keyNode->FirstChildElement(itemKey), itemKey); child != child.end(); ++child)
    {
        _root = &(*child);
        
        if(sVector.size()==0) {
            item = 0;
            deserialize(itemKey, item) ;
            tVector.push_back(item);
        }
        else
        {
            deserialize(itemKey, sVector[i++]) ;
            tVector.push_back(sVector[i-1]);
        }        
        
    }

    try {
       keyNode = rootCopy->NextSiblingElement(); 
    }
    catch(TxException &ex) {
        keyNode = 0;
    }

     _root = keyNode;

    sVector = tVector;
}


template<class T>
inline void IvwDeserializer::deserialize(const std::string& key, T* & data) {
    
    TxElement* keyNode;

    try {
        keyNode = _root->FirstChildElement(); 
        //if(!keyNode) return;
    }
    catch(TxException &ex) {
        keyNode = 0;
    }

    std::string attr("");
    try {
        _root->GetAttribute( IvwSerializeConstants::TYPE_ATTRIBUTE, &attr );
    }
    catch(TxException& ex) {

    }

    if(!_allowReference) {
        //_allowReference = true;
        if(!data) {
            if(!attr.empty()) {
                data = IvwSerializeBase::getRegisteredType<T>(attr);     
                if(data) {                    
                    (*data).deserialize(*this) ;
                    _root = keyNode;
                }     
            }
            else {
                data = IvwSerializeBase::getNonRegisteredType<T>();                
                deserialize(key, *data) ;
                _root = keyNode;
                
            }
        }
        else {           
            deserialize(key, *data) ;
             _root = keyNode;
        }
        
        return;
    }    
}


template<class T>
inline void IvwDeserializer::deserializePrimitives(const std::string& key, T& data) {    
    TxElement* keyNode = _root->FirstChildElement(key); 
    //if(!keyNode) return;
    keyNode->GetAttribute(IvwSerializeConstants::VALUE_ATTRIBUTE, &data);
}

template<class T>
inline void IvwDeserializer::deserializeVector(const std::string& key, T& vector, const bool& isColor) {

    TxElement* keyNode = _root->FirstChildElement(); 
    if(!keyNode) return;

    std::string attr;

    keyNode->GetAttribute(
        isColor ? IvwSerializeConstants::COLOR_R_ATTRIBUTE : IvwSerializeConstants::VECTOR_X_ATTRIBUTE, &vector[0]);

    if (vector.length() >= 2) {
        keyNode->GetAttribute(
        isColor ? IvwSerializeConstants::COLOR_G_ATTRIBUTE : IvwSerializeConstants::VECTOR_Y_ATTRIBUTE, &vector[1]);
    }

    if (vector.length() >= 3) {
        keyNode->GetAttribute(
        isColor ? IvwSerializeConstants::COLOR_B_ATTRIBUTE : IvwSerializeConstants::VECTOR_Z_ATTRIBUTE, &vector[2]);
    }

    if (vector.length() >= 4) {
       keyNode->GetAttribute(
        isColor ? IvwSerializeConstants::COLOR_A_ATTRIBUTE : IvwSerializeConstants::VECTOR_W_ATTRIBUTE, &vector[3]);
    }
}

} //namespace
#endif