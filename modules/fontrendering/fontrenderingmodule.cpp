/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Johan Noren
 *
 **********************************************************************/

#include <modules/fontrendering/fontrenderingmodule.h>

#include <modules/fontrendering/processors/imageoverlay.h>

namespace inviwo {

FontrenderingModule::FontrenderingModule() : InviwoModule() {
    setIdentifier("Fontrendering");
    setXMLFileName("fontrendering/fontrenderingmodule.xml");

	registerProcessor(ImageOverlay);
}

} // namespace
