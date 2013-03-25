#ifndef IVW_EVENTMAPPER_H
#define IVW_EVENTMAPPER_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/interaction/events/mouseevent.h>
#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/core/interaction/trackballaction.h>
#include <vector>

namespace inviwo {

    class IVW_CORE_API TrackballKeyMapper {

    public:

        TrackballKeyMapper();
        virtual ~TrackballKeyMapper();

        MouseEvent::MouseButton getKey(TrackballAction::Action action);
        void setKey(TrackballAction::Action action, Event e);
        
    private:
        std::vector<EventProperty*>* map_; 
        std::vector<int>* test_;

    };

} // namespace

#endif // IVW_EVENTMAPPER_H