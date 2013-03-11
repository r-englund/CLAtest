#ifndef IVW_EVENTMAPPER_H
#define IVW_EVENTMAPPER_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/interaction/events/mouseevent.h>
#include <string>


namespace inviwo {

    class IVW_CORE_API TrackballKeyMapper {

    public:
        /**
        * An enum containing all actions to be mapped.
        */
        enum Action {
            TRACKBALL_ROTATE  =      0,
            TRACKBALL_ZOOM    ,
            TRACKBALL_PAN     ,
            COUNT
        };

        /** 
         * \brief The constructor.
         * The constructor which sets the default key binding
         */
        TrackballKeyMapper();
        virtual ~TrackballKeyMapper();

        /** 
         * \brief Get method.
         * Used to get the key mapped to a certain action.
         * @param int action The action we want a key for.
         * @return int The key mapped to the action.
         */
        int getKey(int action);

        /** 
         * \brief Set method.
         * Used to change the key mapping. Maps the action to the key.
         * @param int action The action to be mapped.
         * @param int key The key to map to an action.
         * @return void
         */
        void setKey (int action, int key);

        std::string getActionName(int action);
        
    private:
        int keys[COUNT]; ///< The array containing all mapping.
        std::string actionNames[COUNT];

    };

} // namespace

#endif // IVW_EVENTMAPPER_H