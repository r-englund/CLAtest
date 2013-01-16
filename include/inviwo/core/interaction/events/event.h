#ifndef IVW_EVENT_H
#define IVW_EVENT_H

namespace inviwo {

    class Event {
    public:

        enum Modifier {
            MODIFIER_NONE  =      0,
            MODIFIER_ALT   = 1 << 1,
            MODIFIER_CTRL  = 1 << 2,
            MODIFIER_SHIFT = 1 << 3
        };

        Event();
        virtual ~Event();
    };

} // namespace

#endif // IVW_EVENT_H