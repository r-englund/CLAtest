/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
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

#include <inviwo/core/properties/directoryproperty.h>
#include <inviwo/core/properties/fileproperty.h>
#include <inviwo/core/util/urlparser.h>

namespace inviwo {

DirectoryProperty::DirectoryProperty(std::string identifier, std::string displayName,std::string value, PropertyOwner::InvalidationLevel invalidationLevel, 
    PropertySemantics::Type semantics)
    : TemplateProperty<std::string>(identifier, displayName,value, invalidationLevel, semantics),
      fileIndexingHandle_(0)
{    
}


std::vector<std::string> DirectoryProperty::getDirectoryTree()  const  {    
    return directoryTree_;
}

std::vector<std::string> DirectoryProperty::getFiles(std::string filters)  const {
    std::vector<std::string> validFilesWithExtension;
    for (size_t i=0; i<directoryTree_.size(); i++) {
        std::string file = get()+"/";
        file = URLParser::getFileDirectory(file) + directoryTree_[i];
        if (filters=="*.*") {
            validFilesWithExtension.push_back(file);
        }
        else if (URLParser::getFileExtension(directoryTree_[i]) == filters) {
            validFilesWithExtension.push_back(file);
        }
    }
    return validFilesWithExtension;
}

void DirectoryProperty::setDirectoryTree(std::vector<std::string> dirTree)  {    
    directoryTree_ = dirTree;
}

int DirectoryProperty::getVariantType() {
    return Variant::VariantTypeString;
}

Variant DirectoryProperty::getVariant() {
    return Variant(get());
}

void  DirectoryProperty::setVariant(const Variant& val) {
    if (val.canConvert(getVariantType())) {
        set(val.getString());
    }
}

void DirectoryProperty::serialize(IvwSerializer& s) const {
    Property::serialize(s) ;    
    std::string basePath = s.getFileName();
    std::string absoluteFilePath = get();
    if (basePath.empty())
        basePath = IVW_DIR+"data/workspace";
    std::string relativePath = URLParser::getRelativePath(basePath, absoluteFilePath);
    s.serialize("directory", relativePath);    
    s.serialize("files", directoryTree_, "file");
}

void DirectoryProperty::deserialize(IvwDeserializer& d) {
    Property::deserialize(d) ;
    std::string relativePath;
    d.deserialize("directory", relativePath);
    std::string basePath = d.getFileName(); 
    if (basePath.empty())
        basePath = IVW_DIR+"data/workspace";
    basePath = URLParser::getFileDirectory(basePath);   

    std::vector<std::string> directoryTree;
    d.deserialize("files", directoryTree, "file");
    directoryTree_ = directoryTree;

     set(basePath+relativePath);
}

} // namespace
