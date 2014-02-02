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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/properties/optionproperties.h>

namespace inviwo {

OptionPropertyInt::OptionPropertyInt(std::string identifier, std::string displayName,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics semantics)
    : TemplateOptionProperty<int>(identifier, displayName, 0, invalidationLevel, semantics)
{}

OptionPropertyInt::OptionPropertyInt(std::string identifier, std::string displayName, int value,
                                     PropertyOwner::InvalidationLevel invalidationLevel,
                                     PropertySemantics semantics)
    : TemplateOptionProperty<int>(identifier, displayName, value, invalidationLevel, semantics)
{}

OptionPropertyFloat::OptionPropertyFloat(std::string identifier, std::string displayName,
        PropertyOwner::InvalidationLevel invalidationLevel,
        PropertySemantics semantics)
    : TemplateOptionProperty<float>(identifier, displayName, 0.0f, invalidationLevel, semantics)
{}

OptionPropertyFloat::OptionPropertyFloat(std::string identifier, std::string displayName, float value,
        PropertyOwner::InvalidationLevel invalidationLevel,
        PropertySemantics semantics)
    : TemplateOptionProperty<float>(identifier, displayName, value, invalidationLevel, semantics)
{}

OptionPropertyDouble::OptionPropertyDouble(std::string identifier, std::string displayName,
        PropertyOwner::InvalidationLevel invalidationLevel,
        PropertySemantics semantics)
    : TemplateOptionProperty<double>(identifier, displayName, 0.0, invalidationLevel, semantics)
{}

OptionPropertyDouble::OptionPropertyDouble(std::string identifier, std::string displayName, double value,
        PropertyOwner::InvalidationLevel invalidationLevel,
        PropertySemantics semantics)
    : TemplateOptionProperty<double>(identifier, displayName, value, invalidationLevel, semantics)
{}

OptionPropertyString::OptionPropertyString(std::string identifier, std::string displayName,
        PropertyOwner::InvalidationLevel invalidationLevel,
        PropertySemantics semantics)
    : TemplateOptionProperty<std::string>(identifier, displayName, identifier, invalidationLevel, semantics)
{}

OptionPropertyString::OptionPropertyString(std::string identifier, std::string displayName, std::string value,
        PropertyOwner::InvalidationLevel invalidationLevel,
        PropertySemantics semantics)
    : TemplateOptionProperty<std::string>(identifier, displayName, value, invalidationLevel, semantics)
{}

void OptionPropertyString::addOption(std::string identifier, std::string displayName) {
    TemplateOptionProperty<std::string>::addOption(identifier, displayName, identifier);
}

void OptionPropertyString::addOption(std::string identifier, std::string displayName, std::string value) {
    TemplateOptionProperty<std::string>::addOption(identifier, displayName, value);
}

} // namespace
