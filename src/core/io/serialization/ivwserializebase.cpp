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

#include <inviwo/core/io/serialization/ivwserializebase.h>
#include <inviwo/core/processors/processorfactory.h>
#include <inviwo/core/metadata/metadatafactory.h>
#include <inviwo/core/io/serialization/ivwserializable.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/util/factory.h>


namespace inviwo {

IvwSerializeBase::NodeSwitch::NodeSwitch(IvwSerializeBase& serializer, TxElement* node, bool getChild)
    : serializer_(serializer)
    , storedNode_(serializer_.rootElement_)
    , storedGetChild_(serializer_.getChild_){
    serializer_.rootElement_ = node;
    serializer_.getChild_ = getChild;
}

IvwSerializeBase::NodeSwitch::~NodeSwitch() {
    serializer_.rootElement_ = storedNode_;
    serializer_.getChild_ = storedGetChild_;
}

IvwSerializeBase::ReferenceDataContainer::ReferenceDataContainer() {
    referenceCount_ = 0;
}

IvwSerializeBase::ReferenceDataContainer::~ReferenceDataContainer() {
}

size_t IvwSerializeBase::ReferenceDataContainer::insert(const void *data, TxElement *node, bool isPointer) {
 
    IvwSerializeBase::ReferenceData refData;
    refData.node_ = node;
    refData.isPointer_ = isPointer;

    referenceMap_.insert(RefDataPair(data, refData));
    
    return referenceMap_.count(data);
}

    
void IvwSerializeBase::ReferenceDataContainer::setReferenceAttributes() {
    
    std::pair<RefMap::const_iterator, RefMap::const_iterator> sameKeys;
    
    // Loop over all different key valus.
    for (RefMap::const_iterator uniqueKey = referenceMap_.begin();
        uniqueKey != referenceMap_.end();
        uniqueKey = referenceMap_.upper_bound(uniqueKey->first)) {
   
        sameKeys = referenceMap_.equal_range(uniqueKey->first);
        
        if (std::distance(sameKeys.first, sameKeys.second)<=1) continue;

        std::stringstream ss;
        ss<<"ref";
        ss<<referenceCount_;
        
        // Loop over all items with the same key as uniqueKey.
        for (RefMap::const_iterator item = sameKeys.first;
             item != sameKeys.second; ++item) {
            
            if (item->second.isPointer_) {
                item->second.node_->SetAttribute(IvwSerializeConstants::REF_ATTRIBUTE, ss.str());
            } else {
                item->second.node_->SetAttribute(IvwSerializeConstants::ID_ATTRIBUTE, ss.str());
            }
        }
        referenceCount_++;
    }
}
    
size_t IvwSerializeBase::ReferenceDataContainer::find(const void *data) {
    return referenceMap_.count(data);
}
    
void* IvwSerializeBase::ReferenceDataContainer::find(const std::string& type, const std::string& reference_or_id) {
    void* data = 0;

    if(reference_or_id.empty())
        return data;
    
    for (RefMap::iterator it = referenceMap_.begin(); it != referenceMap_.end(); ++it) {
        std::string type_attrib("");
        std::string ref_attrib("");
        std::string id_attrib("");
        
        it->second.node_->GetAttribute(IvwSerializeConstants::TYPE_ATTRIBUTE, &type_attrib, false);
        it->second.node_->GetAttribute( IvwSerializeConstants::REF_ATTRIBUTE, &ref_attrib, false);
        it->second.node_->GetAttribute( IvwSerializeConstants::ID_ATTRIBUTE, &id_attrib, false);
        
        if (type_attrib == type && (ref_attrib == reference_or_id || id_attrib == reference_or_id)) {
            data = const_cast<void *>(it->first);
            break;
        }
    }
    return data;
}
    
TxElement* IvwSerializeBase::ReferenceDataContainer::nodeCopy(const void *data) {
    std::pair<RefMap::iterator, RefMap::iterator> pIt;
    std::vector<ReferenceData> nodes;

    TxElement* nodeCopy=0;

    pIt = referenceMap_.equal_range(data);

    for (RefMap::iterator mIt = pIt.first; mIt != pIt.second; ++mIt) {
        nodeCopy = mIt->second.node_->Clone()->ToElement();
        if (nodeCopy) {
            nodeCopy->Clear();
            break;
        }
    }

    return nodeCopy;
}

IvwSerializeBase::IvwSerializeBase(IvwSerializeBase &s, bool allowReference)
    : fileName_(s.fileName_)
    , doc_(s.fileName_)
    , allowRef_(allowReference){
        
    registerFactories();
}

IvwSerializeBase::IvwSerializeBase(std::string fileName, bool allowReference)
    : fileName_(fileName)
    , doc_(fileName)
    , allowRef_(allowReference){
        
    registerFactories();
}

IvwSerializeBase::~IvwSerializeBase() {
}

void IvwSerializeBase::registerFactories(void) {
    registeredFactories_.clear();
    registeredFactories_.push_back(ProcessorFactory::getPtr());
    registeredFactories_.push_back(MetaDataFactory::getPtr());
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
        || type == typeid(int *)
        || type == typeid(signed int *)
        || type == typeid(unsigned int *)
        || type == typeid(float *)
        || type == typeid(double *)
        || type == typeid(long double *)
        || type == typeid(std::string *)){
        
        return true;
    }

    return false;
}

bool IvwSerializeBase::isPrimitiveType(const std::type_info& type) const {
    if (type == typeid(bool)
        || type == typeid(char)
        || type == typeid(int)
        || type == typeid(signed int)
        || type == typeid(unsigned int)
        || type == typeid(float)
        || type == typeid(double)
        || type == typeid(long double)
        || type == typeid(std::string)) {
        
        return true;
    }

    return false;
}

void IvwSerializeBase::setAllowReference(const bool& allowReference) {
    allowRef_ = allowReference;
}

std::string IvwSerializeBase::nodeToString(const TxElement& node) {
    try {
        TiXmlPrinter printer;
        printer.SetIndent("    ");
        node.Accept(&printer);
        return printer.CStr();
    } catch(TxException&) {
        return "No valid root node";
    }
}

} //namespace
