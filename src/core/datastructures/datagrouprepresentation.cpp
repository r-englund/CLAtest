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

#include <inviwo/core/datastructures/datagrouprepresentation.h>

namespace inviwo {

DataGroupRepresentation::DataGroupRepresentation() {
    data_ = NULL;
    groupData_ = NULL;
}

DataGroupRepresentation::DataGroupRepresentation(const DataGroupRepresentation& rhs) {
    data_ = rhs.data_;
    groupData_ = rhs.groupData_;
}

DataGroupRepresentation::~DataGroupRepresentation() { }

std::string DataGroupRepresentation::getClassName() const { 
    return "DataGroupRepresentation"; 
}

void DataGroupRepresentation::setPointerToData(std::vector<Data*>* dVec, std::vector<DataGroup*>* gdVec){
    data_ = dVec;
    groupData_ = gdVec;
}

} // namespace
