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

LinkSettings::LinkSettings(InviwoModule* module, std::string id) : Settings(module, id) {}

LinkSettings::~LinkSettings() {
}

void LinkSettings::initialize() {
    addProperty(new BoolProperty("displayLinks", "Display links", true));
}

void LinkSettings::deinitialize()  {}

} // namespace
