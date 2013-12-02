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

CompositeMetaData::CompositeMetaData()
    : MetaData()
{}

CompositeMetaData::~CompositeMetaData() {}

CompositeMetaData* CompositeMetaData::create() const {
    return new CompositeMetaData();
}

void CompositeMetaData::addMetaData(MetaData* metadata) {
    subMetaData_.push_back(metadata);
}

void CompositeMetaData::addMetaData(MetaData& metadata) {
    addMetaData(&metadata);
}

void  CompositeMetaData::serialize(IvwSerializer& s) const {
    MetaData::serialize(s);
}

void  CompositeMetaData::deserialize(IvwDeserializer& d) {
   MetaData::deserialize(d);
}

} // namespace
