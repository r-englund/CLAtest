#ifndef IVW_SERIALIZE_BASE_H
#define IVW_SERIALIZE_BASE_H

#ifndef TIXML_USE_TICPP
#  define TIXML_USE_TICPP
#endif

#include "ticpp/ticpp.h"
#include "inviwo/core/io/serialization/ivwserializeconstants.h"

// include glm
#include "ext/glm/glm.hpp"
#include "ext/glm/gtx/transform.hpp"
#include "ext/glm/gtc/quaternion.hpp"
#include "ext/glm/gtx/quaternion.hpp"
#include "ext/glm/core/type.hpp"
#include "ext/glm/gtc/type_ptr.hpp"

typedef glm::ivec2 ivec2;
typedef glm::ivec3 ivec3;
typedef glm::ivec4 ivec4;
typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::bvec2 bvec2;
typedef glm::bvec3 bvec3;
typedef glm::bvec3 bvec4;
typedef glm::mat2 mat2;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef glm::quat quat;

namespace inviwo {

    typedef ticpp::Document TxDocument;
    typedef ticpp::Element TxElement;
    typedef ticpp::Node TxNode;
    typedef ticpp::Exception TxException;
    typedef ticpp::Declaration TxDeclaration;
    typedef ticpp::Comment TxComment;
    typedef ticpp::Attribute TxAttribute;
    typedef ticpp::Iterator<TxElement> TxEIt;
    typedef ticpp::Iterator<TxAttribute> TxAIt;


class InviwoFactoryBase;
class IvwSerializable;


class IvwSerializeBase {
public:
    IvwSerializeBase(IvwSerializeBase &s, bool allowReference=false);
    IvwSerializeBase(std::string fileName, bool allowReference=false);
    virtual ~IvwSerializeBase();
    virtual std::string getFileName();
    //virtual void readFile(std::ostream& stream);
    //virtual void writeFile(std::ostream& stream);
    
    bool isPrimitiveType(const std::type_info& type) const;
    bool isPrimitivePointerType(const std::type_info& type) const;

    void setAllowReference(const bool &allowReference);

    virtual void registerFactories(void) ;

    template <typename T>
    T* getRegisteredType(const std::string &className);

    //IvwSerializable* getRegisteredType(const std::string &className);

    template <typename T>
    T* getNonRegisteredType();

    template <typename T>
    T* allocateMemory(std::string className);    
    virtual void setFileName(const std::string fileName);

    /*template <typename T>
    void serialize(const std::string &key, const std::vector<T> &sVector, const std::string &itemKey);

    void serialize(const std::string &key, const IvwSerializable &sObj);

    void serialize(const std::string &key, const std::string &data);

    void serialize(const std::string &key, const vec2 &data);
    void serialize(const std::string &key, const vec3 &data);
    void serialize(const std::string &key, const vec4 &data);
    void serialize(const std::string &key, const ivec2 &data);
    void serialize(const std::string &key, const ivec3 &data);
    void serialize(const std::string &key, const ivec4 &data);

    template<class T>
    void serialize(const std::string& key, const T* const& data);

    template <typename T>
    void serializeSTL_Vector(const std::string &key, const T &sVector, const std::string &itemKey);

    void serializePrimitives(const std::string &key, const std::string &data);

    template<typename T>
    void serializePrimitives(const std::string& key, const T& data);

    template<class T>
    void serializeVector(const std::string& key, const T& vector, const bool& isColor=false);
    */

    class NodeSwitch {
    public:
         NodeSwitch(IvwSerializeBase& serializer, TxElement* node);
        ~NodeSwitch();

    private:  
        IvwSerializeBase& _serializer;
        TxElement* _storedNode;
    };
    
protected:
    friend class NodeSwitch;

    //TODO: These are static factory objects. But still storing them in vectors can be useful??? 
    std::vector<InviwoFactoryBase *> _registeredFactoryVector;
    std::string _fileName;
    TxDocument _doc;
    TxElement*_root;
    bool _allowReference;
    
};
/*
template <typename T>
inline T* IvwSerializeBase::getRegisteredType(const std::string &className)
{
    T* data = 0;
    std::vector<InviwoFactoryBase *>::iterator it;
    for(it = _registeredFactoryVector.begin(); it!=_registeredFactoryVector.end(); it++)
    {
        data = dynamic_cast<T*>( (*it)->create(className) ) ;
        if(data)
            break;
    }   

    return data;
}*/

template <typename T>
T* IvwSerializeBase::getRegisteredType(const std::string &className)
{
    T* data = 0;
    std::vector<InviwoFactoryBase *>::iterator it;
    for(it = _registeredFactoryVector.begin(); it!=_registeredFactoryVector.end(); it++)
    {
        data = dynamic_cast<T*>( (*it)->create(className) ) ;
        if(data)
            break;
    }

    return data;
}

template <typename T>
inline T* IvwSerializeBase::getNonRegisteredType()
{   
    return new T();
}

/*
template <typename T>
inline void IvwSerializeBase::serialize(const std::string &key, const std::vector<T> &sVector, const std::string &itemKey) {
    serializeSTL_Vector(key, sVector, itemKey);
}

template <typename T>
inline void IvwSerializeBase::serializeSTL_Vector(const std::string &key, const T &sVector, const std::string &itemKey) {
    TxElement* newNode = new TxElement(key);
    _root->LinkEndChild(newNode);

    NodeSwitch tempNodeSwitch(*this, newNode);

    for (typename T::const_iterator it = sVector.begin(); it != sVector.end(); ++it) {
        if(!isPrimitiveType(typeid(typename T::value_type)) && !isPrimitivePointerType(typeid(typename T::value_type)) ) {
            serialize(itemKey, (*it)) ;
        }
    }
}

template<class T>
inline void IvwSerializeBase::serialize(const std::string& key, const T* const& data) {
    
    if(!_allowReference)
    {
        //_allowReference = true;
        serialize(key, *data) ;
        return;
    }

    TxElement* newNode = new TxElement(key);

    if (data) {
       
        //std::string typeString = getTypeString(typeid(*data));
        //if (!typeString.empty())
        //newNode->SetAttribute(IvwSerializeConstants::TYPE_ATTRIBUTE, typeid(*data).name());
            
    }

    _root->LinkEndChild(newNode);
}

template<class T>
inline void IvwSerializeBase::serializePrimitives(const std::string& key, const T& data)
{
    
    TxElement* newNode = new TxElement(key);
    _root->LinkEndChild(newNode);
    newNode->SetAttribute(IvwSerializeConstants::VALUE_ATTRIBUTE, ss.str());
}

template<class T>
inline void IvwSerializeBase::serializeVector(const std::string& key, const T& vector, const bool& isColor)
{
    TxElement* newNode = new TxElement(key);
    _root->LinkEndChild(newNode);

    std::stringstream ss;
    ss.precision(IvwSerializeConstants::STRINGSTREAM_PRECISION);
    ss<<(isColor ? static_cast<int>(vector[0]) : vector[0]);

    newNode->SetAttribute(
        isColor ? IvwSerializeConstants::COLOR_R_ATTRIBUTE : IvwSerializeConstants::VECTOR_X_ATTRIBUTE, ss.str());

    if (vector.length() >= 2) {
        ss.str(std::string());
        ss<<(isColor ? static_cast<int>(vector[1]) : vector[1]);
        newNode->SetAttribute(
            isColor ? IvwSerializeConstants::COLOR_G_ATTRIBUTE : IvwSerializeConstants::VECTOR_Y_ATTRIBUTE,ss.str());
    }

    if (vector.length() >= 3) {
        ss.str(std::string());
        ss<<(isColor ? static_cast<int>(vector[2]) : vector[2]);
        newNode->SetAttribute(
            isColor ? IvwSerializeConstants::COLOR_B_ATTRIBUTE : IvwSerializeConstants::VECTOR_Z_ATTRIBUTE,ss.str());
    }

    if (vector.length() >= 4) {
        ss.str(std::string());
        ss<<(isColor ? static_cast<int>(vector[2]) : vector[2]);
        newNode->SetAttribute(
            isColor ? IvwSerializeConstants::COLOR_A_ATTRIBUTE : IvwSerializeConstants::VECTOR_W_ATTRIBUTE,ss.str());
    }
}*/

} //namespace
#endif