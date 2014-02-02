/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 *
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_OPTIONPROPERTIES_H
#define IVW_OPTIONPROPERTIES_H

#include <inviwo/core/properties/baseoptionproperty.h>

namespace inviwo {

/** class OptionPropertyInt
*   \brief  The OptionPropertyInt is a property holding different options stored with a display name and the integer value
*   The widget for the property consist of a combobox with the display names for each mapped value
* @see TemplateOptionProperty
*/
class IVW_CORE_API OptionPropertyInt : public TemplateOptionProperty<int> {

public:
    OptionPropertyInt(std::string identifier, std::string displayName,
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics semantics=PropertySemantics::Default);
    OptionPropertyInt(std::string identifier, std::string displayName, int value,
                      PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                      PropertySemantics semantics=PropertySemantics::Default);
    /**
     * \brief  Variants are used while linking. All options are converted to strings (including values) and hence variant type is string.
     *
     *  values must have overloaded streaming operator <<
     *
     * @return inviwo::Variant variant
     */
    virtual Variant getVariant();
    /**
     * \brief Variants are used while linking. All options are converted to strings (including values) and hence variant type is string.
     *
     * values must have overloaded streaming operator >>
     *
     * @param const Variant & input source variant
     */
    virtual void setVariant(const Variant& inVariant);

    virtual std::string getClassName()  const { return "OptionPropertyInt"; }
};


/** class OptionPropertyFloat
* \brief The OptionPropertyFloat is a property holding different options stored with a display name and the float value
*
*  The widget for the property consist of a combobox with the display names for each mapped value
* @see TemplateOptionProperty
*/
class IVW_CORE_API OptionPropertyFloat : public TemplateOptionProperty<float> {

public:
    OptionPropertyFloat(std::string identifier, std::string displayName,
                        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                        PropertySemantics semantics=PropertySemantics::Default);
    OptionPropertyFloat(std::string identifier, std::string displayName, float value,
                        PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                        PropertySemantics semantics=PropertySemantics::Default);
    /**
     * \brief  Variants are used while linking. All options are converted to strings (including values) and hence variant type is string.
     *
     *  values must have overloaded streaming operator <<
     *
     * @return inviwo::Variant variant
     */
    virtual Variant getVariant();
    /**
     * \brief Variants are used while linking. All options are converted to strings (including values) and hence variant type is string.
     *
     * values must have overloaded streaming operator >>
     *
     * @param const Variant & input source variant
     */
    virtual void setVariant(const Variant& inVariant);

    virtual std::string getClassName()  const { return "OptionPropertyFloat"; }
};

/** class OptionPropertyDouble
* \brief The OptionPropertyDouble is a property holding different options stored with a display name and the double value
*
*  The widget for the property consist of a combobox with the display names for each mapped value
* @see TemplateOptionProperty
*/
class IVW_CORE_API OptionPropertyDouble : public TemplateOptionProperty<double> {

public:
    OptionPropertyDouble(std::string identifier, std::string displayName,
                         PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                         PropertySemantics semantics=PropertySemantics::Default);
    OptionPropertyDouble(std::string identifier, std::string displayName, double value,
                         PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                         PropertySemantics semantics=PropertySemantics::Default);
    /**
     * \brief  Variants are used while linking. All options are converted to strings (including values) and hence variant type is string.
     *
     *  values must have overloaded streaming operator <<
     *
     * @return inviwo::Variant variant
     */
    virtual Variant getVariant();
    /**
     * \brief Variants are used while linking. All options are converted to strings (including values) and hence variant type is string.
     *
     * values must have overloaded streaming operator >>
     *
     * @param const Variant & input source variant
     */
    virtual void setVariant(const Variant& inVariant);

    virtual std::string getClassName()  const { return "OptionPropertyDouble"; }
};


/** class OptionPropertyString
* \brief The OptionPropertyString is a property holding different options stored with a display name and the string value
* The widget for the property consist of a combobox with the display names for each mapped value
* @see TemplateOptionProperty
*/
class IVW_CORE_API OptionPropertyString : public TemplateOptionProperty<std::string> {

public:
    OptionPropertyString(std::string identifier, std::string displayName, std::string value,
                         PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                         PropertySemantics semantics=PropertySemantics::Default);
    OptionPropertyString(std::string identifier, std::string displayName,
                         PropertyOwner::InvalidationLevel invalidationLevel=PropertyOwner::INVALID_OUTPUT,
                         PropertySemantics semantics=PropertySemantics::Default);
    /**
     * \brief  Variants are used while linking. All options are converted to strings (including values) and hence variant type is string.
     *
     *  values must have overloaded streaming operator <<
     *
     * @return inviwo::Variant variant
     */
    virtual Variant getVariant();
    /**
     * \brief Variants are used while linking. All options are converted to strings (including values) and hence variant type is string.
     *
     * values must have overloaded streaming operator >>
     *
     * @param const Variant & input source variant
     */
    virtual void setVariant(const Variant& inVariant);
    void addOption(std::string identifier, std::string displayName);
    void addOption(std::string identifier, std::string displayName, std::string value);

    virtual std::string getClassName()  const { return "OptionPropertyString"; }
};

typedef OptionPropertyDouble DoubleOptionProperty; // deprecated
typedef OptionPropertyFloat  FloatOptionProperty;  // deprecated
typedef OptionPropertyInt    IntOptionProperty;    // deprecated
typedef OptionPropertyString StringOptionProperty; // deprecated

} // namespace

#endif // IVW_OPTIONPROPERTIES_H