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
 * Primary author : Hans-Christian Helltegen
 *
 **********************************************************************/

#ifndef IVW_TRACKBALLACTION_H
#define IVW_TRACKBALLACTION_H

#include <inviwo/core/interaction/action.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API TrackballAction : public Action {
public:
    enum Actions {
        TRACKBALL_ROTATE  =      0,
        TRACKBALL_ZOOM    ,
        TRACKBALL_PAN     ,
        TRACKBALL_STEPROTATE_UP,
        TRACKBALL_STEPROTATE_LEFT,
        TRACKBALL_STEPROTATE_DOWN,
        TRACKBALL_STEPROTATE_RIGHT,
        TRACKBALL_STEPZOOM_IN,
        TRACKBALL_STEPZOOM_OUT,
        TRACKBALL_STEPPAN_UP,
        TRACKBALL_STEPPAN_LEFT,
        TRACKBALL_STEPPAN_DOWN,
        TRACKBALL_STEPPAN_RIGHT,
        COUNT
    };

    TrackballAction(TrackballAction::Actions action);
    ~TrackballAction();

    virtual std::string getClassName() const { return "TrackballAction"; }

    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

private:
    std::string actionNames_[COUNT];
};

} // namespace

#endif // IVW_TRACKBALLACTION_H