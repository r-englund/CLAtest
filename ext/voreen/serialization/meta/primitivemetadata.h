/**********************************************************************
 *                                                                    *
 * Voreen - The Volume Rendering Engine                               *
 *                                                                    *
 * Created between 2005 and 2011 by The Voreen Team                   *
 * as listed in CREDITS.TXT <http://www.voreen.org>                   *
 *                                                                    *
 * This file is part of the Voreen software package. Voreen is free   *
 * software: you can redistribute it and/or modify it under the terms *
 * of the GNU General Public License version 2 as published by the    *
 * Free Software Foundation.                                          *
 *                                                                    *
 * Voreen is distributed in the hope that it will be useful,          *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the       *
 * GNU General Public License for more details.                       *
 *                                                                    *
 * You should have received a copy of the GNU General Public License  *
 * in the file "LICENSE.txt" along with this program.                 *
 * If not, see <http://www.gnu.org/licenses/>.                        *
 *                                                                    *
 * The authors reserve all rights not expressly granted herein. For   *
 * non-commercial academic use see the license exception specified in *
 * the file "LICENSE-academic.txt". To get information about          *
 * commercial licensing please contact the authors.                   *
 *                                                                    *
 **********************************************************************/

#ifndef VRN_PRIMITIVEMETADATA_H
#define VRN_PRIMITIVEMETADATA_H

#include "../serialization.h"

namespace voreen {

/**
 * Generic abstract base class for meta data of primitive types.
 */
template<typename T>
class PrimitiveMetaDataBase : public MetaDataBase {
public:
    PrimitiveMetaDataBase() {}
    PrimitiveMetaDataBase(T value);

    /**
     * Assigns the value to be stored.
     */
    void setValue(T value);

    /**
     * Returns the stored value.
     */
    T getValue() const;

    virtual MetaDataBase* clone() const;

    /// @see Serializable::serialize
    virtual void serialize(XmlSerializer& s) const;
    /// @see Serializable::deserialize
    virtual void deserialize(XmlDeserializer& s);

private:
    T value_;
};

//---------------------------------------------------------------------------------------

/**
 * Holds a single boolean value to be stored as serialization meta data.
 *
 * @see MetaDataBase
 */
class BoolMetaData : public PrimitiveMetaDataBase<bool> {
};

//---------------------------------------------------------------------------------------

/**
 * Holds a single string value to be stored as serialization meta data.
 *
 * @see MetaDataBase
 */
class StringMetaData : public PrimitiveMetaDataBase<std::string> {
};

//---------------------------------------------------------------------------------------

/**
 * Holds a single int value to be stored as serialization meta data.
 *
 * @see MetaDataBase
 */
class IntMetaData : public PrimitiveMetaDataBase<int> {
};

//---------------------------------------------------------------------------------------

/**
 * Holds a single size_t value to be stored as serialization meta data.
 *
 * @see MetaDataBase
 */
class SizeTMetaData : public PrimitiveMetaDataBase<size_t> {
};

//---------------------------------------------------------------------------------------

/**
 * Holds a single float value to be stored as serialization meta data.
 *
 * @see MetaDataBase
 */
class FloatMetaData : public PrimitiveMetaDataBase<float> {
};

//---------------------------------------------------------------------------------------

/**
 * Holds a single double value to be stored as serialization meta data.
 *
 * @see MetaDataBase
 */
class DoubleMetaData : public PrimitiveMetaDataBase<double> {
};

//---------------------------------------------------------------------------------------

/**
 * Holds a vec3 value to be stored as serialization meta data.
 *
 * @see MetaDataBase
 */
class Vec3MetaData : public PrimitiveMetaDataBase<tgt::vec3> {
};

//---------------------------------------------------------------------------------------

/**
 * Holds a mat4 value to be stored as serialization meta data.
 *
 * @see MetaDataBase
 */
class Mat4MetaData : public PrimitiveMetaDataBase<tgt::mat4> {
};

//---------------------------------------------------------------------------------------
//template definitions

template <typename T>
PrimitiveMetaDataBase<T>::PrimitiveMetaDataBase(T value) : value_(value) {}

template <typename T>
void PrimitiveMetaDataBase<T>::setValue(T value) {
    value_ = value;
}

template <typename T>
T PrimitiveMetaDataBase<T>::getValue() const {
    return value_;
}

template <typename T>
MetaDataBase* PrimitiveMetaDataBase<T>::clone() const {
    return new PrimitiveMetaDataBase<T>(getValue());
}

template <typename T>
void PrimitiveMetaDataBase<T>::serialize(XmlSerializer& s) const {
    s.serialize("value", value_);
}

template <typename T>
void PrimitiveMetaDataBase<T>::deserialize(XmlDeserializer& s) {
    s.deserialize("value", value_);
}

} // namespace

#endif // VRN_PRIMITIVEMETADATA_H
