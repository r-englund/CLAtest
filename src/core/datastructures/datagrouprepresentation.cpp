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

DataGroupRepresentation::DataGroupRepresentation()
    : DataRepresentation(), valid_(false) {}

DataGroupRepresentation::DataGroupRepresentation(const DataGroupRepresentation& rhs)
    : DataRepresentation(rhs), valid_(false) {}

DataGroupRepresentation::~DataGroupRepresentation() { }

std::string DataGroupRepresentation::getClassName() const {
    return "DataGroupRepresentation";
}

DataGroupRepresentation& DataGroupRepresentation::operator=(const DataGroupRepresentation& that) {
    if (this != &that)
        DataRepresentation::operator=(that);

    return *this;
}

void DataGroupRepresentation::setAsInvalid() {
    valid_ = false;
}

bool DataGroupRepresentation::isValid() {
    return valid_;
}

void DataGroupRepresentation::setAsValid() {
    valid_ = true;
}

} // namespace
