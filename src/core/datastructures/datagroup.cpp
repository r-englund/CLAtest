 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met: 
 * 
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer. 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Main file authors: Erik Sundén, Peter Steneteg
 *
 *********************************************************************************/

#include <inviwo/core/datastructures/datagroup.h>

namespace inviwo {

DataGroup::DataGroup() : editableUpdate_(false) {
}

DataGroup::DataGroup(const DataGroup& rhs) : editableUpdate_(false) {
    for (size_t i = 0; i < rhs.data_.size(); ++i)
        data_.push_back(rhs.data_[i]->clone());

    for (size_t i = 0; i < rhs.groupData_.size(); ++i)
        groupData_.push_back(rhs.groupData_[i]->clone());
}

DataGroup& DataGroup::operator=(const DataGroup& that) {
    if (this != &that) {
        deinitialize();

        for (size_t i = 0; i < that.data_.size(); ++i)
            data_.push_back(that.data_[i]->clone());

        for (size_t i = 0; i < that.groupData_.size(); ++i)
            groupData_.push_back(that.groupData_[i]->clone());
    }

    return *this;
}


DataGroup::~DataGroup() {
    deinitialize();
}

void DataGroup::deinitialize() {
    clearRepresentations();

    for (size_t i = 0; i < data_.size(); ++i)
        delete data_[i];

    data_.clear();

    for (size_t i = 0; i < groupData_.size(); ++i)
        delete groupData_[i];

    groupData_.clear();
}

bool DataGroup::hasRepresentations() const {
    return !representations_.empty();
}

void DataGroup::setRepresentationsAsInvalid() {
    for (int i=0; i<static_cast<int>(representations_.size()); ++i)
        representations_[i]->setAsInvalid();
}

void DataGroup::clearRepresentations() {
    while (hasRepresentations()) {
        delete representations_.back();
        representations_.pop_back();
    }
}

} // namespace
