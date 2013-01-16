#ifndef IVW_RESIZEEVENT_H
#define IVW_RESIZEEVENT_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/interaction/events/event.h"

namespace inviwo {

    class ResizeEvent : public Event {

    public:        
        ResizeEvent(ivec2 canvasSize);
        ~ResizeEvent();
        inline ivec2 canvasSize() const {return canvasSize_; }

    private:        
        ivec2 canvasSize_;
    };

} // namespace

#endif // IVW_RESIZEEVENT_H