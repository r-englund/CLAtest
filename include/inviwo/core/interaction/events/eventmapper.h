#ifndef IVW_EVENTMAPPER_H
#define IVW_EVENTMAPPER_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/interaction/events/mouseevent.h>


namespace inviwo {

    class IVW_CORE_API EventMapper {

    public:
        enum Action {
            TRACKBALL_ROTATE  =      0,
            TRACKBALL_ZOOM    = 1 << 1,
            TRACKBALL_PAN     = 1 << 2,
            COUNT
        };

        EventMapper();
        virtual ~EventMapper();
        int getKey(int action);
        void setKey (int action, int key);
        
    private:
        int keys[COUNT];

    };

} // namespace

#endif // IVW_EVENTMAPPER_H