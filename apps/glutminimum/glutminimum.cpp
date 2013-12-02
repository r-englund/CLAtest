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
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include <modules/opengl/inviwoopengl.h>
#include <GL/glut.h>
#include <modules/glut/canvasglut.h>

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/network/processornetwork.h>
#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/canvasprocessor.h>

#include <moduleregistration.h>


using namespace inviwo;

CanvasGLUT* canvas = 0;
InviwoApplication* app = 0;
ProcessorNetwork* processorNetwork = 0;
ProcessorNetworkEvaluator* processorNetworkEvaluator = 0;


void deinitialize() {
    delete processorNetwork;
    processorNetwork = 0;
    delete processorNetworkEvaluator;
    processorNetworkEvaluator = 0;
    if (app) app->deinitialize();
    delete app;
    app = 0;
}


void keyPressed(unsigned char key, int /*x*/, int /*y*/) {
    switch (key) {
        case 27: // ESC key
            deinitialize();
            exit(0);
            break;
        case '1': {
            Processor* processor = processorNetwork->getProcessorByName("EntryExitPoints");
            FloatProperty* distance = dynamic_cast<FloatProperty*>(processor->getPropertyByIdentifier("viewDist"));
            distance->set(distance->get()+distance->getIncrement());
            break;
        }
        case '2': {
            Processor* processor = processorNetwork->getProcessorByName("EntryExitPoints");
            FloatProperty* distance = dynamic_cast<FloatProperty*>(processor->getPropertyByIdentifier("viewDist"));
            distance->set(distance->get()-distance->getIncrement());     
            break;
        }
    }
}


void keyPressedSpecial(int /*key*/, int /*x*/, int /*y*/) {
}


int main(int argc, char** argv) {
    InviwoApplication inviwoApp(argc, argv, "glutminimum "+IVW_VERSION, IVW_DIR);

    glutInit(&argc, argv);

    canvas = new CanvasGLUT("glutminimum", uvec2(256,256));//, GLCanvas::RGBAD);
    canvas->initializeGL();

    inviwoApp.initialize(&inviwo::registerAllModules);

    // Create process network
    processorNetwork = new ProcessorNetwork();
    inviwoApp.setProcessorNetwork(processorNetwork);

    // Create process network evaluator
    processorNetworkEvaluator = new ProcessorNetworkEvaluator(processorNetwork);
    processorNetworkEvaluator->setDefaultRenderContext(canvas);
    canvas->setNetworkEvaluator(processorNetworkEvaluator);
    canvas->initializeSquare();
    canvas->activate();

    // Load simple scene
    IvwDeserializer xmlDeserializer(inviwoApp.getPath(InviwoApplication::PATH_PROJECT, "tests/simple.inv"));
    processorNetwork->deserialize(xmlDeserializer);
    std::vector<Processor*> processors = processorNetwork->getProcessors();
    for (std::vector<Processor*>::iterator it = processors.begin(); it!=processors.end(); it++) {
        (*it)->initialize();
    }

    // Register Canvas GLUT
    processorNetworkEvaluator->registerCanvas(canvas);

    glutKeyboardFunc(keyPressed);
    glutSpecialFunc(keyPressedSpecial);
    
    glutMainLoop();
    return 0;
}
