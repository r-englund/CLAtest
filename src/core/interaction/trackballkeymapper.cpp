#include <inviwo/core/interaction/trackballkeymapper.h>

namespace inviwo {

    // Default key binding
    TrackballKeyMapper::TrackballKeyMapper() {
        map_ = new std::vector<EventProperty*>;
        map_->push_back(new EventProperty("rotate", "Rotate", 
            new MouseEvent(MouseEvent::MOUSE_BUTTON_LEFT, Event::MODIFIER_NONE), 
            new TrackballAction(TrackballAction::TRACKBALL_ROTATE)));
        map_->push_back(new EventProperty("zoom", "Zoom", 
            new MouseEvent(MouseEvent::MOUSE_BUTTON_RIGHT, Event::MODIFIER_NONE), 
            new TrackballAction(TrackballAction::TRACKBALL_ZOOM)));
        map_->push_back(new EventProperty("pan", "Pan", 
            new MouseEvent(MouseEvent::MOUSE_BUTTON_MIDDLE, Event::MODIFIER_NONE), 
            new TrackballAction(TrackballAction::TRACKBALL_PAN)));
    }

    TrackballKeyMapper::~TrackballKeyMapper() {}

    MouseEvent::MouseButton TrackballKeyMapper::getKey( TrackballAction::Action action ) {
        for (size_t i = 0; i < map_->size(); i++){
            if (action == (map_->at(i)->getAction()->action()))
                return (MouseEvent::MouseButton)map_->at(i)->getEvent()->button();
        }

        return MouseEvent::MOUSE_BUTTON_NONE;
    }

    void TrackballKeyMapper::setKey( TrackballAction::Action action, Event e ) {
        for (size_t i = 0; i < map_->size(); i++){
            if (action == (map_->at(i)->getAction()->action()))
                map_->at(i)->setEvent(&e);
        }
    }


    

} // namespace
