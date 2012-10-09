#include "inviwo/core/io/serialization/ivwserializebase.h"
#include "inviwo/core/inviwofactorybase.h"
#include "inviwo/core/processors/processorfactory.h"
#include "inviwo/core/io/serialization/ivwserializable.h"
#include "inviwo/core/inviwo.h"


namespace inviwo {

IvwSerializeBase::NodeSwitch::NodeSwitch(IvwSerializeBase& serializer, TxElement* node)
    : _serializer(serializer)
    , _storedNode(_serializer._root)
{
    _serializer._root = node;
}

IvwSerializeBase::NodeSwitch::~NodeSwitch() {
    _serializer._root = _storedNode;
}

IvwSerializeBase::IvwSerializeBase(IvwSerializeBase &s, bool allowReference)
    : _fileName(s._fileName)
    , _doc(s._fileName)
    , _allowReference(allowReference)
{
     registerFactories();  
}

IvwSerializeBase::IvwSerializeBase(std::string fileName, bool allowReference)
    : _fileName(fileName)
    , _doc(fileName)
    , _allowReference(allowReference)
{
    registerFactories(); 
}

IvwSerializeBase::~IvwSerializeBase() {
}

void IvwSerializeBase::registerFactories(void) {
    _registeredFactoryVector.clear();
    _registeredFactoryVector.push_back(InviwoFactoryBase::instance<ProcessorFactory>());
}

template <typename T>
T* IvwSerializeBase::allocateMemory(std::string className) {
    T* data = 0;
    return data;
}

std::string IvwSerializeBase::getFileName() {
    return _fileName;
}

void IvwSerializeBase::setFileName(const std::string fileName) {
    _fileName = fileName;
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
    _allowReference = allowReference;
}

} //namespace
