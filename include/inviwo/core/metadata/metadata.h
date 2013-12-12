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
    virtual MetaData* create() const;
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
    virtual MetaDataPrimitiveType<T>* create() const;
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
MetaDataPrimitiveType<T>* MetaDataPrimitiveType<T>::create() const {
    return new MetaDataPrimitiveType<T>(*this);
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
void MetaDataPrimitiveType<T>::deserialize( IvwDeserializer& d ){}
template <typename T>
void MetaDataPrimitiveType<T>::serialize( IvwSerializer& s ) const{}

/*---------------------------------------------------------------------*/

class IVW_CORE_API BoolMetaData : public MetaDataPrimitiveType<bool> {
public:
    BoolMetaData();
    BoolMetaData(bool value);
    virtual std::string getClassName() const;
    virtual BoolMetaData* create() const;
    virtual BoolMetaData* clone() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d); 
};

/*---------------------------------------------------------------------*/

class IVW_CORE_API IntMetaData : public MetaDataPrimitiveType<int> {
public:
    IntMetaData();
    IntMetaData(int value);
    virtual std::string getClassName() const;
    virtual IntMetaData* create() const;
    virtual IntMetaData* clone() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/

class IVW_CORE_API IVec2MetaData : public MetaDataPrimitiveType<ivec2> {
public:
    IVec2MetaData();
    IVec2MetaData(ivec2 value);
    virtual std::string getClassName() const;
    virtual IVec2MetaData* create() const;
    virtual IVec2MetaData* clone() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/

class IVW_CORE_API IVec3MetaData : public MetaDataPrimitiveType<ivec3> {
public:
    IVec3MetaData();
    IVec3MetaData(ivec3 value);
    virtual std::string getClassName() const;
    virtual IVec3MetaData* create() const;
    virtual IVec3MetaData* clone() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

};

/*---------------------------------------------------------------------*/

class IVW_CORE_API UVec2MetaData : public MetaDataPrimitiveType<uvec2> {
public:
    UVec2MetaData();
    UVec2MetaData(uvec2 value);
    virtual std::string getClassName() const;
    virtual UVec2MetaData* create() const;
    virtual UVec2MetaData* clone() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

};

/*---------------------------------------------------------------------*/

class IVW_CORE_API UVec3MetaData : public MetaDataPrimitiveType<uvec3> {
public:
    UVec3MetaData();
    UVec3MetaData(uvec3 value);
    virtual std::string getClassName() const;
    virtual UVec3MetaData* create() const;
    virtual UVec3MetaData* clone() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

};

/*---------------------------------------------------------------------*/

class IVW_CORE_API FloatMetaData : public MetaDataPrimitiveType<float> {
public:
    FloatMetaData();
    FloatMetaData(float value);
    virtual std::string getClassName() const;
    virtual FloatMetaData* create() const;
    virtual FloatMetaData* clone() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/

class IVW_CORE_API Vec2MetaData : public MetaDataPrimitiveType<vec2> {
public:
    Vec2MetaData();
    Vec2MetaData(vec2 value);
    virtual std::string getClassName() const;
    virtual Vec2MetaData* create() const;
    virtual Vec2MetaData* clone() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/

class IVW_CORE_API Vec3MetaData : public MetaDataPrimitiveType<vec3> {
public:
    Vec3MetaData();
    Vec3MetaData(vec3 value);
    virtual std::string getClassName() const;
    virtual Vec3MetaData* create() const;
    virtual Vec3MetaData* clone() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/

class IVW_CORE_API StringMetaData : public MetaDataPrimitiveType<std::string> {
public:
    StringMetaData();
    StringMetaData(std::string value);
    virtual std::string getClassName() const;
    virtual StringMetaData* create() const;
    virtual StringMetaData* clone() const;
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
};

/*---------------------------------------------------------------------*/
template<unsigned int N, typename T>
class VectorMetaData {};

template<typename T>
class VectorMetaData<4,T> : public MetaDataPrimitiveType<Vector<4,T> > {
public:
	VectorMetaData<4,T>() : MetaDataPrimitiveType<Vector<4,T> >(Vector<4,T>(0)) {};
	VectorMetaData<4,T>(Vector<4,T> value): MetaDataPrimitiveType<Vector<4,T> >(value) {};
	virtual std::string getClassName() const {
		std::ostringstream name;
		name << "VectorMetaData<4, " << typeid(T).name() << ">";
		return name.str();
	};
	virtual VectorMetaData<4,T>* create() const {
		return new VectorMetaData<4,T>();
	};

    virtual VectorMetaData<4,T>* clone() const {
        return new VectorMetaData<4,T>(*this);
    }

	virtual void serialize(IvwSerializer& s) const {
		Vector<4,T> v = MetaDataPrimitiveType<Vector<4,T> >::get();
		glm::detail::tvec4<T> u(v.x, v.y, v.z, v.w);
		s.serialize(getClassName(), u);
	};
	virtual void deserialize(IvwDeserializer& d)  {
		glm::detail::tvec4<T> u(0);
		d.deserialize(getClassName(), u);
		Vector<4,T> v(u);
		MetaDataPrimitiveType<Vector<4,T> >::set(u);
	};
};


template<typename T>
class VectorMetaData<3,T> : public MetaDataPrimitiveType<Vector<3,T> > {
public:
	VectorMetaData<3,T>() : MetaDataPrimitiveType<Vector<3,T> >(Vector<3,T>(0)) {};
	VectorMetaData<3,T>(Vector<3,T> value): MetaDataPrimitiveType<Vector<3,T> >(value) {};
	virtual std::string getClassName() const {
		std::ostringstream name;
		name << "VectorMetaData<3, " << typeid(T).name() << ">";
		return name.str();
	};
	virtual VectorMetaData<3,T>* create() const {
		return new VectorMetaData<3,T>();
	};
    virtual VectorMetaData<3,T>* clone() const{
        return new VectorMetaData<3,T>(*this);
    };
	virtual void serialize(IvwSerializer& s) const {
		Vector<3,T> v = MetaDataPrimitiveType<Vector<3,T> >::get();
		glm::detail::tvec3<T> u(v.x, v.y, v.z);
		s.serialize(getClassName(), u);
	};
	virtual void deserialize(IvwDeserializer& d)  {
		glm::detail::tvec3<T> u(0);
		d.deserialize(getClassName(), u);
		Vector<3,T> v(u);
		MetaDataPrimitiveType<Vector<3,T> >::set(u);
	};
};

template<typename T>
class VectorMetaData<2,T> : public MetaDataPrimitiveType<Vector<2,T> > {
public:
	VectorMetaData<2,T>() : MetaDataPrimitiveType<Vector<2,T> >(Vector<2,T>(0)) {};
	VectorMetaData<2,T>(Vector<2,T> value): MetaDataPrimitiveType<Vector<2,T> >(value) {};
	virtual std::string getClassName() const {
		std::ostringstream name;
		name << "VectorMetaData<2, " << typeid(T).name() << ">";
		return name.str();
	};
	virtual VectorMetaData<2,T>* create() const {
		return new VectorMetaData<2,T>();
	};
    virtual VectorMetaData<2,T>* clone() const{
        return new VectorMetaData<2,T>(*this);
    };
	virtual void serialize(IvwSerializer& s) const {
		Vector<2,T> v = MetaDataPrimitiveType<Vector<2,T> >::get();
		glm::detail::tvec2<T> u(v.x, v.y);
		s.serialize(getClassName(), u);
	};
	virtual void deserialize(IvwDeserializer& d)  {
		glm::detail::tvec2<T> u(0);
		d.deserialize(getClassName(), u);
		Vector<2,T> v(u);
		MetaDataPrimitiveType<Vector<2,T> >::set(u);
	};
};
template<typename T>
class VectorMetaData<1,T> : public MetaDataPrimitiveType<Vector<1,T> > {
public:
	VectorMetaData<1,T>() : MetaDataPrimitiveType<Vector<1,T> >(Vector<1,T>(0)) {};
	VectorMetaData<1,T>(Vector<1,T> value): MetaDataPrimitiveType<Vector<1,T> >(value) {};
	virtual std::string getClassName() const {
		std::ostringstream name;
		name << "VectorMetaData<1, " << typeid(T).name() << ">";
		return name.str();
	};
	virtual VectorMetaData<1,T>* create() const {
		return new VectorMetaData<1,T>();
	};
    virtual VectorMetaData<1,T>* clone() const{
        return new VectorMetaData<1,T>(*this);
    };
	virtual void serialize(IvwSerializer& s) const {
		Vector<1,T> v = MetaDataPrimitiveType<Vector<1,T> >::get();
		T u(v.x);
		s.serialize(getClassName(), u);
	};
	virtual void deserialize(IvwDeserializer& d)  {
		T u(0);
		d.deserialize(getClassName(), u);
		Vector<1,T> v(u);
		MetaDataPrimitiveType<Vector<1,T> >::set(u);
	};
};


template<unsigned int N, typename T>
class MatrixMetaData {};

template<typename T>
class MatrixMetaData<4,T> : public MetaDataPrimitiveType<Matrix<4,T> > {
public:
	MatrixMetaData<4,T>() : MetaDataPrimitiveType<Matrix<4,T> >(Matrix<4,T>(0)) {};
	MatrixMetaData<4,T>(Matrix<4,T> value): MetaDataPrimitiveType<Matrix<4,T> >(value) {};
	virtual std::string getClassName() const {
		std::ostringstream name;
		name << "MatrixMetaData<4, " << typeid(T).name() << ">";
		return name.str();
	};
	virtual MatrixMetaData<4,T>* create() const {
		return new MatrixMetaData<4,T>();
	};
    virtual MatrixMetaData<4,T>* clone() const{
        return new MatrixMetaData<4,T>(*this);
    };
	virtual void serialize(IvwSerializer& s) const {
		Matrix<4,T> m = MetaDataPrimitiveType<Matrix<4,T> >::get();
		glm::detail::tmat4x4<T> u(m[0][0], m[0][1], m[0][2], m[0][3], 
								  m[1][0], m[1][1], m[1][2], m[1][3],
								  m[2][0], m[2][1], m[2][2], m[2][3],
								  m[3][0], m[3][1], m[3][2], m[3][3]);
		s.serialize(getClassName(), u);
	};
	virtual void deserialize(IvwDeserializer& d)  {
		glm::detail::tmat4x4<T> u(0);
		d.deserialize(getClassName(), u);
		Matrix<4,T> v(u);
		MetaDataPrimitiveType<Matrix<4,T> >::set(u);
	};
};
template<typename T>
class MatrixMetaData<3,T> : public MetaDataPrimitiveType<Matrix<3,T> > {
public:
	MatrixMetaData<3,T>() : MetaDataPrimitiveType<Matrix<3,T> >(Matrix<3,T>(0)) {};
	MatrixMetaData<3,T>(Matrix<3,T> value): MetaDataPrimitiveType<Matrix<3,T> >(value) {};
	virtual std::string getClassName() const {
		std::ostringstream name;
		name << "MatrixMetaData<3, " << typeid(T).name() << ">";
		return name.str();
	};
	virtual MatrixMetaData<3,T>* create() const {
		return new MatrixMetaData<3,T>();
	};
    virtual MatrixMetaData<3,T>* clone() const{
        return new MatrixMetaData<3,T>(*this);
    };
	virtual void serialize(IvwSerializer& s) const {
		Matrix<3,T> m = MetaDataPrimitiveType<Matrix<3,T> >::get();
		glm::detail::tmat3x3<T> u(m[0][0], m[0][1], m[0][2], 
								  m[1][0], m[1][1], m[1][2],
								  m[2][0], m[2][1], m[2][2]);
		s.serialize(getClassName(), u);
	};
	virtual void deserialize(IvwDeserializer& d)  {
		glm::detail::tmat3x3<T> u(0);
		d.deserialize(getClassName(), u);
		Matrix<3,T> v(u);
		MetaDataPrimitiveType<Matrix<3,T> >::set(u);
	};
};
template<typename T>
class MatrixMetaData<2,T> : public MetaDataPrimitiveType<Matrix<2,T> > {
public:
	MatrixMetaData<2,T>() : MetaDataPrimitiveType<Matrix<2,T> >(Matrix<2,T>(0)) {};
	MatrixMetaData<2,T>(Matrix<2,T> value): MetaDataPrimitiveType<Matrix<2,T> >(value) {};
	virtual std::string getClassName() const {
		std::ostringstream name;
		name << "MatrixMetaData<2, " << typeid(T).name() << ">";
		return name.str();
	};
	virtual MatrixMetaData<2,T>* create() const {
		return new MatrixMetaData<2,T>();
	};
    virtual MatrixMetaData<2,T>* clone() const{
        return new MatrixMetaData<2,T>(*this);
    };
	virtual void serialize(IvwSerializer& s) const {
		Matrix<2,T> m = MetaDataPrimitiveType<Matrix<2,T> >::get();
		glm::detail::tmat2x2<T> u(m[0][0], m[0][1], 
								  m[1][0], m[1][1]);
		s.serialize(getClassName(), u);
	};
	virtual void deserialize(IvwDeserializer& d)  {
		glm::detail::tmat2x2<T> u(0);
		d.deserialize(getClassName(), u);
		Matrix<2,T> v(u);
		MetaDataPrimitiveType<Matrix<2,T> >::set(u);
	};
};
/*---------------------------------------------------------------------*/
} // namespace

#endif // IVW_PROCESSOR_H
