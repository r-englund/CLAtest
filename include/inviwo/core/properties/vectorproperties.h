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
 * Main file authors: Timo Ropinski, Erik Sundén, Alexander Johansson
 *
 *********************************************************************************/

#ifndef IVW_VECTORPROPERTIES_H
#define IVW_VECTORPROPERTIES_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <inviwo/core/properties/minmaxproperty.h>

namespace inviwo {
/** class FloatMinMaxProperty
*   \brief The FloatMinMaxProperty class provides a property holding a 2-D vector containing float values, where x is always less then y.
*
*   A FloatMinMax property are normally used by a processor.
*   The property has a widget consisting of 1 range slider for setting the float values.
* @see OrdinalProperty
*/
class IVW_CORE_API FloatMinMaxProperty : public MinMaxProperty<glm::mediump_float> {
public:
    /**
     * \brief Constructs a FloatMinMaxProperty
     */
    FloatMinMaxProperty(std::string identifier, std::string displayName,
                        float valueMin = 0.f, float valueMax = 1.f, float rangeMin = 0.f, float rangeMax = 1.f, float increment = 0.01f,
                        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                        PropertySemantics semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);
    virtual std::string getClassName()  const { return "FloatMinMaxProperty"; }
};
/** class FloatVec2Property
*   \brief The FloatVec2Property class provides a property holding a 2-D vector containing float values.
*
*   A FloatVec2 property are normally used by a processor.
*   The property has a widget consisting of 2 sliders for setting the float values.
* @see OrdinalProperty
*/
class IVW_CORE_API FloatVec2Property : public OrdinalProperty<vec2> {
public:
    /**
     * \brief Constructs a FloatVec2Property.
     *
     * <DESCRIBE THE METHOD>
     *
     * @param std::string identifier <DESCRIBE ME>
     * @param std::string displayName <DESCRIBE ME>
     * @param vec2 value <DESCRIBE ME>
     * @param vec2 minValue <DESCRIBE ME>
     * @param vec2 maxValue <DESCRIBE ME>
     * @param vec2 increment <DESCRIBE ME>
     * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
     * @param PropertySemantics semantics <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */
    FloatVec2Property(std::string identifier, std::string displayName, vec2 value = vec2(0.0f),
                      vec2 minValue=vec2(0.0f), vec2 maxValue= vec2(1.0f), vec2 increment=vec2(0.01f),
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "FloatVec2Property"; }

};
/** class FloatVec3Property
*   \brief The FloatVec3Property class provides a property holding a 3-D vector containing float values.
*
*   A FloatVec3 property are normally used by a processor.
*   The property has a widget consisting of 3 sliders for setting the float values.
* @see OrdinalProperty
*/
class IVW_CORE_API FloatVec3Property : public OrdinalProperty<vec3> {
public:
    /**
     * \brief Constructs a FloatVec3Property.
     *
     * <DESCRIBE THE METHOD>
     *
     * @param std::string identifier <DESCRIBE ME>
     * @param std::string displayName <DESCRIBE ME>
     * @param vec3 value <DESCRIBE ME>
     * @param vec3 minValue <DESCRIBE ME>
     * @param vec3 maxValue <DESCRIBE ME>
     * @param vec3 increment <DESCRIBE ME>
     * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
     * @param PropertySemantics semantics <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */
    FloatVec3Property(std::string identifier, std::string displayName, vec3 value = vec3(0.0f),
                      vec3 minValue=vec3(0.0f), vec3 maxValue=vec3(1.0f), vec3 increment=vec3(0.01f),
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "FloatVec3Property"; }
};
/** class FloatVec4Property
*   \brief The FloatVec4Property class provides a property holding a 4-D vector containing float values.
*
*   A FloatVec4 property are normally used by a processor.
*   The property has a widget consisting of 4 sliders for setting the float values.
* @see OrdinalProperty
*/
class IVW_CORE_API FloatVec4Property : public OrdinalProperty<vec4> {
public:
    /**
     * \brief Constructs a  FloatVec4Property
     *
     * <DESCRIBE THE METHOD>
     *
     * @param std::string identifier <DESCRIBE ME>
     * @param std::string displayName <DESCRIBE ME>
     * @param vec4 value <DESCRIBE ME>
     * @param vec4 minValue <DESCRIBE ME>
     * @param vec4 maxValue <DESCRIBE ME>
     * @param vec4 increment <DESCRIBE ME>
     * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
     * @param PropertySemantics semantics <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */
    FloatVec4Property(std::string identifier, std::string displayName, vec4 value = vec4(0.0f),
                      vec4 minValue=vec4(0.0f), vec4 maxValue= vec4(1.0f), vec4 increment=vec4(0.01f),
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "FloatVec4Property"; }

};
/** class IntMinMaxProperty
*   \brief The IntMinMaxProperty class provides a property holding a 2-D vector containing int values, where x is always less then y.
*
*   A IntMinMax property are normally used by a processor.
*   The property has a widget consisting of 1 range slider for setting the float values.
* @see OrdinalProperty
*/
class IVW_CORE_API IntMinMaxProperty : public MinMaxProperty<int> {
public:
    /**
     * \brief Constructs a IntMinMaxProperty
     */
    IntMinMaxProperty(std::string identifier, std::string displayName,
                      int valueMin = 0, int valueMax = 10, int rangeMin=0, int rangeMax=10, int increment=1,
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& s);
    virtual std::string getClassName()  const { return "IntMinMaxProperty"; }

};
/** class IntVec2Property
*   \brief The IntVec2Property class provides a property holding a 2-D vector containing integer values.
*
*   A IntVec2 property are normally used by a processor.
*   The property has a widget consisting of 2 sliders for setting the integer values.
* @see OrdinalProperty
*/
class IVW_CORE_API IntVec2Property : public OrdinalProperty<ivec2> {
public:
    /**
     * \brief Constructs a IntVec2Property
     *
     * <DESCRIBE THE METHOD>
     *
     * @param std::string identifier <DESCRIBE ME>
     * @param std::string displayName <DESCRIBE ME>
     * @param ivec2 value <DESCRIBE ME>
     * @param ivec2 minValue <DESCRIBE ME>
     * @param ivec2 maxValue <DESCRIBE ME>
     * @param ivec2 increment <DESCRIBE ME>
     * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
     * @param PropertySemantics semantics <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */
    IntVec2Property(std::string identifier, std::string displayName, ivec2 value = ivec2(0),
                    ivec2 minValue=ivec2(0), ivec2 maxValue=ivec2(10), ivec2 increment=ivec2(1),
                    PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                    PropertySemantics semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "IntVec2Property"; }

};
/** class IntVec3Property
*   \brief The IntVec2Property class provides a property holding a 3-D vector containing integer values.
*
*   A IntVec3 property are normally used by a processor.
*   The property has a widget consisting of 3 sliders for setting the integer values.
* @see OrdinalProperty
*/
class IVW_CORE_API IntVec3Property : public OrdinalProperty<ivec3> {
public:
    /**
     * \brief Constructs a IntVec3Property
     *
     * <DESCRIBE THE METHOD>
     *
     * @param std::string identifier <DESCRIBE ME>
     * @param std::string displayName <DESCRIBE ME>
     * @param ivec3 value <DESCRIBE ME>
     * @param ivec3 minValue <DESCRIBE ME>
     * @param ivec3 maxValue <DESCRIBE ME>
     * @param ivec3 increment <DESCRIBE ME>
     * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
     * @param PropertySemantics semantics <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */
    IntVec3Property(std::string identifier, std::string displayName, ivec3 value = ivec3(0),
                    ivec3 minValue=ivec3(0), ivec3 maxValue= ivec3(10), ivec3 increment=ivec3(1),
                    PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                    PropertySemantics semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "IntVec3Property"; }

};
/** class IntVec4Property
*   \brief The IntVec4Property class provides a property holding a 4-D vector containing integer values.
*
*   A IntVec4 property are normally used by a processor.
*   The property has a widget consisting of 4 sliders for setting the integer values.
* @see OrdinalProperty
*/
class IVW_CORE_API IntVec4Property : public OrdinalProperty<ivec4> {
public:
    /**
     * \brief Constructs a IntVec4Property
     *
     * <DESCRIBE THE METHOD>
     *
     * @param std::string identifier <DESCRIBE ME>
     * @param std::string displayName <DESCRIBE ME>
     * @param ivec4 value <DESCRIBE ME>
     * @param ivec4 minValue <DESCRIBE ME>
     * @param ivec4 maxValue <DESCRIBE ME>
     * @param ivec4 increment <DESCRIBE ME>
     * @param PropertyOwner::InvalidationLevel invalidationLevel <DESCRIBE ME>
     * @param PropertySemantics semantics <DESCRIBE ME>
     * @return  <DESCRIBE ME>
     */
    IntVec4Property(std::string identifier, std::string displayName, ivec4 value = ivec4(0),
                    ivec4 minValue=ivec4(0), ivec4 maxValue= ivec4(10), ivec4 increment=ivec4(1),
                    PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                    PropertySemantics semantics = PropertySemantics::Default);
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);
    virtual std::string getClassName()  const { return "IntVec4Property"; }

};



} // namespace

#endif // IVW_VECTORPROPERTIES_H