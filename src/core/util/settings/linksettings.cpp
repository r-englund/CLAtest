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

namespace inviwo {

LinkSettings::LinkSettings(std::string id) : Settings(id) {}

LinkSettings::~LinkSettings() {
}

void LinkSettings::initialize() {
    addProperty(new BoolProperty("displayLinks", "Display links", true));
    InviwoCore* module = InviwoApplication::getPtr()->getModuleByType<InviwoCore>();
    ivwAssert(module!=0, "Core module is not yet registered")
    std::vector<Property*> registeredProperties = module->getProperties();
    for (size_t i=0; i<registeredProperties.size(); i++) {
        std::string id = registeredProperties[i]->getIdentifier();
        std::string displayName = registeredProperties[i]->getDisplayName();
        addProperty(new BoolProperty("link"+id, displayName, false));
    }
}

void LinkSettings::deinitialize()  {}

bool LinkSettings::isLinkable(Property* property)  {
    Property* prop = 0;
    BoolProperty* linkOption = 0;

    //camera
    prop = getPropertyByIdentifier("linkcamera");
    ivwAssert(prop!=0, "Camera Property link option not found in settings");
    linkOption = dynamic_cast<BoolProperty*>( prop );
    bool linkCameraProperty = linkOption->get();
    if (linkCameraProperty && dynamic_cast<CameraProperty*>(property))
        return true;    

    //transfer functions
    prop = getPropertyByIdentifier("linktransferfunction");
    ivwAssert(prop!=0, "TransferFunction Property link option not found in settings");
    linkOption = dynamic_cast<BoolProperty*>( prop );
    bool linkTfProperty = linkOption->get();
    if (linkTfProperty && dynamic_cast<TransferFunctionProperty*>(property))
        return true;    

    //options
    prop = getPropertyByIdentifier("linkstringoptions");
    ivwAssert(prop!=0, "Options Property link option not found in settings");
    linkOption = dynamic_cast<BoolProperty*>( prop );
    bool linkOptionProperty = linkOption->get();
    if (linkOptionProperty && dynamic_cast<BaseOptionProperty*>(property))
        return true;


    return false;
}


} // namespace
