/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file authors: Erik Sundén
 *
 *********************************************************************************/

#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif

#ifdef WIN32
#include <windows.h>
#endif

#include <modules/opengl/inviwoopengl.h>
#include <modules/glfw/canvasglfw.h>

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/network/processornetwork.h>
#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/util/filesystem.h>
#include <modules/base/processors/imageexport.h>
#include <moduleregistration.h>

using namespace inviwo;

int main(int argc, char** argv) {
    inviwo::ConsoleLogger consoleLogger;
    inviwo::LogCentral::instance()->registerLogger(&consoleLogger);

    InviwoApplication inviwoApp(argc, argv, "Inviwo "+IVW_VERSION + " - GLFWApp", inviwo::filesystem::findBasePath());

    if(!glfwInit()){
        LogErrorCustom("GLFWMinimum", "GLFW could not be initialized.");
        return 0;
    }

    CanvasGLFW* canvas = new CanvasGLFW(inviwoApp.getDisplayName(), uvec2(128,128));
    canvas->initializeGL();
    inviwoApp.initialize(&inviwo::registerAllModules);

    // Create process network
    ProcessorNetwork processorNetwork;
    inviwoApp.setProcessorNetwork(&processorNetwork);

    // Create process network evaluator
    ProcessorNetworkEvaluator processorNetworkEvaluator(&processorNetwork);
    processorNetworkEvaluator.setDefaultRenderContext(canvas);

    canvas->setNetworkEvaluator(&processorNetworkEvaluator);
    canvas->initializeSquare();
    canvas->initialize();
    canvas->activate();

    // Load simple scene
    processorNetworkEvaluator.disableEvaluation();
    processorNetwork.lock();
    const CommandLineParser* cmdparser = (inviwo::InviwoApplication::getRef()).getCommandLineParser();
    std::string workspace;

    if (cmdparser->getLoadWorkspaceFromArg())
        workspace = cmdparser->getWorkspacePath();
    else
        workspace = inviwoApp.getPath(InviwoApplication::PATH_WORKSPACES, "tests/simpleslicergl.inv");

    IvwDeserializer xmlDeserializer(workspace);
    processorNetwork.deserialize(xmlDeserializer);
    std::vector<Processor*> processors = processorNetwork.getProcessors();
    int i=0;

    for (std::vector<Processor*>::iterator it = processors.begin(); it!=processors.end(); it++) {
        (*it)->invalidate(PropertyOwner::INVALID_RESOURCES);
        CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>((*it));

        if (canvasProcessor) {
            CanvasGLFW* currentC;
            if (i==0) {
                currentC = canvas;
            }
            else {
                currentC = new CanvasGLFW(canvasProcessor->getIdentifier(), uvec2(canvasProcessor->getCanvasSize()));
                currentC->initializeGL();
                currentC->setNetworkEvaluator(&processorNetworkEvaluator);
                currentC->initialize();
            }
            processorNetworkEvaluator.registerCanvas(currentC, canvasProcessor->getIdentifier());
            currentC->setWindowTitle(inviwoApp.getDisplayName() + " : " + canvasProcessor->getIdentifier());
            currentC->setWindowSize(uvec2(canvasProcessor->getCanvasSize()));

            i++;
        }
    }

    processorNetwork.setModified(true);
    processorNetwork.unlock();
    processorNetworkEvaluator.enableEvaluation();

    if (cmdparser->getCaptureAfterStartup()) {
        std::string path = cmdparser->getOutputPath();

        if (path.empty())
            path = InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_IMAGES);

        processorNetworkEvaluator.saveSnapshotAllCanvases(path, cmdparser->getSnapshotName());
    }

    if (cmdparser->getQuitApplicationAfterStartup()){
        glfwTerminate();
        return 0;
    }

    while (CanvasGLFW::getWindowCount()>0)
    {
        glfwWaitEvents();
    }

    glfwTerminate();

    if(i==0)
        delete canvas;

    return 0;
}
