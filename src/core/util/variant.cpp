/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/core/util/variant.h>

#include <sstream>

#define IVEC2STRINGFORMAT "( %i %i )"
#define IVEC3STRINGFORMAT "( %i %i %i )"
#define IVEC4STRINGFORMAT "( %i %i %i %i )"
#define VEC2STRINGFORMAT "( %f %f )"
#define VEC3STRINGFORMAT "( %f %f %f )"
#define VEC4STRINGFORMAT "( %f %f %f %f )"
#define DVEC2STRINGFORMAT "( %f %f )"
#define DVEC3STRINGFORMAT "( %f %f %f )"
#define DVEC4STRINGFORMAT "( %f %f %f %f )"
#define MAT2STRINGFORMAT "( ( %f %f ) ( %f %f ) )"
#define MAT3STRINGFORMAT "( ( %f %f %f ) ( %f %f %f ) ( %f %f %f ) )"
#define MAT4STRINGFORMAT "( ( %f %f %f %f ) ( %f %f %f %f ) ( %f %f %f %f ) ( %f %f %f %f ) )"

#define VALUETOSTRINGBUFFERSIZE 2048

namespace inviwo {

static const int canConvertMatrix[Variant::VariantTypeLastBaseType + 1] =
{
    // Don't change order

    0,

    1 << Variant::VariantTypeDouble     | 1 << Variant::VariantTypeFloat    | 1 << Variant::VariantTypeInteger  |
    1 << Variant::VariantTypeLong       | 1 << Variant::VariantTypeString   | 1 << Variant::VariantTypeInt64,

    1 << Variant::VariantTypeBool       | 1 << Variant::VariantTypeFloat    | 1 << Variant::VariantTypeInteger  |
    1 << Variant::VariantTypeLong       | 1 << Variant::VariantTypeString   | 1 << Variant::VariantTypeInt64,

    1 << Variant::VariantTypeBool       | 1 << Variant::VariantTypeDouble   | 1 << Variant::VariantTypeInteger  |
    1 << Variant::VariantTypeLong       | 1 << Variant::VariantTypeString   | 1 << Variant::VariantTypeInt64,

    1 << Variant::VariantTypeBool       | 1 << Variant::VariantTypeDouble   | 1 << Variant::VariantTypeFloat    |
    1 << Variant::VariantTypeLong       | 1 << Variant::VariantTypeString   | 1 << Variant::VariantTypeInt64,

    1 << Variant::VariantTypeBool       | 1 << Variant::VariantTypeDouble   | 1 << Variant::VariantTypeFloat    |
    1 << Variant::VariantTypeInteger    | 1 << Variant::VariantTypeString   | 1 << Variant::VariantTypeInt64,

    1 << Variant::VariantTypeBool       | 1 << Variant::VariantTypeDouble   | 1 << Variant::VariantTypeFloat    |
    1 << Variant::VariantTypeInteger    | 1 << Variant::VariantTypeLong     | 1 << Variant::VariantTypeIVec2    |
    1 << Variant::VariantTypeIVec3      | 1 << Variant::VariantTypeIVec4    | 1 << Variant::VariantTypeVec2     |
    1 << Variant::VariantTypeVec3       | 1 << Variant::VariantTypeVec4     | 1 << Variant::VariantTypeDVec2    |
    1 << Variant::VariantTypeDVec3      | 1 << Variant::VariantTypeDVec4    | 1 << Variant::VariantTypeMat2     |
    1 << Variant::VariantTypeMat3       | 1 << Variant::VariantTypeMat4     | 1 << Variant::VariantTypeInt64,

    1 << Variant::VariantTypeIVec3      | 1 << Variant::VariantTypeIVec4    | 1 << Variant::VariantTypeVec2     |
    1 << Variant::VariantTypeVec3       | 1 << Variant::VariantTypeVec4     | 1 << Variant::VariantTypeDVec2    |
    1 << Variant::VariantTypeDVec3      | 1 << Variant::VariantTypeDVec4    | 1 << Variant::VariantTypeString   ,

    1 << Variant::VariantTypeIVec2      | 1 << Variant::VariantTypeIVec4    | 1 << Variant::VariantTypeVec2     |
    1 << Variant::VariantTypeVec3       | 1 << Variant::VariantTypeVec4     | 1 << Variant::VariantTypeDVec2    |
    1 << Variant::VariantTypeDVec3      | 1 << Variant::VariantTypeDVec4    | 1 << Variant::VariantTypeString   ,

    1 << Variant::VariantTypeIVec2      | 1 << Variant::VariantTypeIVec3    | 1 << Variant::VariantTypeVec2     |
    1 << Variant::VariantTypeVec3       | 1 << Variant::VariantTypeVec4     | 1 << Variant::VariantTypeDVec2    |
    1 << Variant::VariantTypeDVec3      | 1 << Variant::VariantTypeDVec4    | 1 << Variant::VariantTypeString   ,

    1 << Variant::VariantTypeIVec2      | 1 << Variant::VariantTypeIVec3    | 1 << Variant::VariantTypeIVec4    |
    1 << Variant::VariantTypeVec3       | 1 << Variant::VariantTypeVec4     | 1 << Variant::VariantTypeDVec2    |
    1 << Variant::VariantTypeDVec3      | 1 << Variant::VariantTypeDVec4    | 1 << Variant::VariantTypeString   ,

    1 << Variant::VariantTypeIVec2      | 1 << Variant::VariantTypeIVec3    | 1 << Variant::VariantTypeIVec4    |
    1 << Variant::VariantTypeVec2       | 1 << Variant::VariantTypeVec4     | 1 << Variant::VariantTypeDVec2    |
    1 << Variant::VariantTypeDVec3      | 1 << Variant::VariantTypeDVec4    | 1 << Variant::VariantTypeString   ,

    1 << Variant::VariantTypeIVec2      | 1 << Variant::VariantTypeIVec3    | 1 << Variant::VariantTypeIVec4    |
    1 << Variant::VariantTypeVec2       | 1 << Variant::VariantTypeVec3     | 1 << Variant::VariantTypeDVec2    |
    1 << Variant::VariantTypeDVec3      | 1 << Variant::VariantTypeDVec4    | 1 << Variant::VariantTypeString   ,

    1 << Variant::VariantTypeIVec2      | 1 << Variant::VariantTypeIVec3    | 1 << Variant::VariantTypeIVec4    |
    1 << Variant::VariantTypeVec2       | 1 << Variant::VariantTypeVec3     | 1 << Variant::VariantTypeVec4     |
    1 << Variant::VariantTypeDVec3      | 1 << Variant::VariantTypeDVec4    | 1 << Variant::VariantTypeString   ,

    1 << Variant::VariantTypeIVec2      | 1 << Variant::VariantTypeIVec3    | 1 << Variant::VariantTypeIVec4    |
    1 << Variant::VariantTypeVec2       | 1 << Variant::VariantTypeVec3     | 1 << Variant::VariantTypeVec4     |
    1 << Variant::VariantTypeDVec2      | 1 << Variant::VariantTypeDVec4    | 1 << Variant::VariantTypeString   ,

    1 << Variant::VariantTypeIVec2      | 1 << Variant::VariantTypeIVec3    | 1 << Variant::VariantTypeIVec4    |
    1 << Variant::VariantTypeVec2       | 1 << Variant::VariantTypeVec3     | 1 << Variant::VariantTypeVec4     |
    1 << Variant::VariantTypeDVec2      | 1 << Variant::VariantTypeDVec3    | 1 << Variant::VariantTypeString   ,

    1 << Variant::VariantTypeString,

    1 << Variant::VariantTypeString,

    1 << Variant::VariantTypeString,

    1 << Variant::VariantTypeString,

    1 << Variant::VariantTypeString,

    1 << Variant::VariantTypeString,

    1 << Variant::VariantTypeBool       | 1 << Variant::VariantTypeDouble   | 1 << Variant::VariantTypeFloat    |
    1 << Variant::VariantTypeInteger   | 1 << Variant::VariantTypeLong      | 1 << Variant::VariantTypeString,

    0
};


template<class T>
const std::string Variant::toString(const T& value) const {
    std::stringstream s;
    std::string result;
    s << value;
    s >> result;
    return result;
}

Variant::Variant()
    : IvwSerializable()
    , value_(0)
    , currentType_(VariantTypeInvalid)
{}

Variant::Variant(const Variant& obj)
    : IvwSerializable()
    , value_(0)
{
    switch (obj.getType()) {
        case VariantTypeInvalid:
            currentType_ = VariantTypeInvalid;
            break;

        case VariantTypeBool:
            set<bool>(obj.getBool(), VariantTypeBool);
            break;

        case VariantTypeDouble:
            set<double>(obj.getDouble(), VariantTypeDouble);
            break;

        case VariantTypeFloat:
            set<float>(obj.getFloat(), VariantTypeFloat);
            break;

        case VariantTypeInteger:
            set<int>(obj.getInt(), VariantTypeInteger);
            break;

        case VariantTypeLong:
            set<long>(obj.getLong(), VariantTypeLong);
            break;

        case VariantTypeString:
            set<std::string>(obj.getString(), VariantTypeString);
            break;

        case VariantTypeIVec2:
            set<ivec2>(obj.getIVec2(), VariantTypeIVec2);
            break;

        case VariantTypeIVec3:
            set<ivec3>(obj.getIVec3(), VariantTypeIVec3);
            break;

        case VariantTypeIVec4:
            set<ivec4>(obj.getIVec4(), VariantTypeIVec4);
            break;

        case VariantTypeVec2:
            set<vec2>(obj.getVec2(), VariantTypeVec2);
            break;

        case VariantTypeVec3:
            set<vec3>(obj.getVec3(), VariantTypeVec3);
            break;

        case VariantTypeVec4:
            set<vec4>(obj.getVec4(), VariantTypeVec4);
            break;

        case VariantTypeDVec2:
            set<dvec2>(obj.getDVec2(), VariantTypeDVec2);
            break;

        case VariantTypeDVec3:
            set<dvec3>(obj.getDVec3(), VariantTypeDVec3);
            break;

        case VariantTypeDVec4:
            set<dvec4>(obj.getDVec4(), VariantTypeDVec4);
            break;

        case VariantTypeMat2:
            set<mat2>(obj.getMat2(), VariantTypeMat2);
            break;

        case VariantTypeMat3:
            set<mat3>(obj.getMat3(), VariantTypeMat3);
            break;

        case VariantTypeMat4:
            set<mat4>(obj.getMat4(), VariantTypeMat4);
            break;

        case VariantTypeDMat2:
            set<dmat2>(obj.getDMat2(), VariantTypeDMat2);
            break;

        case VariantTypeDMat3:
            set<dmat3>(obj.getDMat3(), VariantTypeDMat3);
            break;

        case VariantTypeDMat4:
            set<dmat4>(obj.getDMat4(), VariantTypeDMat4);
            break;

        case VariantTypeInt64:
            set<long>(obj.getInt64(), VariantTypeInt64);
            break;

        default:
            break;
    }

    if (obj.getType() >= VariantTypeUserType) {
        currentType_ = obj.currentType_;
        value_ = obj.value_;
    }
}

Variant::Variant(bool value) : value_(0), currentType_(VariantTypeBool) {
    set<bool>(value, VariantTypeBool);
}

Variant::Variant(double value) : value_(0), currentType_(VariantTypeDouble) {
    set<double>(value, VariantTypeDouble);
}

Variant::Variant(float value) : value_(0), currentType_(VariantTypeFloat) {
    set<float>(value, VariantTypeFloat);
}

Variant::Variant(int value) : value_(0), currentType_(VariantTypeInteger) {
    set<int>(value, VariantTypeInteger);
}

Variant::Variant(long value) : value_(0), currentType_(VariantTypeLong) {
    set<long>(value, VariantTypeLong);
}

Variant::Variant(const char* value) : value_(0), currentType_(VariantTypeString) {
    set<std::string>(std::string(value), VariantTypeString);
}

Variant::Variant(const std::string& value) : value_(0), currentType_(VariantTypeString) {
    set<std::string>(value, VariantTypeString);
}

Variant::Variant(const ivec2& value) : value_(0), currentType_(VariantTypeIVec2) {
    set<ivec2>(value, VariantTypeIVec2);
}

Variant::Variant(const ivec3& value) : value_(0), currentType_(VariantTypeIVec3) {
    set<ivec3>(value, VariantTypeIVec3);
}

Variant::Variant(const ivec4& value) : value_(0), currentType_(VariantTypeIVec4) {
    set<ivec4>(value, VariantTypeIVec4);
}

Variant::Variant(const vec2& value) : value_(0), currentType_(VariantTypeVec2) {
    set<vec2>(value, VariantTypeVec2);
}

Variant::Variant(const vec3& value) : value_(0), currentType_(VariantTypeVec3) {
    set<vec3>(value, VariantTypeVec3);
}

Variant::Variant(const vec4& value) : value_(0), currentType_(VariantTypeVec4) {
    set<vec4>(value, VariantTypeVec4);
}

Variant::Variant(const dvec2& value) : value_(0), currentType_(VariantTypeDVec2) {
    set<dvec2>(value, VariantTypeDVec2);
}

Variant::Variant(const dvec3& value) : value_(0), currentType_(VariantTypeDVec3) {
    set<dvec3>(value, VariantTypeDVec3);
}

Variant::Variant(const dvec4& value) : value_(0), currentType_(VariantTypeDVec4) {
    set<dvec4>(value, VariantTypeDVec4);
}

Variant::Variant(const mat2& value) : value_(0), currentType_(VariantTypeMat2) {
    set<mat2>(value, VariantTypeMat2);
}

Variant::Variant(const mat3& value) : value_(0), currentType_(VariantTypeMat3) {
    set<mat3>(value, VariantTypeMat3);
}

Variant::Variant(const mat4& value) : value_(0), currentType_(VariantTypeMat4) {
    set<mat4>(value, VariantTypeMat4);
}
Variant::Variant(const dmat2& value) : value_(0), currentType_(VariantTypeDMat2) {
    set<dmat2>(value, VariantTypeDMat2);
}

Variant::Variant(const dmat3& value) : value_(0), currentType_(VariantTypeDMat3) {
    set<dmat3>(value, VariantTypeDMat3);
}

Variant::Variant(const dmat4& value) : value_(0), currentType_(VariantTypeDMat4) {
    set<dmat4>(value, VariantTypeDMat4);
}

Variant::Variant(glm::i64 value) : value_(0), currentType_(VariantTypeInt64) {
    set<glm::i64>(value, VariantTypeInt64);
}

Variant::Variant(const VariantType& type) : value_(0), currentType_(type) {
}

Variant Variant::deepCopy() const {
    switch (currentType_) {
        case VariantTypeInvalid:
            throw Exception("Tried to create a deep copy of an invalid variant");

        default:
            return Variant(*this);
    }
}

Variant::~Variant() {
    deleteValue();
}

void Variant::deleteValue() {
    if (value_ != 0) {
        switch (currentType_) {
            case VariantTypeInvalid:
                break;

            case VariantTypeBool:
                delete static_cast<bool*>(value_);
                break;

            case VariantTypeDouble:
                delete static_cast<double*>(value_);
                break;

            case VariantTypeFloat:
                delete static_cast<float*>(value_);
                break;

            case VariantTypeInteger:
                delete static_cast<int*>(value_);
                break;

            case VariantTypeLong:
                delete static_cast<long*>(value_);
                break;

            case VariantTypeString:
                delete static_cast<std::string*>(value_);
                break;

            case VariantTypeIVec2:
                delete static_cast<ivec2*>(value_);
                break;

            case VariantTypeIVec3:
                delete static_cast<ivec3*>(value_);
                break;

            case VariantTypeIVec4:
                delete static_cast<ivec4*>(value_);
                break;

            case VariantTypeVec2:
                delete static_cast<vec2*>(value_);
                break;

            case VariantTypeVec3:
                delete static_cast<vec3*>(value_);
                break;

            case VariantTypeVec4:
                delete static_cast<vec4*>(value_);
                break;

            case VariantTypeDVec2:
                delete static_cast<dvec2*>(value_);
                break;

            case VariantTypeDVec3:
                delete static_cast<dvec3*>(value_);
                break;

            case VariantTypeDVec4:
                delete static_cast<dvec4*>(value_);
                break;

            case VariantTypeMat2:
                delete static_cast<mat2*>(value_);
                break;

            case VariantTypeMat3:
                delete static_cast<mat3*>(value_);
                break;

            case VariantTypeMat4:
                delete static_cast<mat4*>(value_);
                break;
            
            case VariantTypeDMat2:
                delete static_cast<dmat2*>(value_);
                break;

            case VariantTypeDMat3:
                delete static_cast<dmat3*>(value_);
                break;

            case VariantTypeDMat4:
                delete static_cast<dmat4*>(value_);
                break;

            case VariantTypeInt64:
                delete static_cast<glm::i64*>(value_);
                break;

            default:
                break;
        }

        value_ = 0;
    }
}

Variant::VariantType Variant::getType() const {
    return currentType_;
}

bool Variant::isValid() const {
    return getType() != VariantTypeInvalid;
}

std::string Variant::typeToName(VariantType type) {
    if (type > VariantTypeUserType)
        return "user-defined";
    else {
        switch (type) {
            case VariantTypeInvalid:
                return "<undefined>";
                break;

            case VariantTypeBool:
                return "bool";
                break;

            case VariantTypeDouble:
                return "double";
                break;

            case VariantTypeFloat:
                return "float";
                break;

            case VariantTypeInteger:
                return "integer";
                break;

            case VariantTypeLong:
                return "long";
                break;

            case VariantTypeString:
                return "string";
                break;

            case VariantTypeIVec2:
                return "ivec2";
                break;

            case VariantTypeIVec3:
                return "ivec3";
                break;

            case VariantTypeIVec4:
                return "ivec4";
                break;

            case VariantTypeVec2:
                return "vec2";
                break;

            case VariantTypeVec3:
                return "vec3";
                break;

            case VariantTypeVec4:
                return "vec4";
                break;

            case VariantTypeDVec2:
                return "dvec2";
                break;

            case VariantTypeDVec3:
                return "dvec3";
                break;

            case VariantTypeDVec4:
                return "dvec4";
                break;

            case VariantTypeMat2:
                return "mat2";
                break;

            case VariantTypeMat3:
                return "mat3";
                break;

            case VariantTypeMat4:
                return "mat4";
                break;

            case VariantTypeDMat2:
                return "dmat2";
                break;

            case VariantTypeDMat3:
                return "dmat3";
                break;

            case VariantTypeDMat4:
                return "dmat4";
                break;

            case VariantTypeInt64:
                return "int64";
                break;

            default:
                return "<undefined>";
        }
    }
}

Variant::VariantType Variant::nameToType(const std::string& typeName) {
    if (typeName == "<undefined>")
        return VariantTypeInvalid;
    else if (typeName == "bool")
        return VariantTypeBool;
    else if (typeName == "double")
        return VariantTypeDouble;
    else if (typeName == "float")
        return VariantTypeFloat;
    else if (typeName == "integer")
        return VariantTypeInteger;
    else if (typeName == "long")
        return VariantTypeLong;
    else if (typeName == "string")
        return VariantTypeString;
    else if (typeName == "ivec2")
        return VariantTypeIVec2;
    else if (typeName == "ivec3")
        return VariantTypeIVec3;
    else if (typeName == "ivec4")
        return VariantTypeIVec4;
    else if (typeName == "vec2")
        return VariantTypeVec2;
    else if (typeName == "vec3")
        return VariantTypeVec3;
    else if (typeName == "vec4")
        return VariantTypeVec4;
    else if (typeName == "dvec2")
        return VariantTypeDVec2;
    else if (typeName == "dvec3")
        return VariantTypeDVec3;
    else if (typeName == "dvec4")
        return VariantTypeDVec4;
    else if (typeName == "mat2")
        return VariantTypeMat2;
    else if (typeName == "mat3")
        return VariantTypeMat3;
    else if (typeName == "mat4")
        return VariantTypeMat4;
    else if (typeName == "dmat2")
        return VariantTypeDMat2;
    else if (typeName == "dmat3")
        return VariantTypeDMat3;
    else if (typeName == "dmat4")
        return VariantTypeDMat4;
    else if (typeName == "int64")
        return VariantTypeInt64;
    else if (typeName > "user-defined")
        return VariantTypeUserType;
    else
        return VariantTypeInvalid;
}

bool Variant::canConvert(VariantType t) const {
    return canConvert(currentType_, t);
}

bool Variant::canConvert(int t) const {
    return canConvert(currentType_, VariantType(t));
}

bool Variant::canConvert(VariantType t, VariantType s) {
    if (t == VariantTypeInvalid || s == VariantTypeInvalid)
        return false;
    else {
        if (t == s)
            return true;
        else if (t >= Variant::VariantTypeUserType || s >= Variant::VariantTypeUserType)
            return t == s;
        else
            return (canConvertMatrix[t] & (1 << s)) != 0; // != 0 gets rid of performance warning
    }
}

bool Variant::canConvert(VariantType t, int s) {
    return canConvert(t, VariantType(s));
}

bool Variant::canConvert(int t, VariantType s) {
    return canConvert(VariantType(t), s);
}

bool Variant::canConvert(int t, int s) {
    return canConvert(VariantType(t), VariantType(s));
}

bool Variant::getBool() const {
    switch (currentType_) {
        case VariantTypeBool:
            return VP(bool);

        case VariantTypeDouble:
            return static_cast<bool>(VP(double)!= 0); // != 0 gets rid of performance warning

        case VariantTypeInteger:
            return static_cast<bool>(VP(int)!= 0); // != 0 gets rid of performance warning

        case VariantTypeFloat:
            return static_cast<bool>(VP(float)!= 0); // != 0 gets rid of performance warning

        case VariantTypeString:
        {
            std::string s = VP(std::string);

            for (unsigned int i = 0; i < s.length(); ++i)
                s[i] = static_cast<char>(tolower(s[i]));

            if (s == "1" || s == "true")
                return true;

            if (s == "0" || s == "false")
                return false;

            throw Exception("String->Bool conversion failed");
        }

        case VariantTypeLong:
            return static_cast<bool>(VP(long)!= 0); // != 0 gets rid of performance warning

        case VariantTypeInt64:
            return static_cast<bool>(VP(glm::i64)!= 0); // != 0 gets rid of performance warning

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to bool not implemented");
            break;
    }

    return false;
}


double Variant::getDouble() const {
    switch (currentType_) {
        case VariantTypeBool:
            return static_cast<double>(VP(bool));

        case VariantTypeDouble:
            return VP(double);

        case VariantTypeFloat:
            return static_cast<double>(VP(float));

        case VariantTypeInteger:
            return static_cast<double>(VP(int));

        case VariantTypeLong:
            return static_cast<double>(VP(long));

        case VariantTypeString:
        {
            std::stringstream s(VP(std::string));
            double result;

            if ((s >> result).fail())
                throw Exception("String->Double conversion failed");

            return result;
        }

        case VariantTypeInt64:
            return static_cast<double>(VP(glm::i64)!= 0); // != 0 gets rid of performance warning

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to double not implemented");
            break;
    }
}

float Variant::getFloat() const {
    switch (currentType_) {
        case VariantTypeBool:
            return static_cast<float>(VP(bool));

        case VariantTypeDouble:
            return static_cast<float>(VP(double));

        case VariantTypeFloat:
            return VP(float);

        case VariantTypeInteger:
            return static_cast<float>(VP(int));

        case VariantTypeLong:
            return static_cast<float>(VP(long));

        case VariantTypeString:
        {
            std::stringstream s(VP(std::string));
            float result;

            if ((s >> result).fail())
                throw Exception("String->Double conversion failed");

            return result;
        }

        case VariantTypeInt64:
            return static_cast<float>(VP(glm::i64)!= 0); // != 0 gets rid of performance warning

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to float not implemented");
            break;
    }
}

int Variant::getInt() const {
    switch (currentType_) {
        case VariantTypeBool:
            return static_cast<int>(VP(bool));

        case VariantTypeDouble:
            return static_cast<int>(VP(double));

        case VariantTypeFloat:
            return static_cast<int>(VP(float));

        case VariantTypeInteger:
            return VP(int);

        case VariantTypeLong:
            return static_cast<int>(VP(long));

        case VariantTypeString:
        {
            std::stringstream s(VP(std::string));
            int result;

            if ((s >> result).fail())
                throw Exception("String->Int conversion failed");

            return result;
        }

        case VariantTypeInt64:
            return static_cast<int>(VP(glm::i64)!= 0); // != 0 gets rid of performance warning

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to integer not implemented");
            break;
    }
}

long Variant::getLong() const {
    switch (currentType_) {
        case VariantTypeBool:
            return static_cast<long>(VP(bool));

        case VariantTypeDouble:
            return static_cast<long>(VP(double));

        case VariantTypeFloat:
            return static_cast<long>(VP(float));

        case VariantTypeInteger:
            return static_cast<long>(VP(int));

        case VariantTypeLong:
            return VP(long);

        case VariantTypeString:
        {
            std::stringstream s(VP(std::string));
            long result;

            if ((s >> result).fail())
                throw Exception("String->Int conversion failed");

            return result;
        }

        case VariantTypeInt64:
            return static_cast<long>(VP(glm::i64)!= 0); // != 0 gets rid of performance warning

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to long not implemented");
            break;
    }
}

std::string Variant::getString() const {
    switch (currentType_) {
        case VariantTypeBool:
            return VP(bool) ? "true" : "false";

        case VariantTypeDouble:
            return toString<double>(VP(double));

        case VariantTypeFloat:
            return toString<float>(VP(float));

        case VariantTypeInteger:
            return toString<int>(VP(int));

        case VariantTypeLong:
            return toString<long>(VP(long));

        case VariantTypeString:
            return VP(std::string);

        case VariantTypeIVec2:
        {
            char result[VALUETOSTRINGBUFFERSIZE];
            ivec2 v = VP(ivec2);
            sprintf(result, IVEC2STRINGFORMAT, v.x, v.y);
            return std::string(result);
        }

        case VariantTypeIVec3:
        {
            char result[VALUETOSTRINGBUFFERSIZE];
            ivec3 v = VP(ivec3);
            sprintf(result, IVEC3STRINGFORMAT, v.x, v.y, v.z);
            return std::string(result);
        }

        case VariantTypeIVec4:
        {
            char result[VALUETOSTRINGBUFFERSIZE];
            ivec4 v = VP(ivec4);
            sprintf(result, IVEC4STRINGFORMAT, v.x, v.y, v.z, v.w);
            return std::string(result);
        }

        case VariantTypeVec2:
        {
            char result[VALUETOSTRINGBUFFERSIZE];
            vec2 v = VP(vec2);
            sprintf(result, VEC2STRINGFORMAT, v.x, v.y);
            return std::string(result);
        }

        case VariantTypeVec3:
        {
            char result[VALUETOSTRINGBUFFERSIZE];
            vec3 v = VP(vec3);
            sprintf(result, VEC3STRINGFORMAT, v.x, v.y, v.z);
            return std::string(result);
        }

        case VariantTypeVec4:
        {
            char result[VALUETOSTRINGBUFFERSIZE];
            vec4 v = VP(vec4);
            sprintf(result, VEC4STRINGFORMAT, v.x, v.y, v.z, v.w);
            return std::string(result);
        }

        case VariantTypeDVec2:
        {
            char result[VALUETOSTRINGBUFFERSIZE];
            dvec2 v = VP(dvec2);
            sprintf(result, DVEC2STRINGFORMAT, v.x, v.y);
            return std::string(result);
        }

        case VariantTypeDVec3:
        {
            char result[VALUETOSTRINGBUFFERSIZE];
            dvec3 v = VP(dvec3);
            sprintf(result, DVEC3STRINGFORMAT, v.x, v.y, v.z);
            return std::string(result);
        }

        case VariantTypeDVec4:
        {
            char result[VALUETOSTRINGBUFFERSIZE];
            dvec4 v = VP(dvec4);
            sprintf(result, DVEC4STRINGFORMAT, v.x, v.y, v.z, v.w);
            return std::string(result);
        }

        case VariantTypeMat2:
        {
            char result[VALUETOSTRINGBUFFERSIZE];
            mat2 m = VP(mat2);
            sprintf(result, MAT2STRINGFORMAT, m[0][0], m[0][1],
                    m[1][0], m[1][1]);
            return std::string(result);
        }

        case VariantTypeMat3:
        {
            char result[VALUETOSTRINGBUFFERSIZE];
            mat3 m = VP(mat3);
            sprintf(result, MAT3STRINGFORMAT, m[0][0], m[0][1], m[0][2],
                    m[1][0], m[1][1], m[1][2],
                    m[2][0], m[2][1], m[2][2]);
            return std::string(result);
        }

        case VariantTypeMat4:
        {
            char result[VALUETOSTRINGBUFFERSIZE];
            mat4 m = VP(mat4);
            sprintf(result, MAT4STRINGFORMAT, m[0][0], m[0][1], m[0][2], m[0][3],
                    m[1][0], m[1][1], m[1][2], m[1][3],
                    m[2][0], m[2][1], m[2][2], m[2][3],
                    m[3][0], m[3][1], m[3][2], m[3][3]);
            return std::string(result);
        }

        case VariantTypeDMat2:
        {
                                char result[VALUETOSTRINGBUFFERSIZE];
                                dmat2 m = VP(dmat2);
                                sprintf(result, MAT2STRINGFORMAT, m[0][0], m[0][1],
                                        m[1][0], m[1][1]);
                                return std::string(result);
        }

        case VariantTypeDMat3:
        {
                                char result[VALUETOSTRINGBUFFERSIZE];
                                dmat3 m = VP(dmat3);
                                sprintf(result, MAT3STRINGFORMAT, m[0][0], m[0][1], m[0][2],
                                        m[1][0], m[1][1], m[1][2],
                                        m[2][0], m[2][1], m[2][2]);
                                return std::string(result);
        }

        case VariantTypeDMat4:
        {
                                char result[VALUETOSTRINGBUFFERSIZE];
                                dmat4 m = VP(dmat4);
                                sprintf(result, MAT4STRINGFORMAT, m[0][0], m[0][1], m[0][2], m[0][3],
                                        m[1][0], m[1][1], m[1][2], m[1][3],
                                        m[2][0], m[2][1], m[2][2], m[2][3],
                                        m[3][0], m[3][1], m[3][2], m[3][3]);
                                return std::string(result);
        }

        case VariantTypeInt64:
            return toString<glm::i64>(VP(glm::i64));
        
        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to string not implemented");
            break;
    }
}

glm::i64 Variant::getInt64() const {
    switch (currentType_) {
    case VariantTypeBool:
        return static_cast<glm::i64>(VP(bool));

    case VariantTypeDouble:
        return static_cast<glm::i64>(VP(double));

    case VariantTypeFloat:
        return static_cast<glm::i64>(VP(float));

    case VariantTypeInteger:
        return static_cast<glm::i64>(VP(int));

    case VariantTypeLong:
        return static_cast<glm::i64>(VP(long));

    case VariantTypeString:
        {
            std::stringstream s(VP(std::string));
            glm::i64 result;

            if ((s >> result).fail())
                throw Exception("String->Int conversion failed");

            return result;
        }

    case VariantTypeInt64:
        return VP(glm::i64);

    case VariantTypeInvalid:
        throw Exception("");

    default:
        throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to int64 not implemented");
        break;
    }
}

ivec2 Variant::getIVec2() const {
    switch (currentType_) {
        case VariantTypeIVec2:
            return VP(ivec2);

        case VariantTypeIVec3:
            return ivec2(VP(ivec3).x, VP(ivec3).y);

        case VariantTypeIVec4:
            return ivec2(VP(ivec4).x, VP(ivec4).y);

        case VariantTypeVec2:
            return ivec2(static_cast<int>(VP(vec2).x), static_cast<int>(VP(vec2).y));

        case VariantTypeVec3:
            return ivec2(static_cast<int>(VP(vec3).x), static_cast<int>(VP(vec3).y));

        case VariantTypeVec4:
            return ivec2(static_cast<int>(VP(vec4).x), static_cast<int>(VP(vec4).y));

        case VariantTypeDVec2:
            return ivec2(static_cast<int>(VP(dvec2).x), static_cast<int>(VP(dvec2).y));

        case VariantTypeDVec3:
            return ivec2(static_cast<int>(VP(dvec3).x), static_cast<int>(VP(dvec3).y));

        case VariantTypeDVec4:
            return ivec2(static_cast<int>(VP(dvec4).x), static_cast<int>(VP(dvec4).y));

        case VariantTypeString:
        {
            ivec2 result;
            std::string s = VP(std::string);
            sscanf(VP(std::string).c_str(), IVEC2STRINGFORMAT, &result.x, &result.y);
            return result;
        }

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to ivec2 not implemented");
            break;
    }
}

ivec3 Variant::getIVec3() const {
    switch (currentType_) {
        case VariantTypeIVec2:
            return ivec3(VP(ivec2).x, VP(ivec2).y, 0);

        case VariantTypeIVec3:
            return ivec3(static_cast<int>(VP(ivec3).x), static_cast<int>(VP(ivec3).y), static_cast<int>(VP(ivec3).z));

        case VariantTypeIVec4:
            return ivec3(static_cast<int>(VP(ivec4).x), static_cast<int>(VP(ivec4).y), static_cast<int>(VP(ivec4).z));

        case VariantTypeVec2:
            return ivec3(static_cast<int>(VP(vec2).x), static_cast<int>(VP(vec2).y), 0);

        case VariantTypeVec3:
            return ivec3(static_cast<int>(VP(vec3).x), static_cast<int>(VP(vec3).y), static_cast<int>(VP(vec3).z));

        case VariantTypeVec4:
            return ivec3(static_cast<int>(VP(vec4).x), static_cast<int>(VP(vec4).y), static_cast<int>(VP(vec4).z));

        case VariantTypeDVec2:
            return ivec3(static_cast<int>(VP(dvec2).x), static_cast<int>(VP(dvec2).y), 0);

        case VariantTypeDVec3:
            return ivec3(static_cast<int>(VP(dvec3).x), static_cast<int>(VP(dvec3).y), static_cast<int>(VP(dvec3).z));

        case VariantTypeDVec4:
            return ivec3(static_cast<int>(VP(dvec4).x), static_cast<int>(VP(dvec4).y), static_cast<int>(VP(dvec4).z));

        case VariantTypeString:
        {
            ivec3 result;
            sscanf(VP(std::string).c_str(), IVEC3STRINGFORMAT, &result.x, &result.y, &result.z);
            return result;
        }

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to ivec3 not implemented");
            break;
    }
}

ivec4 Variant::getIVec4() const {
    switch (currentType_) {
        case VariantTypeIVec2:
            return ivec4(VP(ivec2).x, VP(ivec2).y, 0, 0);

        case VariantTypeIVec3:
            return ivec4(VP(ivec3).x, VP(ivec3).y, VP(ivec3).z, 0);

        case VariantTypeIVec4:
            return VP(ivec4);

        case VariantTypeVec2:
            return ivec4(static_cast<int>(VP(vec2).x), static_cast<int>(VP(vec2).y), 0, 0);

        case VariantTypeVec3:
            return ivec4(static_cast<int>(VP(vec3).x), static_cast<int>(VP(vec3).y), static_cast<int>(VP(vec3).z), 0);

        case VariantTypeVec4:
            return ivec4(static_cast<int>(VP(vec4).x), static_cast<int>(VP(vec4).y), static_cast<int>(VP(vec4).z), static_cast<int>(VP(vec4).w));

        case VariantTypeDVec2:
            return ivec4(static_cast<int>(VP(dvec2).x), static_cast<int>(VP(dvec2).y), 0, 0);

        case VariantTypeDVec3:
            return ivec4(static_cast<int>(VP(dvec3).x), static_cast<int>(VP(dvec3).y), static_cast<int>(VP(dvec3).z), 0);

        case VariantTypeDVec4:
            return ivec4(static_cast<int>(VP(dvec4).x), static_cast<int>(VP(dvec4).y), static_cast<int>(VP(dvec4).z), static_cast<int>(VP(dvec4).w));

        case VariantTypeString:
        {
            ivec4 result;
            sscanf(VP(std::string).c_str(), IVEC4STRINGFORMAT, &result.x, &result.y, &result.z, &result.w);
            return result;
        }

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to ivec4 not implemented");
            break;
    }
}

vec2 Variant::getVec2() const {
    switch (currentType_) {
        case VariantTypeIVec2:
            return vec2(static_cast<float>(VP(ivec2).x), static_cast<float>(VP(ivec2).y));

        case VariantTypeIVec3:
            return vec2(static_cast<float>(VP(ivec3).x), static_cast<float>(VP(ivec3).y));

        case VariantTypeIVec4:
            return vec2(static_cast<float>(VP(ivec4).x), static_cast<float>(VP(ivec4).y));

        case VariantTypeVec2:
            return VP(vec2);

        case VariantTypeVec3:
            return vec2(static_cast<float>(VP(vec3).x), static_cast<float>(VP(vec3).y));

        case VariantTypeVec4:
            return vec2(static_cast<float>(VP(vec4).x), static_cast<float>(VP(vec4).y));

        case VariantTypeDVec2:
            return vec2(static_cast<float>(VP(dvec2).x), static_cast<float>(VP(dvec2).y));

        case VariantTypeDVec3:
            return vec2(static_cast<float>(VP(dvec3).x), static_cast<float>(VP(dvec3).y));

        case VariantTypeDVec4:
            return vec2(static_cast<float>(VP(dvec4).x), static_cast<float>(VP(dvec4).y));

        case VariantTypeString:
        {
            vec2 result;
            sscanf(VP(std::string).c_str(), VEC2STRINGFORMAT, &result.x, &result.y);
            return result;
        }

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to vec2 not implemented");
            break;
    }
}

vec3 Variant::getVec3() const {
    switch (currentType_) {
        case VariantTypeIVec2:
            return vec3(static_cast<float>(VP(ivec2).x), static_cast<float>(VP(ivec2).y), 0);

        case VariantTypeIVec3:
            return vec3(static_cast<float>(VP(ivec3).x), static_cast<float>(VP(ivec3).y), static_cast<float>(VP(ivec3).z));

        case VariantTypeIVec4:
            return vec3(static_cast<float>(VP(ivec4).x), static_cast<float>(VP(ivec4).y), static_cast<float>(VP(ivec4).z));

        case VariantTypeVec2:
            return vec3(VP(vec2).x, VP(vec2).y, 0);

        case VariantTypeVec3:
            return VP(vec3);

        case VariantTypeVec4:
            return vec3(VP(vec4).x, VP(vec4).y,VP(vec4).z);

        case VariantTypeDVec2:
            return vec3(static_cast<float>(VP(dvec2).x), static_cast<float>(VP(dvec2).y), 0);

        case VariantTypeDVec3:
            return vec3(static_cast<float>(VP(dvec3).x), static_cast<float>(VP(dvec3).y), static_cast<float>(VP(dvec3).z));

        case VariantTypeDVec4:
            return vec3(static_cast<float>(VP(dvec4).x), static_cast<float>(VP(dvec4).y), static_cast<float>(VP(dvec4).z));

        case VariantTypeString:
        {
            vec3 result;
            sscanf(VP(std::string).c_str(), VEC3STRINGFORMAT, &result.x, &result.y, &result.z);
            return result;
        }

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to vec3 not implemented");
            break;
    }
}

vec4 Variant::getVec4() const {
    switch (currentType_) {
        case VariantTypeIVec2:
            return vec4(static_cast<float>(VP(ivec2).x), static_cast<float>(VP(ivec2).y), 0, 0);

        case VariantTypeIVec3:
            return vec4(static_cast<float>(VP(ivec3).x), static_cast<float>(VP(ivec3).y), static_cast<float>(VP(ivec3).z), 0);

        case VariantTypeIVec4:
            return vec4(static_cast<float>(VP(ivec4).x), static_cast<float>(VP(ivec4).y), static_cast<float>(VP(ivec4).z),
                        static_cast<float>(VP(ivec4).w));

        case VariantTypeVec2:
            return vec4(VP(vec2).x, VP(vec2).y, 0, 0);

        case VariantTypeVec3:
            return vec4(VP(vec3).x, VP(vec3).y, VP(vec3).z, 0);

        case VariantTypeVec4:
            return VP(vec4);

        case VariantTypeDVec2:
            return vec4(static_cast<float>(VP(dvec2).x), static_cast<float>(VP(dvec2).y), 0, 0);

        case VariantTypeDVec3:
            return vec4(static_cast<float>(VP(dvec3).x), static_cast<float>(VP(dvec3).y), static_cast<float>(VP(dvec3).z), 0);

        case VariantTypeDVec4:
            return vec4(static_cast<float>(VP(dvec4).x), static_cast<float>(VP(vec4).y), static_cast<float>(VP(dvec4).z),
                        static_cast<float>(VP(dvec4).w));

        case VariantTypeString:
        {
            vec4 result;
            sscanf(VP(std::string).c_str(), VEC4STRINGFORMAT, &result.x, &result.y, &result.z, &result.w);
            return result;
        }

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to vec4 not implemented");
            break;
    }
}

dvec2 Variant::getDVec2() const {
    switch (currentType_) {
        case VariantTypeIVec2:
            return dvec2(static_cast<double>(VP(ivec2).x), static_cast<double>(VP(ivec2).y));

        case VariantTypeIVec3:
            return dvec2(static_cast<double>(VP(ivec3).x), static_cast<double>(VP(ivec3).y));

        case VariantTypeIVec4:
            return dvec2(static_cast<double>(VP(ivec4).x), static_cast<double>(VP(ivec4).y));

        case VariantTypeVec2:
            return dvec2(static_cast<double>(VP(vec2).x), static_cast<double>(VP(vec2).y));

        case VariantTypeVec3:
            return dvec2(static_cast<double>(VP(vec3).x), static_cast<double>(VP(vec3).y));

        case VariantTypeVec4:
            return dvec2(static_cast<double>(VP(vec4).x), static_cast<double>(VP(vec4).y));

        case VariantTypeDVec2:
            return VP(dvec2);

        case VariantTypeDVec3:
            return dvec2(VP(dvec3).x, VP(dvec3).y);

        case VariantTypeDVec4:
            return dvec2(VP(dvec4).x, VP(dvec4).y);

        case VariantTypeString:
        {
            vec2 result;
            sscanf(VP(std::string).c_str(), DVEC2STRINGFORMAT, &result.x, &result.y);
            return dvec2(result);
        }

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to dvec2 not implemented");
            break;
    }
}

dvec3 Variant::getDVec3() const {
    switch (currentType_) {
        case VariantTypeIVec2:
            return dvec3(static_cast<double>(VP(ivec2).x), static_cast<double>(VP(ivec2).y), 0);

        case VariantTypeIVec3:
            return dvec3(static_cast<double>(VP(ivec3).x), static_cast<double>(VP(ivec3).y), static_cast<double>(VP(ivec3).z));

        case VariantTypeIVec4:
            return dvec3(static_cast<double>(VP(ivec4).x), static_cast<double>(VP(ivec4).y), static_cast<double>(VP(ivec4).z));

        case VariantTypeVec2:
            return dvec3(static_cast<double>(VP(vec2).x), static_cast<double>(VP(vec2).y), 0);

        case VariantTypeVec3:
            return dvec3(static_cast<double>(VP(vec3).x), static_cast<double>(VP(vec3).y), static_cast<double>(VP(vec3).z));

        case VariantTypeVec4:
            return dvec3(static_cast<double>(VP(vec4).x), static_cast<double>(VP(vec4).y), static_cast<double>(VP(vec4).z));

        case VariantTypeDVec2:
            return dvec3(VP(dvec2).x, VP(dvec2).y, 0);

        case VariantTypeDVec3:
            return VP(dvec3);

        case VariantTypeDVec4:
            return dvec3(VP(dvec4).x,VP(dvec4).y, VP(dvec4).z);

        case VariantTypeString:
        {
            vec3 result;
            sscanf(VP(std::string).c_str(), DVEC3STRINGFORMAT, &result.x, &result.y, &result.z);
            return dvec3(result);
        }

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to dvec3 not implemented");
            break;
    }

    return dvec3(0.0);
}

dvec4 Variant::getDVec4() const {
    switch (currentType_) {
        case VariantTypeIVec2:
            return dvec4(static_cast<double>(VP(ivec2).x), static_cast<double>(VP(ivec2).y), 0, 0);

        case VariantTypeIVec3:
            return dvec4(static_cast<double>(VP(ivec3).x), static_cast<double>(VP(ivec3).y), static_cast<double>(VP(ivec3).z), 0);

        case VariantTypeIVec4:
            return dvec4(static_cast<double>(VP(ivec4).x), static_cast<double>(VP(ivec4).y), static_cast<double>(VP(ivec4).z),
                         static_cast<double>(VP(ivec4).w));

        case VariantTypeVec2:
            return dvec4(static_cast<double>(VP(vec2).x), static_cast<double>(VP(vec2).y), 0, 0);

        case VariantTypeVec3:
            return dvec4(static_cast<double>(VP(vec3).x), static_cast<double>(VP(vec3).y), static_cast<double>(VP(vec3).z), 0);

        case VariantTypeVec4:
            return dvec4(static_cast<double>(VP(vec4).x), static_cast<double>(VP(vec4).y), static_cast<double>(VP(vec4).z),
                         static_cast<double>(VP(vec4).w));

        case VariantTypeDVec2:
            return dvec4(VP(dvec2).x, VP(dvec2).y, 0, 0);

        case VariantTypeDVec3:
            return dvec4(VP(dvec3).x, VP(dvec3).y, VP(dvec3).z, 0);

        case VariantTypeDVec4:
            return VP(dvec4);

        case VariantTypeString:
        {
            vec4 result;
            sscanf(VP(std::string).c_str(), DVEC4STRINGFORMAT, &result.x, &result.y, &result.z, &result.w);
            return dvec4(result);
        }

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to dvec4 not implemented");
            break;
    }

    return dvec4(0.0);
}

mat2 Variant::getMat2() const {
    switch (currentType_) {
        case VariantTypeMat2:
            return VP(mat2);
            break;

        case VariantTypeString:
        {
            mat2 result;
            sscanf(VP(std::string).c_str(), MAT2STRINGFORMAT, &result[0][0], &result[0][1], &result[1][0], &result[1][1]);
            return result;
        }

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to mat2 not implemented");
            break;
    }

    return mat2(1.0);
}

mat3 Variant::getMat3() const {
    switch (currentType_) {
        case VariantTypeMat3:
            return VP(mat3);
            break;

        case VariantTypeString:
        {
            mat3 result;
            sscanf(VP(std::string).c_str(), MAT3STRINGFORMAT, &result[0][0], &result[0][1], &result[0][2],
                   &result[1][0], &result[1][1], &result[1][2],
                   &result[2][0], &result[2][1], &result[2][2]);
            return result;
        }

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to mat3 not implemented");
            break;
    }

    return mat3(1.0);
}

mat4 Variant::getMat4() const {
    switch (currentType_) {
        case VariantTypeMat4:
            return VP(mat4);
            break;

        case VariantTypeString:
        {
            mat4 result;
            sscanf(VP(std::string).c_str(), MAT4STRINGFORMAT, &result[0][0], &result[0][1], &result[0][2], &result[0][3],
                   &result[1][0], &result[1][1], &result[1][2], &result[1][3],
                   &result[2][0], &result[2][1], &result[2][2], &result[2][3],
                   &result[3][0], &result[3][1], &result[3][2], &result[3][3]);
            return result;
        }

        case VariantTypeInvalid:
            throw Exception("");

        default:
            throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to mat4 not implemented");
            break;
    }

    return mat4(1.0);
}

dmat2 Variant::getDMat2() const {
    switch (currentType_) {
    case VariantTypeDMat2:
        return VP(dmat2);
        break;

    case VariantTypeString:
    {
                              dmat2 result;
                              sscanf(VP(std::string).c_str(), MAT2STRINGFORMAT, &result[0][0], &result[0][1], &result[1][0], &result[1][1]);
                              return result;
    }

    case VariantTypeInvalid:
        throw Exception("");

    default:
        throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to dmat2 not implemented");
        break;
    }

    return dmat2(1.0);
}

dmat3 Variant::getDMat3() const {
    switch (currentType_) {
    case VariantTypeDMat3:
        return VP(dmat3);
        break;

    case VariantTypeString:
    {
                              dmat3 result;
                              sscanf(VP(std::string).c_str(), MAT3STRINGFORMAT, &result[0][0], &result[0][1], &result[0][2],
                                     &result[1][0], &result[1][1], &result[1][2],
                                     &result[2][0], &result[2][1], &result[2][2]);
                              return result;
    }

    case VariantTypeInvalid:
        throw Exception("");

    default:
        throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to dmat3 not implemented");
        break;
    }

    return dmat3(1.0);
}

dmat4 Variant::getDMat4() const {
    switch (currentType_) {
    case VariantTypeDMat4:
        return VP(dmat4);
        break;

    case VariantTypeString:
    {
                              dmat4 result;
                              sscanf(VP(std::string).c_str(), MAT4STRINGFORMAT, &result[0][0], &result[0][1], &result[0][2], &result[0][3],
                                     &result[1][0], &result[1][1], &result[1][2], &result[1][3],
                                     &result[2][0], &result[2][1], &result[2][2], &result[2][3],
                                     &result[3][0], &result[3][1], &result[3][2], &result[3][3]);
                              return result;
    }

    case VariantTypeInvalid:
        throw Exception("");

    default:
        throw Exception("Variant: Conversion from " + typeToName(currentType_) + " to dmat4 not implemented");
        break;
    }

    return dmat4(1.0);
}

void Variant::setBool(const bool& value) {
    set<bool>(value, VariantTypeBool);
}

void Variant::setDouble(const double& value) {
    set<double>(value, VariantTypeDouble);
}

void Variant::setFloat(const float& value) {
    set<float>(value, VariantTypeFloat);
}

void Variant::setInt(const int& value) {
    set<int>(value, VariantTypeInteger);
}

void Variant::setLong(const long& value) {
    set<long>(value, VariantTypeLong);
}

void Variant::setString(const std::string& value) {
    set<std::string>(value, VariantTypeString);
}

void Variant::setIVec2(const ivec2& value) {
    set<ivec2>(value, VariantTypeIVec2);
}

void Variant::setIVec3(const ivec3& value) {
    set<ivec3>(value, VariantTypeIVec3);
}

void Variant::setIVec4(const ivec4& value) {
    set<ivec4>(value, VariantTypeIVec4);
}


void Variant::setVec2(const vec2& value) {
    set<vec2>(value, VariantTypeVec2);
}

void Variant::setVec3(const vec3& value) {
    set<vec3>(value, VariantTypeVec3);
}

void Variant::setVec4(const vec4& value) {
    set<vec4>(value, VariantTypeVec4);
}

void Variant::setDVec2(const dvec2& value) {
    set<dvec2>(value, VariantTypeDVec2);
}

void Variant::setDVec3(const dvec3& value) {
    set<dvec3>(value, VariantTypeDVec3);
}

void Variant::setDVec4(const dvec4& value) {
    set<dvec4>(value, VariantTypeDVec4);
}

void Variant::setMat2(const mat2& value) {
    set<mat2>(value, VariantTypeMat2);
}

void Variant::setMat3(const mat3& value) {
    set<mat3>(value, VariantTypeMat3);
}

void Variant::setMat4(const mat4& value) {
    set<mat4>(value, VariantTypeMat4);
}

void Variant::setDMat2(const dmat2& value) {
    set<dmat2>(value, VariantTypeDMat2);
}

void Variant::setDMat3(const dmat3& value) {
    set<dmat3>(value, VariantTypeDMat3);
}

void Variant::setDMat4(const dmat4& value) {
    set<dmat4>(value, VariantTypeDMat4);
}

void Variant::setInt64(const glm::i64& value) {
    set<glm::i64>(value, VariantTypeInt64);
}

void Variant::serialize(IvwSerializer& s) const {
    s.serialize("VariantType", typeToName(currentType_));

    switch (currentType_) {
        case VariantTypeBool:
            s.serialize("value", getBool());
            break;

        case VariantTypeDouble:
            s.serialize("value", getDouble());
            break;

        case VariantTypeFloat:
            s.serialize("value", getFloat());
            break;

        case VariantTypeInteger:
            s.serialize("value", getInt());
            break;

        case VariantTypeLong:
            s.serialize("value", getLong());
            break;

        case VariantTypeString:
            s.serialize("value", getString());
            break;

        case VariantTypeIVec2:
            s.serialize("value", getIVec2());
            break;

        case VariantTypeIVec3:
            s.serialize("value", getIVec3());
            break;

        case VariantTypeIVec4:
            s.serialize("value", getIVec4());
            break;

        case VariantTypeVec2:
            s.serialize("value", getVec2());
            break;

        case VariantTypeVec3:
            s.serialize("value", getVec3());
            break;

        case VariantTypeVec4:
            s.serialize("value", getVec4());
            break;

        case VariantTypeDVec2:
            s.serialize("value", getDVec2());
            break;

        case VariantTypeDVec3:
            s.serialize("value", getDVec3());
            break;

        case VariantTypeDVec4:
            s.serialize("value", getDVec4());
            break;

        case VariantTypeMat2:
            s.serialize("value", getMat2());
            break;

        case VariantTypeMat3:
            s.serialize("value", getMat3());
            break;

        case VariantTypeMat4:
            s.serialize("value", getMat4());
            break;

        case VariantTypeDMat2:
            s.serialize("value", getDMat2());
            break;

        case VariantTypeDMat3:
            s.serialize("value", getDMat3());
            break;

        case VariantTypeDMat4:
            s.serialize("value", getDMat4());
            break;

        case VariantTypeInt64:
            s.serialize("value", getInt64());
            break;

        case VariantTypeInvalid:
            throw Exception("Tried to serialize an invalid variant");
            break;

        default:
            throw Exception("Tried to serialize a user-defined variant");
            break;
    }
}

void Variant::deserialize(IvwDeserializer& d) {
    std::string typeString;
    d.deserialize("VariantType", typeString);
    currentType_ = nameToType(typeString);

    switch (currentType_) {
        case VariantTypeBool:
        {
            bool value;
            d.deserialize("value", value);
            setBool(value);
            break;
        }

        case VariantTypeDouble:
        {
            double value;
            d.deserialize("value", value);
            setDouble(value);
            break;
        }

        case VariantTypeFloat:
        {
            float value;
            d.deserialize("value", value);
            setFloat(value);
            break;
        }

        case VariantTypeInteger:
        {
            int value;
            d.deserialize("value", value);
            setInt(value);
            break;
        }

        case VariantTypeLong:
        {
            long value;
            d.deserialize("value", value);
            setLong(value);
            break;
        }

        case VariantTypeString:
        {
            std::string value;
            d.deserialize("value", value);
            setString(value);
            break;
        }

        case VariantTypeIVec2:
        {
            ivec2 value;
            d.deserialize("value", value);
            setIVec2(value);
            break;
        }

        case VariantTypeIVec3:
        {
            ivec3 value;
            d.deserialize("value", value);
            setIVec3(value);
            break;
        }

        case VariantTypeIVec4:
        {
            ivec4 value;
            d.deserialize("value", value);
            setIVec4(value);
            break;
        }

        case VariantTypeVec2:
        {
            vec2 value;
            d.deserialize("value", value);
            setVec2(value);
            break;
        }

        case VariantTypeVec3:
        {
            vec3 value;
            d.deserialize("value", value);
            setVec3(value);
            break;
        }

        case VariantTypeVec4:
        {
            vec4 value;
            d.deserialize("value", value);
            setVec4(value);
            break;
        }

        case VariantTypeDVec2:
        {
            dvec2 value;
            d.deserialize("value", value);
            setDVec2(value);
            break;
        }

        case VariantTypeDVec3:
        {
            dvec3 value;
            d.deserialize("value", value);
            setDVec3(value);
            break;
        }

        case VariantTypeDVec4:
        {
            dvec4 value;
            d.deserialize("value", value);
            setDVec4(value);
            break;
        }

        case VariantTypeMat2:
        {
            mat2 value;
            d.deserialize("value", value);
            setMat2(value);
            break;
        }

        case VariantTypeMat3:
        {
            mat3 value;
            d.deserialize("value", value);
            setMat3(value);
            break;
        }

        case VariantTypeMat4:
        {
            mat4 value;
            d.deserialize("value", value);
            setMat4(value);
            break;
        }

        case VariantTypeDMat2:
        {
            dmat2 value;
            d.deserialize("value", value);
            setDMat2(value);
            break;
        }

        case VariantTypeDMat3:
        {
            dmat3 value;
            d.deserialize("value", value);
            setDMat3(value);
            break;
        }

        case VariantTypeDMat4:
        {
            dmat4 value;
            d.deserialize("value", value);
            setDMat4(value);
            break;
        }

        case VariantTypeInt64:
        {
            glm::i64 value;
            d.deserialize("value", value);
            setInt64(value);
            break;
        }

        case VariantTypeInvalid:
            throw Exception("Tried to deserialize an invalid variant");
            break;

        default:
            throw Exception("Tried to serialize an unknown variant");
            break;
    }
}

Variant& Variant::operator= (const Variant& rhs) {
    if (this != &rhs) {
        std::string rhs_string = rhs.getString();
        Variant temp_rhs(rhs_string);

        switch (this->getType()) {
            case VariantTypeInvalid:
                deleteValue();
                currentType_ = VariantTypeInvalid;
                break;

            case VariantTypeBool:
                set<bool>(temp_rhs.getBool(), VariantTypeBool);
                break;

            case VariantTypeDouble:
                set<double>(temp_rhs.getDouble(), VariantTypeDouble);
                break;

            case VariantTypeFloat:
                set<float>(temp_rhs.getFloat(), VariantTypeFloat);
                break;

            case VariantTypeInteger:
                set<int>(temp_rhs.getInt(), VariantTypeInteger);
                break;

            case VariantTypeLong:
                set<long>(temp_rhs.getLong(), VariantTypeLong);
                break;

            case VariantTypeString:
                set<std::string>(temp_rhs.getString(), VariantTypeString);
                break;

            case VariantTypeIVec2:
                set<ivec2>(rhs.getIVec2(), VariantTypeIVec2);
                break;

            case VariantTypeIVec3:
                set<ivec3>(temp_rhs.getIVec3(), VariantTypeIVec3);
                break;

            case VariantTypeIVec4:
                set<ivec4>(temp_rhs.getIVec4(), VariantTypeIVec4);
                break;

            case VariantTypeVec2:
                set<vec2>(rhs.getVec2(), VariantTypeVec2);
                break;

            case VariantTypeVec3:
                set<vec3>(temp_rhs.getVec3(), VariantTypeVec3);
                break;

            case VariantTypeVec4:
                set<vec4>(temp_rhs.getVec4(), VariantTypeVec4);
                break;

            case VariantTypeDVec2:
                set<dvec2>(temp_rhs.getDVec2(), VariantTypeDVec2);
                break;

            case VariantTypeDVec3:
                set<dvec3>(temp_rhs.getDVec3(), VariantTypeDVec3);
                break;

            case VariantTypeDVec4:
                set<dvec4>(temp_rhs.getDVec4(), VariantTypeDVec4);
                break;

            case VariantTypeMat2:
                set<mat2>(temp_rhs.getMat2(), VariantTypeMat2);
                break;

            case VariantTypeMat3:
                set<mat3>(temp_rhs.getMat3(), VariantTypeMat3);
                break;

            case VariantTypeMat4:
                set<mat4>(temp_rhs.getMat4(), VariantTypeMat4);
                break;

            case VariantTypeDMat2:
                set<dmat2>(temp_rhs.getDMat2(), VariantTypeDMat2);
                break;

            case VariantTypeDMat3:
                set<dmat3>(temp_rhs.getDMat3(), VariantTypeDMat3);
                break;

            case VariantTypeDMat4:
                set<dmat4>(temp_rhs.getDMat4(), VariantTypeDMat4);
                break;

            case VariantTypeInt64:
                set<glm::i64>(temp_rhs.getInt64(), VariantTypeInt64);
                break;

            default:
                break;
        }

        if (rhs.getType() >= VariantTypeUserType) {
            currentType_ = rhs.currentType_;
            value_ = rhs.value_;
        }
    }

    return *this;
}

Variant& Variant::operator= (const bool& rhs) {
    set<bool>(rhs, VariantTypeBool);
    return *this;
}

Variant& Variant::operator= (const double& rhs) {
    set<double>(rhs, VariantTypeDouble);
    return *this;
}

Variant& Variant::operator= (const float& rhs) {
    set<float>(rhs, VariantTypeFloat);
    return *this;
}

Variant& Variant::operator= (const int& rhs) {
    set<int>(rhs, VariantTypeInteger);
    return *this;
}

Variant& Variant::operator= (const long& rhs) {
    set<long>(rhs, VariantTypeLong);
    return *this;
}

Variant& Variant::operator= (const glm::i64& rhs) {
    set<glm::i64>(rhs, VariantTypeInt64);
    return *this;
}

Variant& Variant::operator= (const std::string& rhs) {
    set<std::string>(rhs, VariantTypeString);
    return *this;
}

Variant& Variant::operator= (const ivec2& rhs) {
    set<ivec2>(rhs, VariantTypeIVec2);
    return *this;
}

Variant& Variant::operator= (const ivec3& rhs) {
    set<ivec3>(rhs, VariantTypeIVec3);
    return *this;
}

Variant& Variant::operator= (const ivec4& rhs) {
    set<ivec4>(rhs, VariantTypeIVec4);
    return *this;
}

Variant& Variant::operator= (const vec2& rhs) {
    set<vec2>(rhs, VariantTypeVec2);
    return *this;
}

Variant& Variant::operator= (const vec3& rhs) {
    set<vec3>(rhs, VariantTypeVec3);
    return *this;
}

Variant& Variant::operator= (const vec4& rhs) {
    set<vec4>(rhs, VariantTypeVec4);
    return *this;
}

Variant& Variant::operator= (const dvec2& rhs) {
    set<dvec2>(rhs, VariantTypeDVec2);
    return *this;
}

Variant& Variant::operator= (const dvec3& rhs) {
    set<dvec3>(rhs, VariantTypeDVec3);
    return *this;
}

Variant& Variant::operator= (const dvec4& rhs) {
    set<dvec4>(rhs, VariantTypeDVec4);
    return *this;
}

Variant& Variant::operator= (const mat2& rhs) {
    set<mat2>(rhs, VariantTypeMat2);
    return *this;
}

Variant& Variant::operator= (const mat3& rhs) {
    set<mat3>(rhs, VariantTypeMat3);
    return *this;
}

Variant& Variant::operator= (const mat4& rhs) {
    set<mat4>(rhs, VariantTypeMat4);
    return *this;
}

Variant& Variant::operator= (const dmat2& rhs) {
    set<dmat2>(rhs, VariantTypeDMat2);
    return *this;
}

Variant& Variant::operator= (const dmat3& rhs) {
    set<dmat3>(rhs, VariantTypeDMat3);
    return *this;
}

Variant& Variant::operator= (const dmat4& rhs) {
    set<dmat4>(rhs, VariantTypeDMat4);
    return *this;
}

bool Variant::operator== (const Variant& rhs) const {
    if (getType() != rhs.getType())
        return false;
    else {
        switch (currentType_) {
            case VariantTypeInvalid:
                return false;

            case VariantTypeBool:
                return (getBool() == rhs.getBool());

            case VariantTypeDouble:
                return (getDouble() == rhs.getDouble());

            case VariantTypeFloat:
                return (getFloat() == rhs.getFloat());

            case VariantTypeInteger:
                return (getInt() == rhs.getInt());

            case VariantTypeLong:
                return (getLong() == rhs.getLong());

            case VariantTypeInt64:
                return (getInt64() == rhs.getInt64());

            case VariantTypeString:
                return (getString() == rhs.getString());

            case VariantTypeIVec2:
                return (getIVec2() == rhs.getIVec2());

            case VariantTypeIVec3:
                return (getIVec3() == rhs.getIVec3());

            case VariantTypeIVec4:
                return (getIVec4() == rhs.getIVec4());

            case VariantTypeVec2:
                return (getVec2() == rhs.getVec2());

            case VariantTypeVec3:
                return (getVec3() == rhs.getVec3());

            case VariantTypeVec4:
                return (getVec4() == rhs.getVec4());

            case VariantTypeDVec2:
                return (getDVec2() == rhs.getDVec2());

            case VariantTypeDVec3:
                return (getDVec3() == rhs.getDVec3());

            case VariantTypeDVec4:
                return (getDVec4() == rhs.getDVec4());

            case VariantTypeMat2:
                return (getMat2() == rhs.getMat2());

            case VariantTypeMat3:
                return (getMat3() == rhs.getMat3());

            case VariantTypeMat4:
                return (getMat4() == rhs.getMat4());

            case VariantTypeDMat2:
                return (getDMat2() == rhs.getDMat2());

            case VariantTypeDMat3:
                return (getDMat3() == rhs.getDMat3());

            case VariantTypeDMat4:
                return (getDMat4() == rhs.getDMat4());

            default:
                return currentType_ == rhs.currentType_ && value_ == rhs.value_;
        }
    }

    return false;
}

bool Variant::operator!= (const Variant& rhs) const {
    return !(*this == rhs);
}


} // namespace
