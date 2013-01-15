#ifndef IVW_SERIALIZE_BASE_H
#define IVW_SERIALIZE_BASE_H

#ifndef TIXML_USE_TICPP
#  define TIXML_USE_TICPP
#endif

#include "ext/ticpp/ticpp.h"
#include "inviwo/core/io/serialization/ivwserializeconstants.h"
#include "inviwo/core/inviwofactorybase.h"
#include <map>

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
        IvwSerializeBase(IvwSerializeBase &s, bool allowReference=true);
        IvwSerializeBase(std::string fileName, bool allowReference=true);
        virtual ~IvwSerializeBase();
        virtual std::string getFileName();
        
        bool isPrimitiveType(const std::type_info& type) const;
        bool isPrimitivePointerType(const std::type_info& type) const;

        void setAllowReference(const bool &allowReference);

        virtual void registerFactories(void);

        template <typename T>
        T* getRegisteredType(const std::string &className);

        template <typename T>
        T* getNonRegisteredType();

        template <typename T>
        T* allocateMemory(std::string className);    

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
            int insert(const void *data, TxElement *node, bool isPointer=true);
            int find(const void *data);
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
        std::vector<InviwoFactoryBase*> registeredFactories_;
        std::string fileName_;
        TxDocument doc_;
        TxElement* rootElement_;
        bool allowRef_;
        ReferenceDataContainer refDataContainer_;
    };


    template <typename T>
    T* IvwSerializeBase::getRegisteredType(const std::string &className) {
        T* data = 0;
        std::vector<InviwoFactoryBase *>::iterator it;
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