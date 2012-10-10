#include "inviwo/core/io/serialization/ivwserializer.h"
#include "inviwo/core/inviwofactorybase.h"
#include "inviwo/core/processors/processorfactory.h"
#include "inviwo/core/io/serialization/ivwserializable.h"
#include "inviwo/core/inviwo.h"


namespace inviwo {

IvwSerializer::IvwSerializer(IvwSerializer &s, bool allowReference)
    : IvwSerializeBase(s.getFileName(), allowReference)
{
    registerFactories();

    try {
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
        
    }
}

IvwSerializer::IvwSerializer(std::string fileName, bool allowReference)
    : IvwSerializeBase(fileName, allowReference) {
    
    registerFactories();

    try {
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
        
    }
}

IvwSerializer::~IvwSerializer() {
}

void IvwSerializer::serialize(const std::string &key, const IvwSerializable &sObj) {
    TxElement* newNode = new TxElement(key);
    _root->LinkEndChild(newNode);

    NodeSwitch tempNodeSwitch(*this, newNode);
    sObj.serialize(*this) ; 
}

void IvwSerializer::serializeAttributes(const std::string &key, const std::string &data) {

    std::stringstream ss;
    ss.precision(IvwSerializeConstants::STRINGSTREAM_PRECISION);
    ss<<data;

     //TxElement* node = new TxElement(key);
     //_root->LinkEndChild(node);
     //node->SetAttribute(IvwSerializeConstants::CONTENT_ATTRIBUTE, ss.str());

    //TODO: Check key for xml keys
    _root->SetAttribute(key, ss.str());
}

void IvwSerializer::serializePrimitives(const std::string &key, const std::string &data) {

    std::stringstream ss;
    ss.precision(IvwSerializeConstants::STRINGSTREAM_PRECISION);
    ss<<data;

     TxElement* node = new TxElement(key);
     _root->LinkEndChild(node);
     node->SetAttribute(IvwSerializeConstants::CONTENT_ATTRIBUTE, ss.str());
}

void IvwSerializer::serialize(const std::string &key, const std::string &data, const bool asAttribute) {
    if(asAttribute)
    {
        serializeAttributes(key, data) ;
    }
    else {
        serializePrimitives(key, data) ;
    }
}

void IvwSerializer::serialize(const std::string &key, const float &data) {
    serializePrimitives<float>(key, data);
}
void IvwSerializer::serialize(const std::string &key, const int &data) {
    serializePrimitives<int>(key, data);
}

void IvwSerializer::serialize(const std::string &key, const ivec2 &data) {
    serializeVector(key, data);
}
void IvwSerializer::serialize(const std::string &key, const ivec3 &data) {
    serializeVector(key, data);
}

void IvwSerializer::serialize(const std::string &key, const ivec4 &data) {
    serializeVector(key, data);
}

void IvwSerializer::serialize(const std::string &key, const vec2 &data) {
    serializeVector(key, data);
}
void IvwSerializer::serialize(const std::string &key, const vec3 &data) {
    serializeVector(key, data);
}

void IvwSerializer::serialize(const std::string &key, const vec4 &data) {
    serializeVector(key, data);
}


void IvwSerializer::writeFile(std::ostream& stream) {

    try { 
        std::cout<<_doc;
        stream<<_doc;
        _doc.SaveFile();
    }
    catch(TxException& ex) {
        
    }

}


} //namespace
