#include <inviwo/core/interaction/trackballaction.h>

namespace inviwo {

    TrackballAction::TrackballAction( TrackballAction::Action action ) {
        actionNames_[TRACKBALL_ROTATE] = "Rotate";
        actionNames_[TRACKBALL_ZOOM] = "Zoom";
        actionNames_[TRACKBALL_PAN] = "Pan";

        action_ = action;
        actionName_ = actionNames_[action];
    }

    TrackballAction::~TrackballAction() {}

} //namespace