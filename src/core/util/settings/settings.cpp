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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <inviwo/core/util/settings/settings.h>

namespace inviwo {

Settings::Settings(InviwoModule* module, std::string id) : module_(module), identifier_(id) {}

Settings::~Settings() {
}

void Settings::invalidate() {
    PropertyOwner::invalidate(PropertyOwner::INVALID_OUTPUT);
}

void Settings::serialize(IvwSerializer& s) const {
    PropertyOwner::serialize(s);
}

void Settings::deserialize(IvwDeserializer& d) {
    PropertyOwner::deserialize(d);  
}

std::string Settings::getIdentifier() {
    return identifier_;
}

} // namespace
