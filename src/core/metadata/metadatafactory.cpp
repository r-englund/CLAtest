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

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/common/inviwomodule.h>
#include <inviwo/core/metadata/metadatafactory.h>
#include <inviwo/core/io/serialization/ivwserializable.h>


namespace inviwo {

MetaDataFactory::MetaDataFactory() {
    initialize();
}

MetaDataFactory::~MetaDataFactory() {}

void MetaDataFactory::initialize() {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
        std::vector<MetaData*> curMetadataList = inviwoApp->getModules()[curModuleId]->getMetaData();
        for (size_t curMetadataId=0; curMetadataId<curMetadataList.size(); curMetadataId++)
            registerMetaData(curMetadataList[curMetadataId]);
    }
}

void MetaDataFactory::registerMetaData(MetaData* meta) {
    if (metaDataClassMap_.find(meta->getClassName()) == metaDataClassMap_.end())
        metaDataClassMap_.insert(std::make_pair(meta->getClassName(), meta));
}

IvwSerializable* MetaDataFactory::create(std::string className) const {
    std::map<std::string, MetaData*>::iterator it = metaDataClassMap_.find(className);
    if (it != metaDataClassMap_.end())
        return it->second->clone();
    else
        return 0;
}

bool MetaDataFactory::isValidType(std::string className) const {
    std::map<std::string, MetaData*>::iterator it = metaDataClassMap_.find(className);
    if (it != metaDataClassMap_.end())
        return true;
    else
        return false;
}

void MetaDataFactory::deinitialize() {
}

} // namespace
