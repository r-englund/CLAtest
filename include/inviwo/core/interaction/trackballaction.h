#ifndef IVW_TRACKBALLACTION_H
#define IVW_TRACKBALLACTION_H

#include <inviwo/core/interaction/action.h>
#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

class IVW_CORE_API TrackballAction : public Action {
public:
    enum Action {
        TRACKBALL_ROTATE  =      0,
        TRACKBALL_ZOOM    ,
        TRACKBALL_PAN     ,
        COUNT
    };

    TrackballAction(TrackballAction::Action action);
    ~TrackballAction();

	virtual void serialize(IvwSerializer& s) const;
	virtual void deserialize(IvwDeserializer& d);

private:
    std::string actionNames_[COUNT];
};

} // namespace

#endif // IVW_TRACKBALLACTION_H