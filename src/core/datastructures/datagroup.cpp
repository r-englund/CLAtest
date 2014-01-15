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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/core/datastructures/datagroup.h>

namespace inviwo {

DataGroup::DataGroup() : editableUpdate_(false) { 
}

DataGroup::DataGroup(const DataGroup& rhs) : editableUpdate_(false) {
    for(size_t i = 0; i < rhs.data_.size(); ++i) {
        data_.push_back(rhs.data_[i]->clone());
    }
    for(size_t i = 0; i < rhs.groupData_.size(); ++i) {
        groupData_.push_back(rhs.groupData_[i]->clone());
    }
    //TODO: copy representations!?!
}

DataGroup& DataGroup::operator=(const DataGroup& that){
    if (this != &that) {
        deinitialize();
        for(size_t i = 0; i < that.data_.size(); ++i) {
            data_.push_back(that.data_[i]->clone());
        }
        for(size_t i = 0; i < that.groupData_.size(); ++i) {
            groupData_.push_back(that.groupData_[i]->clone());
        }
        //TODO: copy representations!?!
    }
    return *this;
}


DataGroup::~DataGroup() {
    deinitialize();
}

void DataGroup::deinitialize() {
    clearRepresentations();
    for(size_t i = 0; i < data_.size(); ++i) {
        delete data_[i];
    }
    data_.clear();
    for(size_t i = 0; i < groupData_.size(); ++i) {
        delete groupData_[i];
    }
    groupData_.clear();
}


void DataGroup::clearRepresentations() {
    while (hasRepresentations()) {
        delete representations_.back();
        representations_.pop_back();
    }
}

bool DataGroup::hasRepresentations() const {
    return !representations_.empty();
}

} // namespace
