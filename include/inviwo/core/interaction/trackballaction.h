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
		TRACKBALL_STEPROTATE_LEFT,
		TRACKBALL_STEPROTATE_RIGHT,
		TRACKBALL_STEPROTATE_UP,
		TRACKBALL_STEPROTATE_DOWN,
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