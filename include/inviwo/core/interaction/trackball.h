/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Hans-Christian Helltegen
 *
 **********************************************************************/

#ifndef IVW_TRACKBALL_H
#define IVW_TRACKBALL_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/interaction/interactionhandler.h>
#include <inviwo/core/interaction/events/event.h>
#include <inviwo/core/interaction/events/mouseevent.h>
#include <inviwo/core/interaction/events/keyboardevent.h>
#include <inviwo/core/interaction/events/eventlistener.h>
#include <inviwo/core/interaction/events/resizeevent.h>
#include <inviwo/core/io/serialization/ivwserializable.h>
#include <inviwo/core/properties/cameraproperty.h>
#include <inviwo/core/properties/propertyowner.h>

#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/core/interaction/events/event.h>
#include <inviwo/core/interaction/trackballaction.h>

namespace inviwo {

class IVW_CORE_API Trackball : public InteractionHandler, public PropertyOwner {

public:
    Trackball(CameraProperty* camera);
    ~Trackball();

    virtual void invokeEvent(Event* event);
    void addProperty(Property& property);
    void addProperty(Property* property);

	virtual std::string getClassName() const { return "Trackball"; }

	void serialize(IvwSerializer &s) const;
	void deserialize(IvwDeserializer &d);

private:
	enum Direction {
		UP = 0,
		LEFT,
		DOWN,
		RIGHT
	};

    float pixelWidth_;
    bool isMouseBeingPressedAndHold_;

    vec2 lastMousePos_;
    vec3 lastTrackballPos_;

    CameraProperty* camera_;



    MouseEvent rotateEvent_;
    MouseEvent zoomEvent_;
    MouseEvent panEvent_;

    KeyboardEvent stepRotateUpEvent_;
    KeyboardEvent stepRotateLeftEvent_;
    KeyboardEvent stepRotateDownEvent_;
    KeyboardEvent stepRotateRightEvent_;

    KeyboardEvent stepZoomInEvent_;
    KeyboardEvent stepZoomOutEvent_;
    KeyboardEvent stepPanUpEvent_;
    KeyboardEvent stepPanLeftEvent_;
    KeyboardEvent stepPanDownEvent_;
    KeyboardEvent stepPanRightEvent_;



    TrackballAction rotateAction_;
    TrackballAction zoomAction_;
    TrackballAction panAction_;

    TrackballAction stepRotateUpAction_;
    TrackballAction stepRotateLeftAction_;
    TrackballAction stepRotateDownAction_;
    TrackballAction stepRotateRightAction_;

    TrackballAction stepZoomInAction_;
    TrackballAction stepZoomOutAction_;
    TrackballAction stepPanUpAction_;
    TrackballAction stepPanLeftAction_;
    TrackballAction stepPanDownAction_;
    TrackballAction stepPanRightAction_;


    EventProperty rotateEventProperty_;
    EventProperty zoomEventProperty_;
    EventProperty panEventProperty_;

    EventProperty stepRotateUpProperty_;
    EventProperty stepRotateLeftProperty_;
    EventProperty stepRotateDownProperty_;
    EventProperty stepRotateRightProperty_;

    EventProperty stepZoomInProperty_;
    EventProperty stepZoomOutProperty_;
    EventProperty stepPanUpProperty_;
    EventProperty stepPanLeftProperty_;
    EventProperty stepPanDownProperty_;
    EventProperty stepPanRightProperty_;




    vec3 mapNormalizedMousePosToTrackball(vec2 mousePos);
    vec3 mapToCamera(vec3 pos);
    void rotateCamera(MouseEvent* mouseEvent);
    void zoomCamera(MouseEvent* mouseEvent);
    void panCamera(MouseEvent* mouseEvent);
	void stepRotateCamera(Direction dir);
	void stepZoomCamera(Direction dir);
	void stepPanCamera(Direction dir);
};

} // namespace

#endif // IVW_TRACKBALL_H