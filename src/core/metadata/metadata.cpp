#include "inviwo/core/metadata/metadata.h"

namespace inviwo {

const std::string MetaData::logSource_ = "MetaData";


MetaData::MetaData() {}

MetaData::~MetaData() {}

std::string MetaData::getClassName() const {
    return "MetaData";
}

MetaData* MetaData::create() const {    
    return 0;
}

void MetaData::serialize(IvwSerializer& ) const{

}

void MetaData::deserialize(IvwDeserializer& ) {

}



/*---------------------------------------------------------------------*/

BoolMetaData::BoolMetaData() 
    : MetaDataPrimitiveType<bool>(false) 
{ }

BoolMetaData::BoolMetaData(bool value) 
    : MetaDataPrimitiveType<bool>(value) 
{ }

std::string BoolMetaData::getClassName() const {
    return "BoolMetaData";
}

MetaData* BoolMetaData::create() const {
    return new BoolMetaData();
}

void BoolMetaData::serialize(IvwSerializer& ) const {
}

void BoolMetaData::deserialize(IvwDeserializer& ) {

}

/*---------------------------------------------------------------------*/

IntMetaData::IntMetaData() 
: MetaDataPrimitiveType<int>(false) 
{ }

IntMetaData::IntMetaData(int value) 
: MetaDataPrimitiveType<int>(value) 
{ }

std::string IntMetaData::getClassName() const {
    return "IntMetaData";
}

MetaData* IntMetaData::create() const {
    return new BoolMetaData();
}

void IntMetaData::serialize(IvwSerializer& ) const {
}

void IntMetaData::deserialize(IvwDeserializer& ) {

}

/*---------------------------------------------------------------------*/

IVec2MetaData::IVec2MetaData() 
: MetaDataPrimitiveType<ivec2>(ivec2(0)) 
{ }

IVec2MetaData::IVec2MetaData(ivec2 value) 
: MetaDataPrimitiveType<ivec2>(value) 
{ }

std::string IVec2MetaData::getClassName() const {
    return "IVec2MetaData";
}

MetaData* IVec2MetaData::create() const {
    return new IVec2MetaData();
}

void IVec2MetaData::serialize(IvwSerializer& ) const {
}

void IVec2MetaData::deserialize(IvwDeserializer& ) {

}

/*---------------------------------------------------------------------*/

IVec3MetaData::IVec3MetaData() 
: MetaDataPrimitiveType<ivec3>(ivec3(0)) 
{ }

IVec3MetaData::IVec3MetaData(ivec3 value) 
: MetaDataPrimitiveType<ivec3>(value) 
{ }

std::string IVec3MetaData::getClassName() const {
    return "IVec3MetaData";
}

MetaData* IVec3MetaData::create() const {
    return new IVec3MetaData();
}

void IVec3MetaData::serialize(IvwSerializer& ) const {
}

void IVec3MetaData::deserialize(IvwDeserializer& ) {

}

/*---------------------------------------------------------------------*/

FloatMetaData::FloatMetaData() 
: MetaDataPrimitiveType<float>(0.0f) 
{ }

FloatMetaData::FloatMetaData(float value) 
: MetaDataPrimitiveType<float>(value) 
{ }

std::string FloatMetaData::getClassName() const {
    return "FloatMetaData";
}

MetaData* FloatMetaData::create() const {
    return new FloatMetaData();
}

void FloatMetaData::serialize(IvwSerializer& ) const {
}

void FloatMetaData::deserialize(IvwDeserializer& ) {

}

/*---------------------------------------------------------------------*/

Vec2MetaData::Vec2MetaData() 
: MetaDataPrimitiveType<vec2>(vec2(0)) 
{ }

Vec2MetaData::Vec2MetaData(vec2 value) 
: MetaDataPrimitiveType<vec2>(value) 
{ }

std::string Vec2MetaData::getClassName() const {
    return "Vec2MetaData";
}

MetaData* Vec2MetaData::create() const {
    return new Vec2MetaData();
}

void Vec2MetaData::serialize(IvwSerializer& ) const {
}

void Vec2MetaData::deserialize(IvwDeserializer& ) {

}

/*---------------------------------------------------------------------*/

Vec3MetaData::Vec3MetaData() 
: MetaDataPrimitiveType<vec3>(vec3(0)) 
{ }

Vec3MetaData::Vec3MetaData(vec3 value) 
: MetaDataPrimitiveType<vec3>(value) 
{ }

std::string Vec3MetaData::getClassName() const {
    return "Vec3MetaData";
}

MetaData* Vec3MetaData::create() const {
    return new Vec3MetaData();
}

void Vec3MetaData::serialize(IvwSerializer& ) const {
}

void Vec3MetaData::deserialize(IvwDeserializer& ) {

}

/*---------------------------------------------------------------------*/

StringMetaData::StringMetaData() 
: MetaDataPrimitiveType<std::string>(std::string("")) 
{ }

StringMetaData::StringMetaData(std::string value) 
: MetaDataPrimitiveType<std::string>(value) 
{ }

std::string StringMetaData::getClassName() const {
    return "StringMetaData";
}

MetaData* StringMetaData::create() const {
    return new StringMetaData();
}

void StringMetaData::serialize(IvwSerializer& ) const {
}

void StringMetaData::deserialize(IvwDeserializer& ) {

}

/*---------------------------------------------------------------------*/

} // namespace
