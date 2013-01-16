#ifndef IVW_DESERIALIZER_H
#define IVW_DESERIALIZER_H

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

class IVW_CORE_API IvwDeserializer : public  IvwSerializeBase {
public:
    IvwDeserializer(IvwDeserializer &s, bool allowReference=true);
    IvwDeserializer(std::string fileName, bool allowReference=true);
    virtual ~IvwDeserializer();
    virtual void readFile(std::ostream& stream);

    template <typename T>
    void deserialize(const std::string &key, std::vector<T*> &sVector, const std::string &itemKey);
    void deserialize(const std::string &key, std::string &data, const bool asAttribute=false);    
    void deserialize(const std::string &key, bool &data);
    void deserialize(const std::string &key, float &data);
    void deserialize(const std::string &key, double &data);
    void deserialize(const std::string &key, int &data);
    void deserialize(const std::string &key, long &data);
    void deserialize(const std::string &key, vec2 &data);
    void deserialize(const std::string &key, vec3 &data);
    void deserialize(const std::string &key, vec4 &data);
    void deserialize(const std::string &key, ivec2 &data);
    void deserialize(const std::string &key, ivec3 &data);
    void deserialize(const std::string &key, ivec4 &data); 
    void deserialize(const std::string &key, dvec2 &data);
    void deserialize(const std::string &key, dvec3 &data);
    void deserialize(const std::string &key, dvec4 &data); 
    void deserialize(const std::string &key, mat2 &data);
    void deserialize(const std::string &key, mat3 &data);
    void deserialize(const std::string &key, mat4 &data); 
    void deserialize(const std::string &key, IvwSerializable &sObj);
    template <class T>
    void deserialize(const std::string& key, T* & data);
    
protected:
    friend class NodeSwitch;
private:
    //void deserialize(const std::string& key, IvwSerializable* & data);
    template <typename T>
    void deserializeSTL_Vector(const std::string &key, std::vector<T*> &sVector, const std::string &itemKey);

    void deserializePrimitives(const std::string &key, std::string &data);

    void deserializeAttributes(const std::string &key, std::string &data);
    
    template <typename T>
    void deserializePrimitives(const std::string& key, T& data);
    
    template <class T>
    void deserializeVector(const std::string& key, T& vector, const bool& isColor=false);

    template <class T>
    void deserializePointer(const std::string& key, T* & data);

};

template <typename T>
inline IVW_CORE_API void IvwDeserializer::deserialize(const std::string &key, std::vector<T*> &sVector, const std::string &itemKey) {
    deserializeSTL_Vector(key, sVector, itemKey);
}

template <typename T>
inline IVW_CORE_API void IvwDeserializer::deserializeSTL_Vector(const std::string &key, std::vector<T*> &sVector, const std::string &itemKey) {
    
    TxElement* keyNode;


    try {
        keyNode = rootElement_->FirstChildElement(key);
        keyNode->FirstChildElement();
    } catch (TxException&) {
        return;
    }

    NodeSwitch tempNodeSwitch(*this, keyNode);
  
    T* item;
    std::vector<T*> tVector;

    //TODO: Add count attribute to store vector.size() if necessary
    unsigned int i=0;
    for (TxEIt child(keyNode->FirstChildElement(itemKey), itemKey); child != child.end(); ++child) {
        rootElement_ = &(*child);
        
        if (sVector.size()==0) {
            item = 0;
            deserializePointer(itemKey, item);
            tVector.push_back(item);
        }
        else {
            deserializePointer(itemKey, sVector[i++]);
            tVector.push_back(sVector[i-1]);
        }        
        
    }


    sVector = tVector;
}

template<class T>
inline IVW_CORE_API void IvwDeserializer::deserializePointer(const std::string& /*key*/, T* & data) {
    
    TxElement* nextRootNode;
    TxElement* rootCopy = rootElement_;

    try {
        nextRootNode = rootElement_->FirstChildElement(); 
        //if (!keyNode) return;
    }
    catch (TxException& ) {
        nextRootNode = 0;
    }

    std::string type_attr("");
    std::string ref_attr("");
    std::string id_attr("");
    try {
        rootElement_->GetAttribute(IvwSerializeConstants::TYPE_ATTRIBUTE, &type_attr);
        if (allowRef_) {
            try {
                rootElement_->GetAttribute(IvwSerializeConstants::REF_ATTRIBUTE, &ref_attr);
            } catch (TxException& ) {}
            rootElement_->GetAttribute(IvwSerializeConstants::ID_ATTRIBUTE, &id_attr);
        }
    } catch (TxException& ) {}


    //allocation of referenced data
    if (!data) {
        if (!ref_attr.empty()) {
            //allocate only

            //search in reference list
            //if data doesnt exist just allocate
            // and add to list
            //return the data, donot deserialize it yet

           data=static_cast<T*>(refDataContainer_.find(type_attr, ref_attr));

            if (!data) {
                if (!type_attr.empty()) {
                    data = IvwSerializeBase::getRegisteredType<T>(type_attr);                        
                }
                else {
                    data = IvwSerializeBase::getNonRegisteredType<T>();                    
                }
                if (data) refDataContainer_.insert(data, rootCopy);
            }            
            
            rootElement_ = nextRootNode;

            return;

        }
        else if (!id_attr.empty()) {
            //search in the reference list
            //if data exist , data needs to deserialized but not allocated
            //else data needs to be both allocated and deserialized

            data = static_cast<T*>(refDataContainer_.find(type_attr, id_attr));
            
            if (!data) {
                if (!type_attr.empty()) {
                    data = IvwSerializeBase::getRegisteredType<T>(type_attr);     
                    if (data) { 
                        (*data).deserialize(*this);
                        rootElement_ = nextRootNode;
                    }     
                }
                else {
                    data = IvwSerializeBase::getNonRegisteredType<T>();
                    if (data) {
                        (*data).deserialize(*this);
                        rootElement_ = nextRootNode; 
                    }
                    
                }
                if (data) refDataContainer_.insert(data, rootCopy);
            }
            else {
                (*data).deserialize(*this);
                rootElement_ = nextRootNode;
            }
            
            return;
        }
   }
 

    //allocation of non-referenced data
    if (!data) {
        if (!type_attr.empty()) {
            data = IvwSerializeBase::getRegisteredType<T>(type_attr);
            if (data) {
               (*data).deserialize(*this);
                rootElement_ = nextRootNode;
            }     
        }
        else {
            data = IvwSerializeBase::getNonRegisteredType<T>();
            if (data) {
                (*data).deserialize(*this);
                rootElement_ = nextRootNode;  
            }
            
        }
        if (data) refDataContainer_.insert(data, rootCopy);
    }
    else {
        (*data).deserialize(*this);
         rootElement_ = nextRootNode;
    }    
    
    return;

}

template<class T>
inline IVW_CORE_API void IvwDeserializer::deserialize(const std::string& key, T* & data) {
    TxElement* keyNode;

    try {
        keyNode = rootElement_->FirstChildElement(key); 
    }
    catch (TxException& ) {
        return;
    } 

    std::string type_attr("");
    std::string ref_attr("");
    std::string id_attr("");
    try {
        keyNode->GetAttribute( IvwSerializeConstants::TYPE_ATTRIBUTE, &type_attr );
        if (allowRef_) {
            try {
                keyNode->GetAttribute( IvwSerializeConstants::REF_ATTRIBUTE, &ref_attr );
            }
            catch (TxException&) {
            }
            keyNode->GetAttribute( IvwSerializeConstants::ID_ATTRIBUTE, &id_attr );
        }
    }
    catch (TxException& ) {

    }

    if (!data) {
        if (!ref_attr.empty()) {
            //allocate only

            //search in reference list
            //if data doesnt exist just allocate
            // and add to list
            //return the data, donot deserialize it yet

           data=static_cast<T*>(refDataContainer_.find(type_attr, ref_attr));

            if (!data) {
                if (!type_attr.empty()) {
                    data = IvwSerializeBase::getRegisteredType<T>(type_attr);
                }
                else {
                    data = IvwSerializeBase::getNonRegisteredType<T>();
                }
                if (data) refDataContainer_.insert(data, keyNode);
            }

            return;

        }
        else if (!id_attr.empty()) {
        }
   }
}


template<class T>
inline IVW_CORE_API void IvwDeserializer::deserializePrimitives(const std::string& key, T& data) {
    TxElement* keyNode = rootElement_->FirstChildElement(key); 
    //if (!keyNode) return;
    keyNode->GetAttribute(IvwSerializeConstants::CONTENT_ATTRIBUTE, &data);
}

template<class T>
inline IVW_CORE_API void IvwDeserializer::deserializeVector(const std::string& key, T& vector, const bool& isColor) {

    TxElement* keyNode = rootElement_->FirstChildElement(key); 
    if (!keyNode) {
        return;
    }

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