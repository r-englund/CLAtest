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

IvwSerializeBase::IvwSerializeBase(IvwSerializeBase &s, bool allowReference): _fileName(s._fileName), _doc(s._fileName), _allowReference(allowReference) {
    
    registerFactories();
    /*try {
        TxComment* comment;

        TxDeclaration* decl = new TxDeclaration( IvwSerializeConstants::XML_VERSION, "", "" ); 
	    _doc.LinkEndChild( decl ); 
     
        _root = new TxElement(IvwSerializeConstants::INVIWO_TREEDATA);  
        _root->SetAttribute(IvwSerializeConstants::VERSION_ATTRIBUTE, IvwSerializeConstants::INVIWO_VERSION);
	    _doc.LinkEndChild( _root );

        comment = new TxComment();
	    comment->SetValue(IvwSerializeConstants::EDIT_COMMENT.c_str());  
	    _root->LinkEndChild( comment ); 
    }
    catch(TxException& ex) {
        
    }*/
}

IvwSerializeBase::IvwSerializeBase(std::string fileName, bool allowReference): _fileName(fileName), _doc(fileName), _allowReference(allowReference) {
    
    registerFactories();
    /*try {
        TxComment* comment;

        TxDeclaration* decl = new TxDeclaration( IvwSerializeConstants::XML_VERSION, "", "" ); 
	    _doc.LinkEndChild( decl ); 
     
        _root = new TxElement(IvwSerializeConstants::INVIWO_TREEDATA);  
        _root->SetAttribute(IvwSerializeConstants::VERSION_ATTRIBUTE, IvwSerializeConstants::INVIWO_VERSION);
	    _doc.LinkEndChild( _root );

        comment = new TxComment();
	    comment->SetValue(IvwSerializeConstants::EDIT_COMMENT.c_str());  
	    _root->LinkEndChild( comment ); 
    }
    catch(TxException& ex) {
        
    }*/
}

IvwSerializeBase::~IvwSerializeBase() {
}

void IvwSerializeBase::registerFactories(void) {
    _registeredFactoryVector.clear();
    _registeredFactoryVector.push_back(InviwoFactoryBase::instance<ProcessorFactory>());
    //_registeredFactoryVector.push_back(InviwoFactoryBase::instance<PropertyFactory>());
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


/*
void IvwSerializeBase::serialize(const std::string &key, const IvwSerializable &sObj) {
    TxElement* newNode = new TxElement(key);
    _root->LinkEndChild(newNode);

    NodeSwitch tempNodeSwitch(*this, newNode);
    sObj.serialize(*this) ; 
}

void IvwSerializeBase::serializePrimitives(const std::string &key, const std::string &data) {

    std::stringstream ss;
    ss.precision(IvwSerializeConstants::STRINGSTREAM_PRECISION);
    ss<<data;

     //TxElement* node = new TxElement(key);
     //_root->LinkEndChild(node);
     //node->SetAttribute(IvwSerializeConstants::VALUE_ATTRIBUTE, ss.str());

    //TODO: Check key for xml keys
    _root->SetAttribute(key, ss.str());
}

void IvwSerializeBase::serialize(const std::string &key, const std::string &data) {
    serializePrimitives(key, data) ;
}

void IvwSerializeBase::serialize(const std::string &key, const ivec2 &data) {
    serializeVector(key, data);
}
void IvwSerializeBase::serialize(const std::string &key, const ivec3 &data) {
    serializeVector(key, data);
}

void IvwSerializeBase::serialize(const std::string &key, const ivec4 &data) {
    serializeVector(key, data);
}

void IvwSerializeBase::serialize(const std::string &key, const vec2 &data) {
    serializeVector(key, data);
}
void IvwSerializeBase::serialize(const std::string &key, const vec3 &data) {
    serializeVector(key, data);
}

void IvwSerializeBase::serialize(const std::string &key, const vec4 &data) {
    serializeVector(key, data);
}

void IvwSerializeBase::readFile(std::istream& stream) {

    try {
        _doc.LoadFile();
        //TxNode* pNode = _doc.FirstChildElement()->NextSibling();
    }
    catch(TxException& ex)
    {
        std::cout << ex.what() << std::endl;
    }
}


void IvwSerializeBase::writeFile(std::ostream& stream) {

    try { 
        std::cout<<_doc;
        stream<<_doc;
        _doc.SaveFile();
    }
    catch(TxException& ex) {
        
    }

}
*/

} //namespace
