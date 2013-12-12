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

#ifndef IVW_METADATA_H
#define IVW_METADATA_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API MetaData : public IvwSerializable {

public:
    MetaData();
    MetaData(const MetaData& rhs);
    MetaData& operator=(const MetaData& that);
    virtual ~MetaData();
    virtual std::string getClassName() const;
    virtual MetaData* clone() const; 
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/

template <typename T>
class MetaDataPrimitiveType : public MetaData {
public:
    MetaDataPrimitiveType(T value);
    MetaDataPrimitiveType(const MetaDataPrimitiveType& rhs);
    MetaDataPrimitiveType& operator=(const MetaDataPrimitiveType& that);
    virtual ~MetaDataPrimitiveType() {};
    virtual std::string getClassName() const;
    virtual MetaDataPrimitiveType<T>* clone() const; 
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    void set(T value);
    T get() const;    
protected:
    T value_;
};

template <typename T>
MetaDataPrimitiveType<T>::MetaDataPrimitiveType(T value) : MetaData(), value_(value) {}

template <typename T>
MetaDataPrimitiveType<T>::MetaDataPrimitiveType(const MetaDataPrimitiveType<T>& rhs) 
    : MetaData(rhs), value_(rhs.value_) {}

template <typename T>
MetaDataPrimitiveType<T>& MetaDataPrimitiveType<T>::operator=(const MetaDataPrimitiveType<T>& that){
    if (this != &that) {
       value_ = that.value_;
       MetaData::operator=(that);
    }
    return *this;
}

template <typename T>
std::string MetaDataPrimitiveType<T>::getClassName() const {
    std::ostringstream name;
    name << "MetaDataPrimitiveType<" << typeid(T).name() << ">";
    return name.str();
}

template <typename T>
MetaDataPrimitiveType<T>* MetaDataPrimitiveType<T>::clone() const {
    return new MetaDataPrimitiveType<T>(*this);
}

template <typename T>
void MetaDataPrimitiveType<T>::set(T value) {
    value_ = value;
}

template <typename T>
T MetaDataPrimitiveType<T>::get() const {
    return value_;
}

template <typename T>
void inviwo::MetaDataPrimitiveType<T>::serialize( IvwSerializer& s ) const{
    s.serialize("MetaData", value_);
    s.serialize(IvwSerializeConstants::TYPE_ATTRIBUTE, getClassName(), true);
}

template <typename T>
void inviwo::MetaDataPrimitiveType<T>::deserialize( IvwDeserializer& d ){
    d.deserialize("MetaData", value_);
}

//
/////*---------------------------------------------------------------------*/
#define MetaDataMacroA(n, t, d) \
class IVW_CORE_API n##MetaData : public MetaDataPrimitiveType<t> {\
public:\
	n##MetaData();\
	n##MetaData(t value);\
	virtual std::string getClassName() const;\
	virtual n##MetaData* clone() const;\
};

MetaDataMacroA(Bool, bool, false)
MetaDataMacroA(Int, int, 0)
MetaDataMacroA(Float, float, 0.0f)
MetaDataMacroA(String, std::string, "")
MetaDataMacroA(IVec2, ivec2, ivec2(0))
MetaDataMacroA(IVec3, ivec3, ivec3(0))
MetaDataMacroA(IVec4, ivec4, ivec4(0))
MetaDataMacroA(Vec2, vec2, vec2(0))
MetaDataMacroA(Vec3, vec3, vec3(0))
MetaDataMacroA(Vec4, vec4, vec4(0))
MetaDataMacroA(DVec2, dvec2, dvec2(0))
MetaDataMacroA(DVec3, dvec3, dvec3(0))
MetaDataMacroA(DVec4, dvec4, dvec4(0))
MetaDataMacroA(UVec2, uvec2, uvec2(0))
MetaDataMacroA(UVec3, uvec3, uvec3(0))
MetaDataMacroA(UVec4, uvec4, uvec4(0))
MetaDataMacroA(Mat2, mat2, mat2(0))
MetaDataMacroA(Mat3, mat3, mat3(0))
MetaDataMacroA(Mat4, mat4, mat4(0))



//#include <inviwo/core/metadata/metadatadefinefunc.h>

/*---------------------------------------------------------------------*/

template<unsigned int N, typename T>
class IVW_CORE_API VectorMetaData {};

template<typename T>
class IVW_CORE_API VectorMetaData<4,T> : public MetaDataPrimitiveType<Vector<4,T> > {
public:
	VectorMetaData<4,T>() : MetaDataPrimitiveType<Vector<4,T> >(Vector<4,T>(0)) {};
	VectorMetaData<4,T>(Vector<4,T> value): MetaDataPrimitiveType<Vector<4,T> >(value) {};
	virtual std::string getClassName() const {
		std::ostringstream name;
		name << "VectorMetaData<4, " << typeid(T).name() << ">";
		return name.str();
	};

    virtual VectorMetaData<4,T>* clone() const {
        return new VectorMetaData<4,T>(*this);
    }

	virtual void serialize(IvwSerializer& s) const {
		glm::detail::tvec4<T> u(value_.x, value_.y, value_.z, value_.w);
        s.serialize("MetaData", u);
        s.serialize(IvwSerializeConstants::TYPE_ATTRIBUTE, getClassName(), true);
	};
	virtual void deserialize(IvwDeserializer& d)  {
        glm::detail::tvec4<T> u(0);
        d.deserialize("MetaData", u);
        set(u);
	};
};


template<typename T>
class IVW_CORE_API VectorMetaData<3,T> : public MetaDataPrimitiveType<Vector<3,T> > {
public:
	VectorMetaData<3,T>() : MetaDataPrimitiveType<Vector<3,T> >(Vector<3,T>(0)) {};
	VectorMetaData<3,T>(Vector<3,T> value): MetaDataPrimitiveType<Vector<3,T> >(value) {};
	virtual std::string getClassName() const {
		std::ostringstream name;
		name << "VectorMetaData<3, " << typeid(T).name() << ">";
		return name.str();
	};
    virtual VectorMetaData<3,T>* clone() const{
        return new VectorMetaData<3,T>(*this);
    };
	virtual void serialize(IvwSerializer& s) const {
        glm::detail::tvec3<T> u(value_.x, value_.y, value_.z);
        s.serialize("MetaData", u);
        s.serialize(IvwSerializeConstants::TYPE_ATTRIBUTE, getClassName(), true);
	};
	virtual void deserialize(IvwDeserializer& d)  {
        glm::detail::tvec3<T> u(0);
        d.deserialize("MetaData", u);
        set(u);
	};
};

template<typename T>
class IVW_CORE_API VectorMetaData<2,T> : public MetaDataPrimitiveType<Vector<2,T> > {
public:
	VectorMetaData<2,T>() : MetaDataPrimitiveType<Vector<2,T> >(Vector<2,T>(0)) {};
	VectorMetaData<2,T>(Vector<2,T> value): MetaDataPrimitiveType<Vector<2,T> >(value) {};
	virtual std::string getClassName() const {
		std::ostringstream name;
		name << "VectorMetaData<2, " << typeid(T).name() << ">";
		return name.str();
	};
    virtual VectorMetaData<2,T>* clone() const{
        return new VectorMetaData<2,T>(*this);
    };
	virtual void serialize(IvwSerializer& s) const {
        glm::detail::tvec2<T> u(value_.x, value_.y);
        s.serialize("MetaData", u);
        s.serialize(IvwSerializeConstants::TYPE_ATTRIBUTE, getClassName(), true);
    };
	virtual void deserialize(IvwDeserializer& d)  {
        glm::detail::tvec2<T> u(0);
        d.deserialize("MetaData", u);
        set(u);
	};
};

template<unsigned int N, typename T>
class IVW_CORE_API MatrixMetaData {};

template<typename T>
class IVW_CORE_API MatrixMetaData<4,T> : public MetaDataPrimitiveType<Matrix<4,T> > {
public:
	MatrixMetaData<4,T>() : MetaDataPrimitiveType<Matrix<4,T> >(Matrix<4,T>(0)) {};
	MatrixMetaData<4,T>(Matrix<4,T> value): MetaDataPrimitiveType<Matrix<4,T> >(value) {};
	virtual std::string getClassName() const {
		std::ostringstream name;
		name << "MatrixMetaData<4, " << typeid(T).name() << ">";
		return name.str();
	};
    virtual MatrixMetaData<4,T>* clone() const{
        return new MatrixMetaData<4,T>(*this);
    };
	virtual void serialize(IvwSerializer& s) const {
        glm::detail::tmat4x4<T> u(
            value_[0][0], value_[0][1], value_[0][2], value_[0][3], 
            value_[1][0], value_[1][1], value_[1][2], value_[1][3],
            value_[2][0], value_[2][1], value_[2][2], value_[2][3],
            value_[3][0], value_[3][1], value_[3][2], value_[3][3]);
        s.serialize("MetaData", u);
        s.serialize(IvwSerializeConstants::TYPE_ATTRIBUTE, getClassName(), true);
    };
	virtual void deserialize(IvwDeserializer& d)  {
        glm::detail::tmat4x4<T> u(0);
        d.deserialize("MetaData", u);
        set(u);
	};
};
template<typename T>
class IVW_CORE_API MatrixMetaData<3,T> : public MetaDataPrimitiveType<Matrix<3,T> > {
public:
	MatrixMetaData<3,T>() : MetaDataPrimitiveType<Matrix<3,T> >(Matrix<3,T>(0)) {};
	MatrixMetaData<3,T>(Matrix<3,T> value): MetaDataPrimitiveType<Matrix<3,T> >(value) {};
	virtual std::string getClassName() const {
		std::ostringstream name;
		name << "MatrixMetaData<3, " << typeid(T).name() << ">";
		return name.str();
	};
    virtual MatrixMetaData<3,T>* clone() const{
        return new MatrixMetaData<3,T>(*this);
    };
	virtual void serialize(IvwSerializer& s) const {
        glm::detail::tmat3x3<T> u(
            value_[0][0], value_[0][1], value_[0][2], 
            value_[1][0], value_[1][1], value_[1][2],
            value_[2][0], value_[2][1], value_[2][2]);
        s.serialize("MetaData", u);
        s.serialize(IvwSerializeConstants::TYPE_ATTRIBUTE, getClassName(), true);
    };
	virtual void deserialize(IvwDeserializer& d)  {
        glm::detail::tmat3x3<T> u(0);
        d.deserialize("MetaData", u);
        set(u);
	};
};
template<typename T>
class IVW_CORE_API MatrixMetaData<2,T> : public MetaDataPrimitiveType<Matrix<2,T> > {
public:
	MatrixMetaData<2,T>() : MetaDataPrimitiveType<Matrix<2,T> >(Matrix<2,T>(0)) {};
	MatrixMetaData<2,T>(Matrix<2,T> value): MetaDataPrimitiveType<Matrix<2,T> >(value) {};
	virtual std::string getClassName() const {
		std::ostringstream name;
		name << "MatrixMetaData<2, " << typeid(T).name() << ">";
		return name.str();
	};
    virtual MatrixMetaData<2,T>* clone() const{
        return new MatrixMetaData<2,T>(*this);
    };
	virtual void serialize(IvwSerializer& s) const {
        glm::detail::tmat2x2<T> u(
            value_[0][0], value_[0][1], 
            value_[1][0], value_[1][1]);
        s.serialize("MetaData", u);
        s.serialize(IvwSerializeConstants::TYPE_ATTRIBUTE, getClassName(), true);
    };
	virtual void deserialize(IvwDeserializer& d)  {
        glm::detail::tmat2x2<T> u(0);
        d.deserialize("MetaData", u);
        set(u);
	};
};
/*---------------------------------------------------------------------*/
} // namespace

#endif // IVW_PROCESSOR_H
