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

#include <inviwo/core/metadata/compositemetadata.h>

namespace inviwo {

CompositeMetaData::CompositeMetaData() : MetaData() {}

CompositeMetaData::CompositeMetaData( const CompositeMetaData& rhs ) 
    : MetaData(rhs){
    for(std::vector<MetaData*>::const_iterator it = rhs.subMetaData_.begin();
        it != rhs.subMetaData_.end(); ++it){
            subMetaData_.push_back((*it)->clone());
    }
}

CompositeMetaData& CompositeMetaData::operator=( const CompositeMetaData& that ){
    if (this != &that) {
        for(std::vector<MetaData*>::iterator it = subMetaData_.begin();
            it != subMetaData_.end(); ++it){
                if(*it)
                    delete *it;
        }
        subMetaData_.clear();
        for(std::vector<MetaData*>::const_iterator it = that.subMetaData_.begin();
            it != that.subMetaData_.end(); ++it){
                subMetaData_.push_back((*it)->clone());
        }
        MetaData::operator=(that);
    }
    return *this;
}

CompositeMetaData::~CompositeMetaData() {
    subMetaData_.clear();
}

CompositeMetaData* CompositeMetaData::create() const {
    return new CompositeMetaData();
}
CompositeMetaData* CompositeMetaData::clone() const{
    return new CompositeMetaData(*this);
}

void CompositeMetaData::addMetaData(MetaData* metadata) {
    subMetaData_.push_back(metadata);
}

void CompositeMetaData::addMetaData(MetaData& metadata) {
    addMetaData(&metadata);
}

void  CompositeMetaData::serialize(IvwSerializer& s) const {
   //MetaData::serialize(s);
}

void  CompositeMetaData::deserialize(IvwDeserializer& d) {
   //MetaData::deserialize(d);
}





} // namespace
