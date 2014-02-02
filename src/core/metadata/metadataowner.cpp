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

#include <inviwo/core/metadata/metadataowner.h>

namespace inviwo {

MetaDataOwner::MetaDataOwner()
    : metaData_(new MetaDataMap()) {}

MetaDataOwner::MetaDataOwner(const MetaDataOwner& rhs)
    : metaData_(rhs.metaData_->clone()) {}

MetaDataOwner& MetaDataOwner::operator=(const MetaDataOwner& that) {
    if (this != &that) {
        MetaDataMap* metadata = that.metaData_->clone();
        delete metaData_;
        metaData_ = metadata;
    }

    return *this;
}
MetaDataOwner* MetaDataOwner::clone() const {
    return new MetaDataOwner(*this);
}
MetaDataOwner::~MetaDataOwner() {
    delete metaData_;
}


} // namespace
