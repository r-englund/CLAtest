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

#ifndef IVW_DESERIALIZER_H
#define IVW_DESERIALIZER_H

#pragma warning(disable: 4290)

#include <inviwo/core/io/serialization/ivwserializebase.h>
#include <inviwo/core/util/exception.h>

namespace inviwo {

class IvwSerializable;

class IVW_CORE_API IvwDeserializer : public  IvwSerializeBase {
public:
    /** 
     * \brief Deserializer constructor
     * 
     * @param IvwDeserializer & s optional reference to existing deserializer.
     * @param bool allowReference flag to manage references to avoid multiple object creation.     
     */
    IvwDeserializer(IvwDeserializer &s, bool allowReference=true);
    /** 
     * \brief Deserializer constructor
     *     
     * 
     * @param std::string fileName path to file that is to be deserialized.
     * @param bool allowReference flag to manage references to avoid multiple object creation.     
     */
    IvwDeserializer(std::string fileName, bool allowReference=true);
    /** 
     * \brief Destructor
     */
    virtual ~IvwDeserializer();
    /** 
     * \brief parses file on demand     
     */
    virtual void readFile() throw (SerializationException);

    /** 
     * \brief Deserialize a vector
     *
     * Deserialize the vector that has pre-allocated objects of type T or allocated by deserializer.
     * A vector is identified by key and vector items are identified by itemKey
     *
     * eg. xml tree with key=Properties and itemKey=Property
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
     * @param const std::string & key vector key.
     * @param std::vector<T> & sVector vector to be deserialized.
     * @param const std::string & itemKey vector item key     
     */
    template <typename T>
    void deserialize(const std::string &key, std::vector<T> &sVector, const std::string &itemKey) throw (SerializationException);
    /** 
     * \brief  Deserialize a map
     *
     * Deserialize a map, which can have 
     * keys of type K, 
     * values of type V* (pointers) 
     * and compare function C ( optional if 
     * K primitive type, i.e., std::string, int, etc.,)
     * eg., std::map<std::string, Property*>
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
     * key                   = "Properties"
     * itemKey               = "Property"
     * param comparisionAttribute  = "identifier"
     * param sMap["enableMIP"]     = address of a property
     *         sMap["enableShading"] = address of a property
     *         where, "enableMIP" & "enableShading" are keys.
     *         address of a property is a value
     *
     * Note: If children has attribute "type" , then comparisionAttribute becomes meaningless.
     *       Because deserializer always allocates a new instance of type using registered factories.
     *
     *       eg., <Processor type="EntryExitPoints" identifier="EntryExitPoints" reference="ref2" />
     *
     * @param const std::string & key Map key or parent node of itemKey.
     * @param std::map<K    
     * @param V * 
     * @param C> & sMap  map to be deserialized - source / input map. 
     * @param const std::string & itemKey map item key of childeren nodes.
     * @param const std::string & comparisionAttribute  - forced comparison attribute.     
     */    
    template <typename K, typename V, typename C>
    void deserialize(const std::string &key, std::map<K,V*,C> &sMap, const std::string &itemKey, const std::string &comparisionAttribute) throw (SerializationException);
    /** 
     * \brief Deserialize string data.
     * 
     * @param const std::string & key Parent node key e.g, "Property"
     * @param std::string & data string data to be deserialized
     * @param const bool asAttribute if attribute is true the xml node is formatted as <Key data="this is an attribute"\>, otherwise <Key> <data="this is non-attribute"> <Key\>     
     */
    void deserialize(const std::string &key, std::string &data, const bool asAttribute=false) throw (SerializationException);    
    /** 
     * \brief  Deserialize bool data.
     *
     * @param const std::string & key Parent node key e.g, "Property"
     * @param bool & data string bool to be deserialized     
     */
    void deserialize(const std::string &key, bool &data) throw (SerializationException);
    /** 
     * \brief  Deserialize float data.
     * 
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param float & string float to be deserialized     
     */
    void deserialize(const std::string &key, float &data) throw (SerializationException);
    /** 
     * \brief  Deserialize double data.
     * 
     * @param const std::string & key Parent node key e.g, "Property"
     * @param double & data double to be deserialized    
     */
    void deserialize(const std::string &key, double &data) throw (SerializationException);
    /** 
     * \brief  Deserialize int data.
     *           
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param int & data int data to be deserialized
     */
    void deserialize(const std::string &key, int &data) throw (SerializationException);
    /** 
     * \brief  Deserialize unsigned int data.
     *
     * @param const std::string & key Parent node key e.g, "Property"
     * @param unsigned int & data Data to be deserialized
     */
    void deserialize(const std::string &key, unsigned int &data) throw (SerializationException);
    /** 
     * \brief Deserialize long data.
     *     
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param long & data Data to be deserialized    
     */
    void deserialize(const std::string &key, long &data) throw (SerializationException);
    /** 
     * \brief  Deserialize vec2 data
     * 
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param vec2 & data Data to be deserialized
     */
    void deserialize(const std::string &key, vec2 &data) throw (SerializationException);
    /** 
     * \brief  Deserialize vec3 data.
     *
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param vec3 & data Data to be deserialized
     */
    void deserialize(const std::string &key, vec3 &data) throw (SerializationException);
    /** 
     * \brief  Deserialize vec4 data.
     *
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param vec4 & data Data to be deserialized
     */
    void deserialize(const std::string &key, vec4 &data) throw (SerializationException);
    /** 
     * \brief  Deserialize ivec2 data.
     *
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param ivec2 & data Data to be deserialized
     */
    void deserialize(const std::string &key, ivec2 &data) throw (SerializationException);
    /** 
     * \brief  Deserialize ivec3 data.
     *
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param ivec3 & data Data to be deserialized
     */
    void deserialize(const std::string &key, ivec3 &data) throw (SerializationException);
    /** 
     * \brief Deserialize ivec3 data.  
     *
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param ivec4 & data Data to be deserialized
     */
    void deserialize(const std::string &key, ivec4 &data) throw (SerializationException); 
    /** 
     * \brief  Deserialize uvec2 data.
     *
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param uvec2 & data Data to be deserialized
     */
    void deserialize(const std::string &key, uvec2 &data) throw (SerializationException);
    /** 
     * \brief  Deserialize uvec3 data.
     *
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param uvec3 & data Data to be deserialized
     */
    void deserialize(const std::string &key, uvec3 &data) throw (SerializationException);
    /** 
     * \brief  Deserialize uvec4 data.
     * 
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param uvec4 & data Data to be deserialized
     */
    void deserialize(const std::string &key, uvec4 &data) throw (SerializationException); 
    /** 
     * \brief  Deserialize dvec2 data.
     * 
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param dvec2 & data Data to be deserialized
     */
    void deserialize(const std::string &key, dvec2 &data) throw (SerializationException);
    /** 
     * \brief  Deserialize dvec3 data.
     * 
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param dvec3 & data Data to be deserialized
     */
    void deserialize(const std::string &key, dvec3 &data) throw (SerializationException);
    /** 
     * \brief  Deserialize dvec4 data.
     * 
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param dvec4 & data Data to be deserialized
     */
    void deserialize(const std::string &key, dvec4 &data) throw (SerializationException); 
    /** 
     * \brief  Deserialize mat2 data.
     * 
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param mat2 & data Data to be deserialized
     */
    void deserialize(const std::string &key, mat2 &data) throw (SerializationException);
    /** 
     * \brief  Deserialize mat3 data.
     *
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param mat3 & data Data to be deserialized
     */
    void deserialize(const std::string &key, mat3 &data) throw (SerializationException);
    /** 
     * \brief  Deserialize mat4 data.
     *
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param mat4 & data Data to be deserialized
     */
    void deserialize(const std::string &key, mat4 &data) throw (SerializationException); 
    /** 
     * \brief  Deserialize any serializable object
     * 
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param IvwSerializable & sObj object to be deserialzied
     */
    void deserialize(const std::string &key, IvwSerializable &sObj) throw (SerializationException);
    /** 
     * \brief  Deserialize pointer data of type T, which is of type serializeble object or primitive data
     * 
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param T *  & data pointer to be deserialized which is of type T which is usally serializable object or primitive data
     */
    template <class T>
    void deserialize(const std::string& key, T* & data) throw (SerializationException);
    
protected:
    friend class NodeSwitch;
private:
    //void deserialize(const std::string& key, IvwSerializable* & data);    
    /** 
     * \brief Deserialize a stl vector
     *
     * Deserialize the vector that has pre-allocated objects of type T or allocated by deserializer.
     * A vector is identified by key and vector items are identified by itemKey
     * 
     * @param const std::string & key vector key.
     * @param std::vector<T * > & sVector vector to be deserialized.
     * @param const std::string & itemKey vector item key     
     */
    template <typename T>
    void deserializeSTL_Vector(const std::string &key, std::vector<T*> &sVector, const std::string &itemKey) throw (SerializationException);
    /** 
     * \brief Deserialize a stl vector
     *
     * Deserialize the vector that has pre-allocated objects of type T or added to the stl vectory by deserializer.
     * A vector is identified by key and vector items are identified by itemKey
     * 
     * @param const std::string & key vector key.
     * @param std::vector<T > & sVector vector to be deserialized.
     * @param const std::string & itemKey vector item key     
     */
    template <typename T>
    void deserializeSTL_Vector(const std::string &key, std::vector<T> &sVector, const std::string &itemKey) throw (SerializationException);

    /** 
     * \brief Deserialize a map, which can have keys of type K, values of type V* (pointers) and an optional compare function C. eg., std::map<std::string, Property*>
     *
     * @param const std::string & key Parent node key e.g, "Property"
     * @param T & sMap map source / input map
     * @param const std::string & itemKey item key of childeren nodes.
     * @param const std::string & comparisionAttribute - forced comparison attribute.
     */
    template <typename T>
    void deserializeSTL_Map(const std::string &key, T &sMap, const std::string &itemKey, const std::string &comparisionAttribute) throw (SerializationException);

    /** 
     * \brief Deserialize primitive string data type which is not an attribute that is formatted as <Key> <data="this is non-attribute"> <Key\>     
     * 
     * @param const std::string & key  Parent node key e.g, "Property"
     * @param std::string & data Data to be deserialized
     */
    void deserializePrimitives(const std::string &key, std::string &data) throw (SerializationException);

    /** 
     * \brief Deserialize primitive data type which string data which is an attribute that is formatted as <Key data="this is an attribute"\>     
     *
     * @param const std::string & key Parent node key e.g, "Property"
     * @param std::string & data Data to be deserialized
     */
    void deserializeAttributes(const std::string &key, std::string &data) throw (SerializationException);
    
    /** 
     * \brief Deserialize primitive data type such as int, long, float, etc., (except string data) which is not an attribute that is formatted as <Key> <stepValue=1.0> <Key\>
     *
     * @param const std::string & key Parent node key e.g, "Property"
     * @param T & data Data to be deserialized of type int, long, float, etc., (except string)
     */
    template <typename T>
    void deserializePrimitives(const std::string& key, T& data) throw (SerializationException);
    
    /** 
     * \brief Deserialize vector data sturcture vec2, ive2, vec3, ivec3, etc.,
     *
     * @param const std::string & key Parent node key e.g, "Property"
     * @param T & vector Glm data structures such as vec2, ive2, vec3, ivec3, etc.,
     * @param const bool & isColor If (isColor==true) x="0" y="0" z="0" , If (isColor==false) isColor r="0" g="0" b="0"
     */
    template <class T>
    void deserializeVector(const std::string& key, T& vector, const bool& isColor=false) throw (SerializationException);

    /** 
     * \brief Derserialize pointer
     *
     * @param const std::string & key Parent node key e.g, "Property"
     * @param T *  & data If data==NULL then deserializer tried to allocated data, otherwise pre-allocated data should be passed.
     */
    template <class T>
    void deserializePointer(const std::string& key, T* & data) throw (SerializationException);

};

template <typename T>
inline void IvwDeserializer::deserialize(const std::string &key, std::vector<T> &sVector, const std::string &itemKey) 
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

    try {    
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
    }catch (TxException&) {
        //Proceed to next node safely if exception encountered
    }

    sVector = tVector;
}

template <typename T>
inline void IvwDeserializer::deserializeSTL_Vector(const std::string &key, std::vector<T> &sVector, const std::string &itemKey)
throw (SerializationException) {

    TxElement* keyNode;


    try {
        keyNode = rootElement_->FirstChildElement(key);
        keyNode->FirstChildElement();
    } catch (TxException&) {
        return;
    }

    NodeSwitch tempNodeSwitch(*this, keyNode);    
    
    T item;
    std::vector<T> tVector;

    try {
        //space holder
        if (sVector.size()==0) {
            for (TxEIt child(keyNode->FirstChildElement(itemKey), itemKey); child != child.end(); ++child) {
                T emptyItem;
                tVector.push_back(emptyItem);
            }
        }
        sVector = tVector;

        unsigned int i=0;
        for (TxEIt child(keyNode->FirstChildElement(itemKey), itemKey); child != child.end(); ++child) {
            rootElement_ = &(*child);
            deserialize(itemKey, sVector[i++]);
        }
    }catch (TxException&) {
        //Proceed to next node safely if exception encountered
        sVector.clear();
    }
     
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
                    typename T::key_type keyTypeAttr;
                    rootElement->GetAttribute(comparisionAttribute, &keyTypeAttr);
                    //keyCompare returns false if first argument is less than second.
                    if ( (!keyCompare(keyTypeAttr, it->first)) && (!keyCompare(it->first, keyTypeAttr)) ) {
                        //if required attribute exists then deserialize it
                        nextRootNode = rootElement->FirstChildElement();
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

           //Do not allocate new data
           //if (!data) {
           //   if (!type_attr.empty()) {
           //        data = IvwSerializeBase::getRegisteredType<T>(type_attr);
           //   }
           //   else {
           //       data = IvwSerializeBase::getNonRegisteredType<T>();
           //     }
           //     if (data) refDataContainer_.insert(data, keyNode);
           //}

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

    TxElement* keyNode = rootElement_->FirstChildElement(key, false); 
    if (!keyNode) {        
        //Try to finding key in the current node before exit. If not, let the exception be thrown.
        try {
            T tempVec;
            rootElement_->GetAttribute(
                isColor ? IvwSerializeConstants::COLOR_R_ATTRIBUTE : IvwSerializeConstants::VECTOR_X_ATTRIBUTE, &tempVec[0]);
            keyNode = rootElement_;
        }
        catch (TxException& ) {
            return;
        }        
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