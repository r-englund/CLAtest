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
 * Main file author: Peter Steneteg
 *
 *********************************************************************************/

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
    virtual bool equal(const MetaData& rhs) const;
    friend bool IVW_CORE_API operator==(const MetaData& lhs, const MetaData& rhs);
};

bool IVW_CORE_API operator==(const MetaData& lhs, const MetaData& rhs);
bool IVW_CORE_API operator!=(const MetaData& lhs, const MetaData& rhs);

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
    virtual void set(T value);
    virtual T get() const;
    virtual bool equal(const MetaData& rhs) const;
    template <typename V>
    friend bool operator==(const MetaDataPrimitiveType<V>& lhs, const MetaDataPrimitiveType<V>& rhs);
protected:
    T value_;
};

template <typename T>
bool inviwo::MetaDataPrimitiveType<T>::equal(const MetaData& rhs) const {
    const MetaDataPrimitiveType<T>* tmp = dynamic_cast<const MetaDataPrimitiveType<T>*>(&rhs);
    if (tmp) {
        return tmp->value_ == value_;
    } else {
        return false;
    }
}

template <typename T>
bool operator==(const MetaDataPrimitiveType<T>& lhs, const MetaDataPrimitiveType<T>& rhs) {
    return lhs.value_ == rhs.value_;
}

template <typename T>
MetaDataPrimitiveType<T>::MetaDataPrimitiveType(T value) : MetaData(), value_(value) {}

template <typename T>
MetaDataPrimitiveType<T>::MetaDataPrimitiveType(const MetaDataPrimitiveType<T>& rhs)
    : MetaData(rhs), value_(rhs.value_) {}

template <typename T>
MetaDataPrimitiveType<T>& MetaDataPrimitiveType<T>::operator=(
    const MetaDataPrimitiveType<T>& that) {
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
void inviwo::MetaDataPrimitiveType<T>::serialize(IvwSerializer& s) const {
    s.serialize("MetaData", value_);
    s.serialize(IvwSerializeConstants::TYPE_ATTRIBUTE, getClassName(), true);
}

template <typename T>
void inviwo::MetaDataPrimitiveType<T>::deserialize(IvwDeserializer& d) {
    d.deserialize("MetaData", value_);
}


/*---------------------------------------------------------------------*/
#define MetaDataMacro(n, t, d, v) \
    class IVW_CORE_API n##MetaData : public MetaDataPrimitiveType<t> {\
    public:\
        n##MetaData();\
        n##MetaData(t value);\
        virtual ~n##MetaData() {};\
        virtual std::string getClassName() const;\
        virtual n##MetaData* clone() const;\
    };
#include <inviwo/core/metadata/metadatadefinefunc.h>


/*---------------------------------------------------------------------*/

template<unsigned int N, typename T>
class VectorMetaData {};

template<typename T>
class VectorMetaData<4,T> : public MetaDataPrimitiveType<Vector<4,T> > {
public:
    VectorMetaData<4,T>() : MetaDataPrimitiveType<Vector<4,T> >(Vector<4,T>(0)) {};
    VectorMetaData<4,T>(Vector<4,T> value): MetaDataPrimitiveType<Vector<4,T> >(value) {};
    virtual ~VectorMetaData<4,T>() {};

    virtual std::string getClassName() const {
        std::ostringstream name;
        name << "VectorMetaData<4, " << typeid(T).name() << ">";
        return name.str();
    };
    virtual VectorMetaData<4,T>* clone() const {
        return new VectorMetaData<4,T>(*this);
    }
    virtual void serialize(IvwSerializer& s) const {
        s.serialize("MetaData", this->get().getGLM());
        s.serialize(IvwSerializeConstants::TYPE_ATTRIBUTE, getClassName(), true);
    };
    virtual void deserialize(IvwDeserializer& d)  {
        glm::detail::tvec4<T, glm::defaultp> u(0);
        d.deserialize("MetaData", u);
        this->set(u);
    };
};


template<typename T>
class VectorMetaData<3,T> : public MetaDataPrimitiveType<Vector<3,T> > {
public:
    VectorMetaData<3,T>() : MetaDataPrimitiveType<Vector<3,T> >(Vector<3,T>(0)) {};
    VectorMetaData<3,T>(Vector<3,T> value): MetaDataPrimitiveType<Vector<3,T> >(value) {};
    virtual ~VectorMetaData<3,T>() {};
    virtual std::string getClassName() const {
        std::ostringstream name;
        name << "VectorMetaData<3, " << typeid(T).name() << ">";
        return name.str();
    };
    virtual VectorMetaData<3,T>* clone() const {
        return new VectorMetaData<3,T>(*this);
    };
    virtual void serialize(IvwSerializer& s) const {
        s.serialize("MetaData", this->get().getGLM());
        s.serialize(IvwSerializeConstants::TYPE_ATTRIBUTE, getClassName(), true);
    };
    virtual void deserialize(IvwDeserializer& d)  {
        glm::detail::tvec3<T, glm::defaultp> u(0);
        d.deserialize("MetaData", u);
        this->set(u);
    };
};

template<typename T>
class VectorMetaData<2,T> : public MetaDataPrimitiveType<Vector<2,T> > {
public:
    VectorMetaData<2,T>() : MetaDataPrimitiveType<Vector<2,T> >(Vector<2,T>(0)) {};
    VectorMetaData<2,T>(Vector<2,T> value): MetaDataPrimitiveType<Vector<2,T> >(value) {};
    virtual ~VectorMetaData<2,T>() {};
    virtual std::string getClassName() const {
        std::ostringstream name;
        name << "VectorMetaData<2, " << typeid(T).name() << ">";
        return name.str();
    };
    virtual VectorMetaData<2,T>* clone() const {
        return new VectorMetaData<2,T>(*this);
    };
    virtual void serialize(IvwSerializer& s) const {
        s.serialize("MetaData", this->get().getGLM());
        s.serialize(IvwSerializeConstants::TYPE_ATTRIBUTE, getClassName(), true);
    };
    virtual void deserialize(IvwDeserializer& d)  {
        glm::detail::tvec2<T, glm::defaultp> u(0);
        d.deserialize("MetaData", u);
        this->set(u);
    };
};

template<unsigned int N, typename T>
class MatrixMetaData {};

template<typename T>
class IVW_CORE_API MatrixMetaData<4,T> : public MetaDataPrimitiveType<Matrix<4,T> > {
public:
    MatrixMetaData<4,T>() : MetaDataPrimitiveType<Matrix<4,T> >(Matrix<4,T>(0)) {};
    MatrixMetaData<4,T>(Matrix<4,T> value): MetaDataPrimitiveType<Matrix<4,T> >(value) {};
    virtual ~MatrixMetaData<4,T>() {};
    virtual std::string getClassName() const {
        std::ostringstream name;
        name << "MatrixMetaData<4, " << typeid(T).name() << ">";
        return name.str();
    };
    virtual MatrixMetaData<4,T>* clone() const {
        return new MatrixMetaData<4,T>(*this);
    };
    virtual void serialize(IvwSerializer& s) const {
        s.serialize("MetaData", this->get().getGLM());
        s.serialize(IvwSerializeConstants::TYPE_ATTRIBUTE, getClassName(), true);
    };
    virtual void deserialize(IvwDeserializer& d)  {
        glm::detail::tmat4x4<T, glm::defaultp> u(0);
        d.deserialize("MetaData", u);
        this->set(u);
    };
};
template<typename T>
class MatrixMetaData<3,T> : public MetaDataPrimitiveType<Matrix<3,T> > {
public:
    MatrixMetaData<3,T>() : MetaDataPrimitiveType<Matrix<3,T> >(Matrix<3,T>(0)) {};
    MatrixMetaData<3,T>(Matrix<3,T> value): MetaDataPrimitiveType<Matrix<3,T> >(value) {};
    virtual ~MatrixMetaData<3,T>() {};
    virtual std::string getClassName() const {
        std::ostringstream name;
        name << "MatrixMetaData<3, " << typeid(T).name() << ">";
        return name.str();
    };
    virtual MatrixMetaData<3,T>* clone() const {
        return new MatrixMetaData<3,T>(*this);
    };
    virtual void serialize(IvwSerializer& s) const {
        s.serialize("MetaData", this->get().getGLM());
        s.serialize(IvwSerializeConstants::TYPE_ATTRIBUTE, getClassName(), true);
    };
    virtual void deserialize(IvwDeserializer& d)  {
        glm::detail::tmat3x3<T, glm::defaultp> u(0);
        d.deserialize("MetaData", u);
        this->set(u);
    };
};
template<typename T>
class MatrixMetaData<2,T> : public MetaDataPrimitiveType<Matrix<2,T> > {
public:
    MatrixMetaData<2,T>() : MetaDataPrimitiveType<Matrix<2,T> >(Matrix<2,T>(0)) {};
    MatrixMetaData<2,T>(Matrix<2,T> value): MetaDataPrimitiveType<Matrix<2,T> >(value) {};
    virtual ~MatrixMetaData<2,T>() {};
    virtual std::string getClassName() const {
        std::ostringstream name;
        name << "MatrixMetaData<2, " << typeid(T).name() << ">";
        return name.str();
    };
    virtual MatrixMetaData<2,T>* clone() const {
        return new MatrixMetaData<2,T>(*this);
    };
    virtual void serialize(IvwSerializer& s) const {
        s.serialize("MetaData", this->get().getGLM());
        s.serialize(IvwSerializeConstants::TYPE_ATTRIBUTE, getClassName(), true);
    };
    virtual void deserialize(IvwDeserializer& d)  {
        glm::detail::tmat2x2<T, glm::defaultp> u(0);
        d.deserialize("MetaData", u);
        this->set(u);
    };
};
/*---------------------------------------------------------------------*/
} // namespace

#endif // IVW_PROCESSOR_H
