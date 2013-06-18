#ifndef IVW_DESERIALIZER_H
#define IVW_DESERIALIZER_H

#ifndef TIXML_USE_TICPP
#  define TIXML_USE_TICPP
#endif

#include <inviwo/core/common/inviwocoredefine.h>
#include <ticpp/ticpp.h>
#include <inviwo/core/io/serialization/ivwserializeconstants.h>
#include <inviwo/core/io/serialization/ivwserializebase.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/util/exception.h>

namespace inviwo {

class IvwSerializable;

class IVW_CORE_API IvwDeserializer : public  IvwSerializeBase {
public:
    IvwDeserializer(IvwDeserializer &s, bool allowReference=true);
    IvwDeserializer(std::string fileName, bool allowReference=true);
    virtual ~IvwDeserializer();
    virtual void readFile() throw (SerializationException);

    template <typename T>
    void deserialize(const std::string &key, std::vector<T*> &sVector, const std::string &itemKey) throw (SerializationException);

    /* 
     * Deserialize a map, which can have 
     * keys of type K, 
     * values of type V* (pointers) 
     * and compare function C ( optional if 
     * K primitive type, i.e., std::string, int, etc.,)
     * eg., std::map<std::string, Property*>
     * 
     * @param key - parent node of itemKey.
     * @param sMap - source / input map.
     * @param itemKey - children nodes.
     * @param comparisionAttribute - forced comparison attribute.
     *
     * eg. xml tree
     *
     * <Properties>
     *      <Property identifier="enableMIP" displayName="MIP">
     *          <value content="0" />
     *      </Property>
     *      <Property identifier="enableShading" displayName="Shading">
     *          <value content="0" />
     *      </Property>
     * <Properties>
     *
     * In the above xml tree,
     *
     * @ param key                   = "Properties"
     * @ param itemKey               = "Property"
     * @ param comparisionAttribute  = "identifier"
     * @ param sMap["enableMIP"]     = address of a property
     *         sMap["enableShading"] = address of a property
     *         where, "enableMIP" & "enableShading" are keys.
     *         address of a property is a value
     *
     * Note: If children has attribute "type" , then comparisionAttribute becomes meaningless.
     *       Because deserializer always allocates a new instance of type using registered factories.
     *
     *       eg., <Processor type="EntryExitPoints" identifier="EntryExitPoints" reference="ref2" />
     */
    template <typename K, typename V, typename C>
    void deserialize(const std::string &key, std::map<K,V*,C> &sMap, const std::string &itemKey, const std::string &comparisionAttribute) throw (SerializationException);

    void deserialize(const std::string &key, std::string &data, const bool asAttribute=false) throw (SerializationException);    
    void deserialize(const std::string &key, bool &data) throw (SerializationException);
    void deserialize(const std::string &key, float &data) throw (SerializationException);
    void deserialize(const std::string &key, double &data) throw (SerializationException);
    void deserialize(const std::string &key, int &data) throw (SerializationException);
    void deserialize(const std::string &key, unsigned int &data) throw (SerializationException);
    void deserialize(const std::string &key, long &data) throw (SerializationException);
    void deserialize(const std::string &key, vec2 &data) throw (SerializationException);
    void deserialize(const std::string &key, vec3 &data) throw (SerializationException);
    void deserialize(const std::string &key, vec4 &data) throw (SerializationException);
    void deserialize(const std::string &key, ivec2 &data) throw (SerializationException);
    void deserialize(const std::string &key, ivec3 &data) throw (SerializationException);
    void deserialize(const std::string &key, ivec4 &data) throw (SerializationException); 
    void deserialize(const std::string &key, uvec2 &data) throw (SerializationException);
    void deserialize(const std::string &key, uvec3 &data) throw (SerializationException);
    void deserialize(const std::string &key, uvec4 &data) throw (SerializationException); 
    void deserialize(const std::string &key, dvec2 &data) throw (SerializationException);
    void deserialize(const std::string &key, dvec3 &data) throw (SerializationException);
    void deserialize(const std::string &key, dvec4 &data) throw (SerializationException); 
    void deserialize(const std::string &key, mat2 &data) throw (SerializationException);
    void deserialize(const std::string &key, mat3 &data) throw (SerializationException);
    void deserialize(const std::string &key, mat4 &data) throw (SerializationException); 
    void deserialize(const std::string &key, IvwSerializable &sObj) throw (SerializationException);
    template <class T>
    void deserialize(const std::string& key, T* & data) throw (SerializationException);
    
protected:
    friend class NodeSwitch;
private:
    //void deserialize(const std::string& key, IvwSerializable* & data);
    template <typename T>
    void deserializeSTL_Vector(const std::string &key, std::vector<T*> &sVector, const std::string &itemKey) throw (SerializationException);

    /* 
     * Deserialize a map, which can have keys of type K, values of type V* (pointers) 
     * and an optional compare function C.
     * eg., std::map<std::string, Property*>
     *
     * @ param refer void deserialize()
     */
    template <typename T>
    void deserializeSTL_Map(const std::string &key, T &sMap, const std::string &itemKey, const std::string &comparisionAttribute) throw (SerializationException);

    void deserializePrimitives(const std::string &key, std::string &data) throw (SerializationException);

    void deserializeAttributes(const std::string &key, std::string &data) throw (SerializationException);
    
    template <typename T>
    void deserializePrimitives(const std::string& key, T& data) throw (SerializationException);
    
    template <class T>
    void deserializeVector(const std::string& key, T& vector, const bool& isColor=false) throw (SerializationException);

    template <class T>
    void deserializePointer(const std::string& key, T* & data) throw (SerializationException);

};

template <typename T>
inline void IvwDeserializer::deserialize(const std::string &key, std::vector<T*> &sVector, const std::string &itemKey) 
        throw (SerializationException){
    deserializeSTL_Vector(key, sVector, itemKey);
}

template <typename K, typename V, typename C>
inline void IvwDeserializer::deserialize(const std::string &key,std::map<K,V*,C> &sMap, const std::string &itemKey, const std::string &comparisionAttribute)
        throw (SerializationException){
    deserializeSTL_Map(key, sMap, itemKey, comparisionAttribute);
}

template <typename T>
inline void IvwDeserializer::deserializeSTL_Vector(const std::string &key, std::vector<T*> &sVector, const std::string &itemKey)
        throw (SerializationException) {
    
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
            deserializePointer(itemKey, sVector[i]);
            tVector.push_back(sVector[i]);
            i++;
        }        
    }

    sVector = tVector;
}

template <typename T>
inline void IvwDeserializer::deserializeSTL_Map(const std::string &key, 
                                                T &sMap, 
                                                const std::string &itemKey, 
                                                const std::string &comparisionAttribute) 
        throw (SerializationException) {

    TxElement* keyNode;


    try {
        keyNode = rootElement_->FirstChildElement(key);
        keyNode->FirstChildElement();
    } catch (TxException&) {
        return;
    }

    if (comparisionAttribute == "")
        return;

    NodeSwitch tempNodeSwitch(*this, keyNode);

    TxElement* nextRootNode;
    TxElement* rootElement;

    
    typename T::key_compare keyCompare = sMap.key_comp();
    
    if ( isPrimitiveType(typeid(typename T::key_type)) ) {
        //always use primitive types as keys
        
        for (typename T::iterator it = sMap.begin(); it != sMap.end(); ++it) {            
            //Probe if there is any children with attribute 'comparisionAttribute'
            for (TxEIt child(keyNode->FirstChildElement(itemKey), itemKey); child != child.end(); ++child) {                
                rootElement = &(*child);            
                try { 
                    nextRootNode = rootElement->FirstChildElement();
                    typename T::key_type keyTypeAttr;
                    rootElement->GetAttribute(comparisionAttribute, &keyTypeAttr);
                    //keyCompare returns false if first argument is less than second.
                    if ( (!keyCompare(keyTypeAttr, it->first)) && (!keyCompare(it->first, keyTypeAttr)) ) {
                        //if required attribute exists then deserialize it
                        rootElement_ = &(*child);
                        deserializePointer(itemKey, it->second);
                        break;
                    }
                }
                catch (TxException& ) {
                    rootElement = 0;
                    nextRootNode = 0;
                    break;
                }
            }
        }
    }
    else {
        //TODO: support maps with non-primitive key types if necessary
    }
}

template<class T>
inline void IvwDeserializer::deserializePointer(const std::string& /*key*/, T* & data) throw (SerializationException) {
    
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
         refDataContainer_.insert(data, rootCopy);
         (*data).deserialize(*this);
         rootElement_ = nextRootNode;
    }    
    
    return;

}

template<class T>
inline void IvwDeserializer::deserialize(const std::string& key, T* & data) throw (SerializationException) {
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
inline void IvwDeserializer::deserializePrimitives(const std::string& key, T& data) throw (SerializationException) {
    TxElement* keyNode = rootElement_->FirstChildElement(key); 
    //if (!keyNode) return;
    keyNode->GetAttribute(IvwSerializeConstants::CONTENT_ATTRIBUTE, &data);
}

template<class T>
inline void IvwDeserializer::deserializeVector(const std::string& key, T& vector, const bool& isColor) throw (SerializationException) {

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