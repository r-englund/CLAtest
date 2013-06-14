#include <inviwo/core/io/serialization/ivwserializer.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/io/serialization/ivwserializable.h>
#include <inviwo/core/util/inviwofactorybase.h>


namespace inviwo {

IvwSerializer::IvwSerializer(IvwSerializer &s, bool allowReference)
    : IvwSerializeBase(s.getFileName(), allowReference)
{
    registerFactories();

    try {
        TxComment* comment;

        TxDeclaration* decl = new TxDeclaration( IvwSerializeConstants::XML_VERSION, "", "" ); 
	    doc_.LinkEndChild( decl ); 
     
        rootElement_ = new TxElement(IvwSerializeConstants::INVIWO_TREEDATA);  
        rootElement_->SetAttribute(IvwSerializeConstants::VERSION_ATTRIBUTE, IvwSerializeConstants::INVIWO_VERSION);
	    doc_.LinkEndChild( rootElement_ );

        comment = new TxComment();
	    comment->SetValue(IvwSerializeConstants::EDIT_COMMENT.c_str());  
	    rootElement_->LinkEndChild( comment ); 
    }
    catch (TxException& ) {
        
    }
}

IvwSerializer::IvwSerializer(std::string fileName, bool allowReference)
    : IvwSerializeBase(fileName, allowReference) {
    
    registerFactories();

    try {
        TxComment* comment;

        TxDeclaration* decl = new TxDeclaration( IvwSerializeConstants::XML_VERSION, "", "" ); 
	    doc_.LinkEndChild( decl ); 
     
        rootElement_ = new TxElement(IvwSerializeConstants::INVIWO_TREEDATA);  
        rootElement_->SetAttribute(IvwSerializeConstants::VERSION_ATTRIBUTE, IvwSerializeConstants::INVIWO_VERSION);
	    doc_.LinkEndChild( rootElement_ );

        comment = new TxComment();
	    comment->SetValue(IvwSerializeConstants::EDIT_COMMENT.c_str());  
	    rootElement_->LinkEndChild( comment ); 
    }
    catch (TxException& ) {
        
    }
}

IvwSerializer::~IvwSerializer() {
}

void IvwSerializer::serialize(const std::string &key, const IvwSerializable &sObj) {
    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);

    NodeSwitch tempNodeSwitch(*this, newNode);
    sObj.serialize(*this); 
}

void IvwSerializer::serializeAttributes(const std::string &key, const std::string &data) {

    std::stringstream ss;
    ss.precision(IvwSerializeConstants::STRINGSTREAM_PRECISION);
    ss<<data;

     //TxElement* node = new TxElement(key);
     //rootElement_->LinkEndChild(node);
     //node->SetAttribute(IvwSerializeConstants::CONTENT_ATTRIBUTE, ss.str());

    //TODO: Check key for xml keys
    rootElement_->SetAttribute(key, ss.str());
}

void IvwSerializer::serializePrimitives(const std::string &key, const std::string &data) {

    std::stringstream ss;
    ss.precision(IvwSerializeConstants::STRINGSTREAM_PRECISION);
    ss<<data;

     TxElement* node = new TxElement(key);
     rootElement_->LinkEndChild(node);
     node->SetAttribute(IvwSerializeConstants::CONTENT_ATTRIBUTE, ss.str());
}

void IvwSerializer::serialize(const std::string &key, const std::string &data, const bool asAttribute) {
    if (asAttribute) {
        serializeAttributes(key, data);
    }
    else {
        serializePrimitives(key, data);
    }
}

void IvwSerializer::serialize(const std::string &key, const float &data) {
    serializePrimitives<float>(key, data);
}

void IvwSerializer::serialize(const std::string &key, const double &data) {
    serializePrimitives<double>(key, data);
}

void IvwSerializer::serialize(const std::string &key, const bool &data) {
    serializePrimitives<bool>(key, data);
}

void IvwSerializer::serialize(const std::string &key, const int &data) {
    serializePrimitives<int>(key, data);
}

void IvwSerializer::serialize(const std::string &key, const unsigned int &data) {
    serializePrimitives<unsigned int>(key, data);
}

void IvwSerializer::serialize(const std::string &key, const long &data) {
    serializePrimitives<long>(key, data);
}

void IvwSerializer::serialize(const std::string &key, const long long &data) {
    serializePrimitives<long long>(key, data);
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

void IvwSerializer::serialize(const std::string &key, const uvec2 &data) {
    serializeVector(key, data);
}
void IvwSerializer::serialize(const std::string &key, const uvec3 &data) {
    serializeVector(key, data);
}

void IvwSerializer::serialize(const std::string &key, const uvec4 &data) {
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

void IvwSerializer::serialize(const std::string &key, const dvec2 &data) {
    serializeVector(key, data);
}
void IvwSerializer::serialize(const std::string &key, const dvec3 &data) {
    serializeVector(key, data);
}

void IvwSerializer::serialize(const std::string &key, const dvec4 &data) {
    serializeVector(key, data);
}

void IvwSerializer::serialize(const std::string &key, const mat2 &data) {
    vec2 rowVec;
    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);
    NodeSwitch tempNodeSwitch(*this, newNode);

    for (size_t i=0; i<2; i++) {
        std::stringstream key;
        key << "row" << i;
        rowVec = vec2(data[i][0], data[i][1]);
        serializeVector(key.str(), rowVec);
    }
}
void IvwSerializer::serialize(const std::string &key, const mat3 &data) {
    vec3 rowVec;
    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);
    NodeSwitch tempNodeSwitch(*this, newNode);

    for (size_t i=0; i<3; i++) {
        std::stringstream key;
        key << "row" << i;
        rowVec = vec3(data[i][0], data[i][1], data[i][2]);
        serializeVector(key.str(), rowVec);
    }
}

void IvwSerializer::serialize(const std::string &key, const mat4 &data) {
    vec4 rowVec;
    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);
    NodeSwitch tempNodeSwitch(*this, newNode);

    for (size_t i=0; i<4; i++) {
        std::stringstream key;
        key << "row" << i;
        rowVec = vec4(data[i][0], data[i][1], data[i][2], data[i][3]);
        serializeVector(key.str(), rowVec);
    }    
}

void IvwSerializer::writeFile() {

    try { 
        refDataContainer_.setReferenceAttributes();
        doc_.SaveFile();
    }
    catch (TxException& ) {
        
    }

}


} //namespace
