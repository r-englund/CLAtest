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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#include <inviwo/core/interaction/events/resizeevent.h>

namespace inviwo {

    ResizeEvent::ResizeEvent( uvec2 canvasSize)
        : Event(),
          size_(canvasSize),
          previousSize_(canvasSize){}

    ResizeEvent::~ResizeEvent() {}

} // namespace