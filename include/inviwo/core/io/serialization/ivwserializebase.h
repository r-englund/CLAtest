#ifndef IVW_SERIALIZE_BASE_H
#define IVW_SERIALIZE_BASE_H

#ifndef TIXML_USE_TICPP
#  define TIXML_USE_TICPP
#endif

#include <ticpp/ticpp.h>
#include <inviwo/core/io/serialization/ivwserializeconstants.h>
#include <inviwo/core/util/inviwofactorybase.h>
#include <map>

// include glm
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/core/type.hpp>
#include <glm/gtc/type_ptr.hpp>

typedef glm::ivec2 ivec2;
typedef glm::ivec3 ivec3;
typedef glm::ivec4 ivec4;
typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::dvec2 dvec2;
typedef glm::dvec3 dvec3;
typedef glm::dvec4 dvec4;
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

class IvwSerializable;

class IvwSerializeBase {
public:
    /** 
     * \brief Base class for IvwSerializer and IvwDeserializer.
     *
     * This class consists of features that are common to both serializer
     * and de-serializer. Some of them are reference data manager,
     * (ticpp::Node) node switch and factory registration.
     * 
     * @param IvwSerializeBase & s object of similar type.
     * @param bool allowReference disables or enables reference management schemes.
     */
    IvwSerializeBase(IvwSerializeBase &s, bool allowReference=true);
    /** 
     * \brief Base class for IvwSerializer and IvwDeserializer.
     *
     * This class consists of features that are common to both serializer
     * and de-serializer. Some of them are reference data manager,
     * (ticpp::Node) node switch and factory registration.
     * 
     * @param std::string fileName full path to xml file (for reading or writing).
     * @param bool allowReference disables or enables reference management schemes.
     */
    IvwSerializeBase(std::string fileName, bool allowReference=true);
    /** 
     * \brief Destructor    
     */
    virtual ~IvwSerializeBase();
    /** 
     * \brief gets the xml file name.
     */
    virtual std::string getFileName();    
    /** 
     * \brief Checks whether the given type is a primitive type.
     *
     * return true if type is one of following type:
     * bool, char, signed int, unsigned int, float, double, long double, std::string
     * 
     * @param const std::type_info & type can be one of  bool, char, signed int, unsigned int, float, double, long double, std::string
     * @return bool true or false
     */
    bool isPrimitiveType(const std::type_info& type) const;
    /** 
     * \brief Checks whether the given type is a primitive pointer type.
     *
     * return true if type is one of following type:
     * bool*, char*, signed int*, unsigned int*, float*, double*, long double*, std::string*
     * 
     * @param const std::type_info & type can be one of  bool, char, signed int, unsigned int, float, double, long double, std::string.
     * @return bool true or false.
     */
    bool isPrimitivePointerType(const std::type_info& type) const;
    /** 
     * \brief Enable or disable reference flag.
     */
    void setAllowReference(const bool &allowReference);
    /** 
     * \brief Registers all factories from all modules.     
     */
    virtual void registerFactories(void);

    /** 
     * \brief For allocating objects such as processors, properties.. using registered factories.
     *
     * @param const std::string & className is used by registered factories to allocate the required object.
     * @return T* NULL if allocation fails or className does not exist in any factories.
     */
    template <typename T>
    T* getRegisteredType(const std::string &className);

    /** 
     * \brief For allocating objects that do not belong to any registered factories.          
     * 
     * @return T* Pointer to object of type T.
     */
    template <typename T>
    T* getNonRegisteredType();
    
    virtual void setFileName(const std::string fileName);

    class NodeSwitch {
    public:
         NodeSwitch(IvwSerializeBase& serializer, TxElement* node);
        ~NodeSwitch();

    private:  
        IvwSerializeBase& _serializer;
        TxElement* _storedNode;
    };

    struct ReferenceData {            
        TxElement* _node;
        bool _isPointer;
    };

    typedef std::pair<const void *, IvwSerializeBase::ReferenceData> RefDataPair;
    typedef std::multimap<const void*,ReferenceData> RefMap;
    typedef std::vector<IvwSerializeBase::ReferenceData> RefDataList;

    class ReferenceDataContainer {
    public:
         ReferenceDataContainer();
        ~ReferenceDataContainer();
        size_t insert(const void *data, TxElement *node, bool isPointer=true);
        size_t find(const void *data);
        void* find(const std::string& key, const std::string& reference_or_id);
        std::vector<ReferenceData> getNodes(const void *data);
        TxElement* nodeCopy(const void *data);
        void setReferenceAttributes();

    private:  
        RefMap _allReferenceMap;
        int _refCount;
    };

    
    
protected:
    friend class NodeSwitch;

    //TODO: These are static factory objects. But still storing them in vectors can be useful??? 
    std::vector<Factory*> registeredFactories_;
    std::string fileName_;
    TxDocument doc_;
    TxElement* rootElement_;
    bool allowRef_;
    ReferenceDataContainer refDataContainer_;
};


template <typename T>
T* IvwSerializeBase::getRegisteredType(const std::string &className) {
    T* data = 0;
    std::vector<Factory *>::iterator it;
    for (it = registeredFactories_.begin(); it!=registeredFactories_.end(); it++) {
        data = dynamic_cast<T*>( (*it)->create(className) );
        if (data)
            break;
    }

    return data;
}

template <typename T>
inline T* IvwSerializeBase::getNonRegisteredType() {   
    return new T();
}

} //namespace
#endif