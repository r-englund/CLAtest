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
 * Primary author : Daniel Jönsson
 *
 **********************************************************************/

#include <inviwo/core/datastructures/data.h>

namespace inviwo {

Data::Data()
    : validRepresentations_(0)
    , lastValidRepresentation_(NULL)
    , dataFormatBase_(DataFormatBase::get()){
}

Data::Data(const DataFormatBase* format)
: validRepresentations_(0)
, lastValidRepresentation_(NULL)
, dataFormatBase_(format){
}

Data::Data(const Data& rhs) 
    : validRepresentations_(0)
    , lastValidRepresentation_(NULL)
    , dataFormatBase_(rhs.dataFormatBase_){
    rhs.copyRepresentationsTo(this);
}

Data& Data::operator=(const Data& that) {
    if (this != &that) {
        that.copyRepresentationsTo(this);
        dataFormatBase_ = that.dataFormatBase_;
    }
    return *this;
}


Data::~Data() {
    clearRepresentations();
}

void Data::clearRepresentations() {
    setAllRepresentationsAsInvalid();
    while (hasRepresentations()) {
        delete representations_.back();
        representations_.pop_back();
    }
}

void Data::copyRepresentationsTo(Data* targetData) const{
    targetData->clearRepresentations();
    if(lastValidRepresentation_) {
        DataRepresentation* rep = lastValidRepresentation_->clone();
        targetData->addRepresentation(rep);
    }
}

void Data::addRepresentation(DataRepresentation* representation) {
    representations_.push_back(representation);
    lastValidRepresentation_ = representation;
    setRepresentationAsValid(static_cast<int>(representations_.size())-1);
    newRepresentationCreated();
}

void Data::removeRepresentation( DataRepresentation* representation )
{
    std::vector<DataRepresentation*>::iterator it = std::find(representations_.begin(), representations_.end(), representation);
    if(it != representations_.end()) {
        // Update last valid representation
        if(lastValidRepresentation_ == *it) {
            lastValidRepresentation_ = NULL;
            for(int i = static_cast<int>(representations_.size())-1; i >= 0; --i) {
                // Check if this representation is valid 
                // and make sure that it is not the one removed
                if(isRepresentationValid(i) && representations_[i] != representation) {
                    lastValidRepresentation_ = representations_[i];
                    break;
                }
            }
        }
        // Update valid representation bit mask
        size_t element = static_cast<size_t>(std::distance(representations_.begin(), it));
        // Start after the element that is going to be removed and update the mask.
        for(int i = static_cast<int>(element+1); i < static_cast<int>(representations_.size()); ++i) {
            if(isRepresentationValid(i)) {
                setRepresentationAsValid(i-1);
            } else {
                setRepresentationAsInvalid(i-1);
            }
        }
        delete (*it);
        representations_.erase(it);
    }
    
}

bool Data::hasRepresentations() const {
    return !representations_.empty();
}

void Data::setDataFormat(const DataFormatBase* format){
    dataFormatBase_ = format;
}

const DataFormatBase* Data::getDataFormat() const{
    return dataFormatBase_;
}




} // namespace
