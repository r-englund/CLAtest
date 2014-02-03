 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file authors: Alexander Johansson, Timo Ropinski
 *
 *********************************************************************************/

#ifndef  IVW_MATRIXPROPERTIES_H
#define IVW_MATRIXPROPERTIES_H

#include "inviwo/core/common/inviwocoredefine.h"
#include "inviwo/core/properties/ordinalproperty.h"

namespace inviwo {
/** class FloatMat2Property
*  \brief  The FloatMat2Property provides a property holding a 2 by 2 matrix of float values
*
*  The FlaotMat2Property are used by the processors.
*  It has a widget that shows editable text fields for each element in the matrix in a 2 by 2 grid
*
* @see OrdinalProperty
*/
class IVW_CORE_API FloatMat2Property : public OrdinalProperty<mat2> {
public:
    /**
     * \brief Constructs a FloatMat2Property
     *
     * <DESCRIBE THE METHOD>
     *
     * @param std::string identifier <DESCRIBE ME>
     * @param std::string displayName <DESCRIBE ME>
     * @param mat2 value <DESCRIBE ME>
     * @param mat2 minValue <DESCRIBE ME>
     * @param mat2 maxValue <DESCRIBE ME>
     * @param mat2 increment <DESCRIBE ME>
     * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
     * @param PropertySemantics semantics <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */
    FloatMat2Property(std::string identifier, std::string displayName, mat2 value = mat2(0.0f),
                      mat2 minValue=mat2(0.0f), mat2 maxValue= mat2(1.0f), mat2 increment=mat2(0.1f),
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "FloatMat2Property"; }
};
/** class FloatMat3Property
*  \brief  The FloatMat3Property provides a property holding a 3 by 3 matrix of float values
*
*  The FlaotMat3Property are used by the processors.
*  It has a widget that shows editable text fields for each element in the matrix in a 3 by 3 grid

* @see OrdinalProperty
*/
class IVW_CORE_API FloatMat3Property : public OrdinalProperty<mat3> {
public:
    /**
     * \brief Constructs a FloatMat3Property
     *
     * <DESCRIBE THE METHOD>
     *
     * @param std::string identifier <DESCRIBE ME>
     * @param std::string displayName <DESCRIBE ME>
     * @param mat3 value <DESCRIBE ME>
     * @param mat3 minValue <DESCRIBE ME>
     * @param mat3 maxValue <DESCRIBE ME>
     * @param mat3 increment <DESCRIBE ME>
     * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
     * @param PropertySemantics semantics <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */
    FloatMat3Property(std::string identifier, std::string displayName, mat3 value = mat3(0.0f),
                      mat3 minValue=mat3(0.0f), mat3 maxValue= mat3(1.0f), mat3 increment=mat3(0.1f),
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "FloatMat3Property"; }
};
/** class FloatMat4Property
*  \brief  The FloatMat4Property provides a property holding a 4 by 4 matrix of float values
*
*  The FlaotMat4Property are used by the processors.
*  It has a widget that shows editable text fields for each element in the matrix in a 4 by 4 grid
*
* @see OrdinalProperty
*/
class IVW_CORE_API FloatMat4Property : public OrdinalProperty<mat4> {
public:
    /**
     * \brief Constructs a FloatMat4Property
     *
     * <DESCRIBE THE METHOD>
     *
     * @param std::string identifier <DESCRIBE ME>
     * @param std::string displayName <DESCRIBE ME>
     * @param mat4 value <DESCRIBE ME>
     * @param mat4 minValue <DESCRIBE ME>
     * @param mat4 maxValue <DESCRIBE ME>
     * @param mat4 increment <DESCRIBE ME>
     * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
     * @param PropertySemantics semantics <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */
    FloatMat4Property(std::string identifier, std::string displayName, mat4 value = mat4(0.0f),
                      mat4 minValue=mat4(0.0f), mat4 maxValue= mat4(1.0f), mat4 increment=mat4(0.1f),
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "FloatMat4Property"; }
};

}//namespace

#endif