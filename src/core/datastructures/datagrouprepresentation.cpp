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

DataGroupRepresentation::DataGroupRepresentation(std::vector<Data*>* data){
    data_ = data;
}

DataGroupRepresentation::DataGroupRepresentation(const DataGroupRepresentation& rhs) {
    data_ = rhs.data_;
}

DataGroupRepresentation::~DataGroupRepresentation() { }

std::string DataGroupRepresentation::getClassName() const { 
    return "DataGroupRepresentation"; 
}

} // namespace
