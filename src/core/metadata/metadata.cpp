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
MetaData& MetaData::operator=(const MetaData& that){
    return *this;
}
MetaData::~MetaData() {}
std::string MetaData::getClassName() const {
    return "MetaData";
}

MetaData* MetaData::clone() const {
    return new MetaData(*this);
}

void MetaData::serialize(IvwSerializer& s) const{
    IVW_UNUSED_PARAM(s);
}

void MetaData::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
}

#define MetaDataMacroB(n, t, d) \
    n##MetaData::n##MetaData() : MetaDataPrimitiveType<t>(d) {}; \
    n##MetaData::n##MetaData(t value) : MetaDataPrimitiveType<t>(value) {}; \
    std::string n##MetaData::getClassName() const {std::stringstream s; s << #n; s << "MetaData"; return s.str(); }; \
    n##MetaData* n##MetaData::clone() const { return new n##MetaData(*this); }; \

MetaDataMacroB(Bool, bool, false)
MetaDataMacroB(Int, int, 0)
MetaDataMacroB(Float, float, 0.0f)
MetaDataMacroB(String, std::string, "")
MetaDataMacroB(IVec2, ivec2, ivec2(0))
MetaDataMacroB(IVec3, ivec3, ivec3(0))
MetaDataMacroB(IVec4, ivec4, ivec4(0))
MetaDataMacroB(Vec2, vec2, vec2(0))
MetaDataMacroB(Vec3, vec3, vec3(0))
MetaDataMacroB(Vec4, vec4, vec4(0))
MetaDataMacroB(DVec2, dvec2, dvec2(0))
MetaDataMacroB(DVec3, dvec3, dvec3(0))
MetaDataMacroB(DVec4, dvec4, dvec4(0))
MetaDataMacroB(UVec2, uvec2, uvec2(0))
MetaDataMacroB(UVec3, uvec3, uvec3(0))
MetaDataMacroB(UVec4, uvec4, uvec4(0))
MetaDataMacroB(Mat2, mat2, mat2(0))
MetaDataMacroB(Mat3, mat3, mat3(0))
MetaDataMacroB(Mat4, mat4, mat4(0))

//#include <inviwo/core/metadata/metadatadefinefunc.h>

} // namespace
