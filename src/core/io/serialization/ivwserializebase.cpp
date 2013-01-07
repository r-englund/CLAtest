#include "inviwo/core/io/serialization/ivwserializebase.h"
#include "inviwo/core/inviwofactorybase.h"
#include "inviwo/core/processors/processorfactory.h"
#include "inviwo/core/metadata/metadatafactory.h"
#include "inviwo/core/io/serialization/ivwserializable.h"
#include "inviwo/core/inviwo.h"


namespace inviwo {

IvwSerializeBase::NodeSwitch::NodeSwitch(IvwSerializeBase& serializer, TxElement* node)
    : _serializer(serializer)
    , _storedNode(_serializer.rootElement_)
{
    _serializer.rootElement_ = node;
}

IvwSerializeBase::NodeSwitch::~NodeSwitch() {
    _serializer.rootElement_ = _storedNode;
}

IvwSerializeBase::ReferenceDataContainer::ReferenceDataContainer() {
    _refCount = 0;
}

IvwSerializeBase::ReferenceDataContainer::~ReferenceDataContainer() {
}

int IvwSerializeBase::ReferenceDataContainer::insert(const void *data, TxElement *node, bool isPointer) {
 
    IvwSerializeBase::ReferenceData refData;

    refData._node = node;
    refData._isPointer = isPointer;

    _allReferenceMap.insert(RefDataPair(data, refData));

    int count = _allReferenceMap.count(data);

    return count;
}

void IvwSerializeBase::ReferenceDataContainer::setReferenceAttributes() {

    std::map<const void*, int > keyMap;
    std::map<const void*, int >::iterator keyMapIt;

    for (RefMap::iterator it = _allReferenceMap.begin(); it != _allReferenceMap.end(); ++it) {
        keyMap[(*it).first]++;
    }

    std::stringstream ss;

    for ( keyMapIt=keyMap.begin(); keyMapIt != keyMap.end(); keyMapIt++ ) {

        std::vector<ReferenceData> nodes;
        std::vector<ReferenceData>::iterator nodeIt;

        nodes = getNodes((*keyMapIt).first);

        if (nodes.size()<=1) continue;

         ss.str("");
         ss<<"ref";
         ss<<_refCount;

        for ( nodeIt=nodes.begin(); nodeIt != nodes.end(); nodeIt++ ) {
              

            if ((*nodeIt)._isPointer) { 
                (*nodeIt)._node->SetAttribute(IvwSerializeConstants::REF_ATTRIBUTE, ss.str() );
            }
            else {
                (*nodeIt)._node->SetAttribute(IvwSerializeConstants::ID_ATTRIBUTE, ss.str() );             
            }
        }

        _refCount++;
    }
    
}

int IvwSerializeBase::ReferenceDataContainer::find(const void *data) {
    return _allReferenceMap.count(data);
}

void* IvwSerializeBase::ReferenceDataContainer::find(const std::string& type, const std::string& reference_or_id) {
    void* data = 0;

    std::map<const void*, int > keyMap;
    std::map<const void*, int >::iterator keyMapIt;

    for (RefMap::iterator it = _allReferenceMap.begin(); it != _allReferenceMap.end(); ++it) {
        keyMap[(*it).first]++;
    }

    for ( keyMapIt=keyMap.begin(); keyMapIt != keyMap.end(); keyMapIt++ ) {

        std::vector<ReferenceData> nodes;
        std::vector<ReferenceData>::iterator nodeIt;

        nodes = getNodes((*keyMapIt).first);

        std::string type_attrib("");
        std::string ref_attrib("");
        std::string id_attrib("");

        for ( nodeIt=nodes.begin(); nodeIt != nodes.end(); nodeIt++ ) {
            try {
                (*nodeIt)._node->GetAttribute(IvwSerializeConstants::TYPE_ATTRIBUTE, &type_attrib);
                try {
                    (*nodeIt)._node->GetAttribute( IvwSerializeConstants::REF_ATTRIBUTE, &ref_attrib );
                }
                catch (TxException& ) {
                }
                (*nodeIt)._node->GetAttribute( IvwSerializeConstants::ID_ATTRIBUTE, &id_attrib );
            }
            catch (TxException& ) {
            }

            if (type_attrib == type) {
                if (!reference_or_id.empty()) {
                    if (ref_attrib == reference_or_id || id_attrib == reference_or_id ) {
                        data = const_cast<void *>((*keyMapIt).first);
                        break;
                    }
                }
                else {
                    data = const_cast<void *>((*keyMapIt).first);
                    break;
                }
            }

        }

        if (data) break;
    }

    return data;
}

std::vector<IvwSerializeBase::ReferenceData> IvwSerializeBase::ReferenceDataContainer::getNodes(const void *data) {
    std::pair<RefMap::iterator, RefMap::iterator> pIt;
    std::vector<ReferenceData> nodes;

    pIt = _allReferenceMap.equal_range(data);

    for (RefMap::iterator mIt = pIt.first; mIt != pIt.second; ++mIt) {
        nodes.push_back((*mIt).second);
    }

    return nodes;
}

 TxElement* IvwSerializeBase::ReferenceDataContainer::nodeCopy(const void *data) {
    std::pair<RefMap::iterator, RefMap::iterator> pIt;
    std::vector<ReferenceData> nodes;

    TxElement* nodeCopy=0;

    pIt = _allReferenceMap.equal_range(data);

    for (RefMap::iterator mIt = pIt.first; mIt != pIt.second; ++mIt) {
        if (nodeCopy = (*mIt).second._node->Clone()->ToElement()) {
            nodeCopy->Clear();
            break;
        }
    }

    return nodeCopy;
}

IvwSerializeBase::IvwSerializeBase(IvwSerializeBase &s, bool allowReference)
    : fileName_(s.fileName_)
    , doc_(s.fileName_)
    , allowRef_(allowReference)
{
     registerFactories();  
}

IvwSerializeBase::IvwSerializeBase(std::string fileName, bool allowReference)
    : fileName_(fileName)
    , doc_(fileName)
    , allowRef_(allowReference)
{
    registerFactories(); 
}

IvwSerializeBase::~IvwSerializeBase() {
}

void IvwSerializeBase::registerFactories(void) {
    registeredFactories_.clear();
    registeredFactories_.push_back(InviwoFactoryBase::instance<ProcessorFactory>());
    registeredFactories_.push_back(InviwoFactoryBase::instance<MetaDataFactory>());
}

template <typename T>
T* IvwSerializeBase::allocateMemory(std::string className) {
    T* data = 0;
    return data;
}

std::string IvwSerializeBase::getFileName() {
    return fileName_;
}

void IvwSerializeBase::setFileName(const std::string fileName) {
    fileName_ = fileName;
}

bool IvwSerializeBase::isPrimitivePointerType(const std::type_info& type) const {
    if (type == typeid(bool *)
        || type == typeid(char *)
        || type == typeid(signed int *)
        || type == typeid(unsigned int *)
        || type == typeid(float *)
        || type == typeid(double *)
        || type == typeid(long double *)
        || type == typeid(std::string *))
    {
        return true;
    }

    return false;
}

bool IvwSerializeBase::isPrimitiveType(const std::type_info& type) const {
    if (type == typeid(bool)
        || type == typeid(char)
        || type == typeid(signed int)
        || type == typeid(unsigned int)
        || type == typeid(float)
        || type == typeid(double)
        || type == typeid(long double)
        || type == typeid(std::string))
    {
        return true;
    }

    return false;
}

void IvwSerializeBase::setAllowReference(const bool& allowReference) {
    allowRef_ = allowReference;
}

} //namespace
