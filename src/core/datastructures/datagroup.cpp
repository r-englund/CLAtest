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

DataGroup::DataGroup(){ 
}

DataGroup::DataGroup(const DataGroup& rhs){
    //TOD0: Implement
}

DataGroup& DataGroup::operator=(const DataGroup& that){
    if (this != &that) {
        //TOD0: Implement 
    }
    return *this;
}


DataGroup::~DataGroup() {
}

void DataGroup::addData(Data* dataObj){
    data_.push_back(dataObj);
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
