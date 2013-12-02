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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/core/util/formats.h>
#include <inviwo/core/datastructures/datarepresentation.h>

namespace inviwo {

DataRepresentation::DataRepresentation() 
    : dataFormatBase_(DataUINT8::get()) {}
DataRepresentation::DataRepresentation(const DataFormatBase* format) 
    : dataFormatBase_(format) {}
DataRepresentation::DataRepresentation(const DataRepresentation& rhs) 
    : dataFormatBase_(rhs.dataFormatBase_) {}
DataRepresentation::~DataRepresentation() { }

std::string DataRepresentation::getClassName() const { 
    return "DataRepresentation"; 
}

const DataFormatBase* DataRepresentation::getDataFormat() const { 
    return dataFormatBase_; 
}

std::string DataRepresentation::getDataFormatString() const { 
    return std::string(dataFormatBase_->getString()); 
}

DataFormatId DataRepresentation::getDataFormatId() const { 
    return dataFormatBase_->getId(); 
}

void DataRepresentation::setDataFormat(const DataFormatBase* format){
    dataFormatBase_ = format;
}

} // namespace
