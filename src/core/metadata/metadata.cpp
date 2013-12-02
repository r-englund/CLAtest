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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/metadata/metadata.h>

namespace inviwo {

MetaData::MetaData() {}

MetaData::~MetaData() {}

MetaData* MetaData::clone() const {
    return new MetaData(*this);
}

std::string MetaData::getClassName() const {
    return "MetaData";
}

MetaData* MetaData::create() const {    
    return 0;
}

void MetaData::serialize(IvwSerializer& s) const{
    IVW_UNUSED_PARAM(s);
}

void MetaData::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
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

BoolMetaData* BoolMetaData::clone() const{
    return new BoolMetaData(*this);
}

void BoolMetaData::serialize(IvwSerializer& s) const {
    IVW_UNUSED_PARAM(s);
}

void BoolMetaData::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
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

IntMetaData* IntMetaData::clone() const{
    return new IntMetaData(*this);
}

void IntMetaData::serialize(IvwSerializer& s) const {
    IVW_UNUSED_PARAM(s);
}

void IntMetaData::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
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

IVec2MetaData* IVec2MetaData::clone() const{
    return new IVec2MetaData(*this);
}

void IVec2MetaData::serialize(IvwSerializer& s) const {
    IVW_UNUSED_PARAM(s);
}

void IVec2MetaData::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
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
IVec3MetaData* IVec3MetaData::clone() const{
    return new IVec3MetaData(*this);
}
 void IVec3MetaData::serialize(IvwSerializer& s) const {
    IVW_UNUSED_PARAM(s);
}

void IVec3MetaData::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
}

/*---------------------------------------------------------------------*/

UVec2MetaData::UVec2MetaData() 
: MetaDataPrimitiveType<uvec2>(uvec2(0)) 
{ }

UVec2MetaData::UVec2MetaData(uvec2 value) 
: MetaDataPrimitiveType<uvec2>(value) 
{ }

std::string UVec2MetaData::getClassName() const {
    return "UVec2MetaData";
}

MetaData* UVec2MetaData::create() const {
    return new UVec2MetaData();
}
UVec2MetaData* UVec2MetaData::clone() const{
    return new UVec2MetaData(*this);
}
void UVec2MetaData::serialize(IvwSerializer& s) const {
    IVW_UNUSED_PARAM(s);
}

void UVec2MetaData::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
}

/*---------------------------------------------------------------------*/

UVec3MetaData::UVec3MetaData() 
: MetaDataPrimitiveType<uvec3>(uvec3(0)) 
{ }

UVec3MetaData::UVec3MetaData(uvec3 value) 
: MetaDataPrimitiveType<uvec3>(value) 
{ }

std::string UVec3MetaData::getClassName() const {
    return "UVec3MetaData";
}

MetaData* UVec3MetaData::create() const {
    return new UVec3MetaData();
}
UVec3MetaData* UVec3MetaData::clone() const{
    return new UVec3MetaData(*this);
}
void UVec3MetaData::serialize(IvwSerializer& s) const {
    IVW_UNUSED_PARAM(s);
}

void UVec3MetaData::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
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
FloatMetaData* FloatMetaData::clone() const{
    return new FloatMetaData(*this);
}
void FloatMetaData::serialize(IvwSerializer& s) const {
    IVW_UNUSED_PARAM(s);
}

void FloatMetaData::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
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
Vec2MetaData* Vec2MetaData::clone() const{
    return new Vec2MetaData(*this);
}
void Vec2MetaData::serialize(IvwSerializer& s) const {
    IVW_UNUSED_PARAM(s);
}

void Vec2MetaData::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
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
Vec3MetaData* Vec3MetaData::clone() const{
    return new Vec3MetaData(*this);
}
void Vec3MetaData::serialize(IvwSerializer& s) const {
    IVW_UNUSED_PARAM(s);
}

void Vec3MetaData::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
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
StringMetaData* StringMetaData::clone() const{
    return new StringMetaData(*this);
}
void StringMetaData::serialize(IvwSerializer& s) const {
    IVW_UNUSED_PARAM(s);
}

void StringMetaData::deserialize(IvwDeserializer& d) {
    IVW_UNUSED_PARAM(d);
}

/*---------------------------------------------------------------------*/

} // namespace
