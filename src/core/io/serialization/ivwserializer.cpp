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

#include <inviwo/core/io/serialization/ivwserializer.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/io/serialization/ivwserializable.h>
#include <inviwo/core/util/inviwofactorybase.h>


namespace inviwo {

IvwSerializer::IvwSerializer(IvwSerializer &s, bool allowReference)
    : IvwSerializeBase(s.getFileName(), allowReference){
    initialize();
}

IvwSerializer::IvwSerializer(std::string fileName, bool allowReference)
    : IvwSerializeBase(fileName, allowReference) {
    initialize();
}

IvwSerializer::~IvwSerializer() {}

void IvwSerializer::initialize(){
    registerFactories();
    
    try {
        TxComment* comment;
        
        TxDeclaration* decl = new TxDeclaration(IvwSerializeConstants::XML_VERSION, "", "" );
	    doc_.LinkEndChild( decl );
        
        rootElement_ = new TxElement(IvwSerializeConstants::INVIWO_TREEDATA);
        rootElement_->SetAttribute(IvwSerializeConstants::VERSION_ATTRIBUTE,
                                   IvwSerializeConstants::INVIWO_VERSION);
	    doc_.LinkEndChild(rootElement_);
        
        comment = new TxComment();
	    comment->SetValue(IvwSerializeConstants::EDIT_COMMENT.c_str());
	    rootElement_->LinkEndChild(comment);
    } catch (TxException& e) {
        throw SerializationException(e.what());
    }
}
    
void IvwSerializer::serialize(const std::string &key, const IvwSerializable &sObj) {
    TxElement* newNode = new TxElement(key);
    rootElement_->LinkEndChild(newNode);

    NodeSwitch tempNodeSwitch(*this, newNode);
    sObj.serialize(*this); 
}
   
void IvwSerializer::serialize(const std::string &key,
                              const std::string &data,
                              const bool asAttribute) {
    if (asAttribute) {
        rootElement_->SetAttribute(key, data);
    } else {
        serializePrimitives<std::string>(key, data);
    }
}

void IvwSerializer::serialize(const std::string &key, const bool &data) {
    serializePrimitives<bool>(key, data);
}
void IvwSerializer::serialize(const std::string &key, const float &data) {
    serializePrimitives<float>(key, data);
}
void IvwSerializer::serialize(const std::string &key, const double &data) {
    serializePrimitives<double>(key, data);
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
void IvwSerializer::serialize(const std::string &key, const unsigned long long &data) {
    serializePrimitives<unsigned long long>(key, data);
}

    
void IvwSerializer::writeFile() {
    try { 
        refDataContainer_.setReferenceAttributes();
        doc_.SaveFile();
    } catch (TxException& e) {
        throw SerializationException(e.what());
    }
}


} //namespace
