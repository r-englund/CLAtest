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

#ifndef IVW_LINKSETTINGS_H
#define IVW_LINKSETTINGS_H

#include <inviwo/core/util/settings/settings.h>
#include <inviwo/core/properties/boolproperty.h>

namespace inviwo {

class IVW_CORE_API LinkSettings : public Settings {

public:
    LinkSettings(std::string id="Link Settings");
    virtual ~LinkSettings();
    virtual void initialize();
    virtual void deinitialize();
    virtual bool isLinkable(Property* property);

private:
    BoolProperty displayLinksproerty_;
    std::vector<BoolProperty*> linkProperties_;
};

} // namespace

#endif // IVW_LINKSETTINGS_H
