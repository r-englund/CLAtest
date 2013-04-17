#ifndef IVW_RESIZEEVENT_H
#define IVW_RESIZEEVENT_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/interaction/events/event.h>

namespace inviwo {

    class IVW_CORE_API ResizeEvent : public Event {

    public:        
        ResizeEvent(uvec2 canvasSize);
        ~ResizeEvent();
        inline uvec2 canvasSize() const {return canvasSize_; }

    private:        
        uvec2 canvasSize_;
    };

} // namespace

#endif // IVW_RESIZEEVENT_H