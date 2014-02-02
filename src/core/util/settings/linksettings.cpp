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

#include <inviwo/core/util/settings/linksettings.h>
#include <inviwo/core/common/inviwocore.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/propertyfactory.h>

namespace inviwo {

LinkSettings::LinkSettings(std::string id) :
    Settings(id)
    , displayLinksproerty_("displayLinks", "Display links", true) {}

LinkSettings::~LinkSettings() {
    for (size_t i=0; i<linkProperties_.size(); i++) {
        BoolProperty* property = linkProperties_[i];
        delete property;
    }

    linkProperties_.clear();
}

void LinkSettings::initialize() {
    addProperty(&displayLinksproerty_);
    std::vector<std::string> properties = PropertyFactory::getPtr()->getRegistedPropertyClassNames();
    std::sort(properties.begin(), properties.end());

    for (size_t i = 0; i<properties.size(); i++) {
        BoolProperty* linkPropery = new BoolProperty("link-" + properties[i], properties[i], false);
        linkProperties_.push_back(linkPropery);
        linkPropery->setGroupID("auto-link-properties");
        linkPropery->setGroupDisplayName("auto-link-properties", "Auto Link Properties");
        addProperty(linkPropery);
    }
}

void LinkSettings::deinitialize()  {}

bool LinkSettings::isLinkable(Property* property)  {
    Property* prop = getPropertyByIdentifier("link-" + property->getClassName());

    if (prop) {
        BoolProperty* linkOption = dynamic_cast<BoolProperty*>(prop);

        if (linkOption)
            return linkOption->get();
    }

    return false;
}


} // namespace
