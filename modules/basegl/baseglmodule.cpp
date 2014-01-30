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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <modules/basegl/baseglmodule.h>

#include <modules/basegl/processors/background.h>
#include <modules/basegl/processors/cubeproxygeometry.h>
#include <modules/basegl/processors/drawlinesprocessor.h>
#include <modules/basegl/processors/entryexitpoints.h>
#include <modules/basegl/processors/findedges.h>
#include <modules/basegl/processors/firstivwprocessor.h>
#include <modules/basegl/processors/imageclassify.h>
#include <modules/basegl/processors/imagegrayscale.h>
#include <modules/basegl/processors/imagemixer.h>
#include <modules/basegl/processors/lightingraycaster.h>
#include <modules/basegl/processors/lightvolumegl.h>
#include <modules/basegl/processors/positionwidgetprocessor.h>
#include <modules/basegl/processors/redgreenprocessor.h>
#include <modules/basegl/processors/simpleraycaster.h>
#include <modules/basegl/processors/volumeslicegl.h>

namespace inviwo {

BaseGLModule::BaseGLModule() : InviwoModule() {
    setIdentifier("BaseGL");
    setXMLFileName("basegl/baseglmodule.xml");

	registerProcessor(Background);
	registerProcessor(CubeProxyGeometry);
    registerProcessor(DrawLinesProcessor);
    registerProcessor(EntryExitPoints);
	registerProcessor(FindEdges);
	registerProcessor(FirstIvwProcessor);
    registerProcessor(ImageClassify);
	registerProcessor(ImageGrayscale);
    registerProcessor(ImageMixer);
    registerProcessor(LightingRaycaster);
	registerProcessor(LightVolumeGL);
    registerProcessor(PositionWidgetProcessor);
    registerProcessor(RedGreenProcessor);
    registerProcessor(SimpleRaycaster);
    registerProcessor(VolumeSliceGL);
}

} // namespace
             