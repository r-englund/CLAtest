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
 * Primary author : Peter Steneteg
 *
 **********************************************************************/

#include <inviwo/core/metadata/metadata.h>

namespace inviwo {

MetaData::MetaData() {}
MetaData::MetaData(const MetaData& rhs) {};
MetaData& MetaData::operator=(const MetaData& that) {
    return *this;
}
MetaData::~MetaData() {}
std::string MetaData::getClassName() const {
    return "MetaData";
}

MetaData* MetaData::clone() const {
    return new MetaData(*this);
}

void MetaData::serialize(IvwSerializer& s) const {
    IVW_UNUSED_PARAM(s);
}

void MetaData::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
}

#define MetaDataMacro(n, t, d, v) \
    n##MetaData::n##MetaData() : MetaDataPrimitiveType<t>(d) {}; \
    n##MetaData::n##MetaData(t value) : MetaDataPrimitiveType<t>(value) {}; \
    std::string n##MetaData::getClassName() const {std::stringstream s; s << #n; s << "MetaData"; return s.str(); }; \
    n##MetaData* n##MetaData::clone() const { return new n##MetaData(*this); }; \
     
#include <inviwo/core/metadata/metadatadefinefunc.h>


} // namespace
