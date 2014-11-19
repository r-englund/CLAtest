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
 * Contact: Erik Sundén
 *
 *********************************************************************************/

#ifdef _MSC_VER
#pragma comment(linker, "/SUBSYSTEM:CONSOLE")
#endif

#include <QApplication>
#include <QMainWindow>

#include <modules/opengl/inviwoopengl.h>
#include <modules/openglqt/canvasprocessorwidgetqt.h>
#include <modules/openglqt/canvasqt.h>

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/core/network/processornetwork.h>
#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/canvasprocessor.h>
#include <inviwo/core/util/filesystem.h>
#include <moduleregistration.h>

using namespace inviwo;

int main(int argc, char** argv) {
#ifdef __unix__
    setenv("XLIB_SKIP_ARGB_VISUALS", "1", 1);
#endif
    std::string appName = "Inviwo " + IVW_VERSION + " - QtApp";

    QApplication app(argc, argv);
    app.setOrganizationName("Inviwo Foundation");
    app.setOrganizationDomain("inviwo.org");
    app.setApplicationName(QString::fromStdString(appName));
    app.setAttribute(Qt::AA_NativeWindows);

    LogCentral::init();
    // LogCentral::getPtr()->registerLogger(new ConsoleLogger());

    InviwoApplication inviwoApp(argc, argv, appName, inviwo::filesystem::findBasePath());

    // Initialize all modules
    inviwoApp.initialize(&inviwo::registerAllModules);

    // Continue initialization of default context
    CanvasQt* sharedCanvas =
        static_cast<CanvasQt*>(inviwoApp.getProcessorNetworkEvaluator()->getDefaultRenderContext());
    sharedCanvas->initialize();
    sharedCanvas->activate();

    // Set canvas as central widget
    QMainWindow mainWin;

    // Load simpleraycaster scene
    inviwoApp.getProcessorNetwork()->lock();
    const CommandLineParser* cmdparser = inviwoApp.getCommandLineParser();
    std::string workspace;

    if (cmdparser->getLoadWorkspaceFromArg())
        workspace = cmdparser->getWorkspacePath();
    else
        workspace =
            inviwoApp.getPath(InviwoApplication::PATH_WORKSPACES, "tests/simpleraycaster.inv");

    IvwDeserializer xmlDeserializer(workspace);
    inviwoApp.getProcessorNetwork()->deserialize(xmlDeserializer);
    std::vector<Processor*> processors = inviwoApp.getProcessorNetwork()->getProcessors();
    int i = 0;

    for (std::vector<Processor*>::iterator it = processors.begin(); it != processors.end(); ++it) {
        (*it)->invalidate(INVALID_RESOURCES);
        CanvasProcessor* canvasProcessor = dynamic_cast<CanvasProcessor*>((*it));

        if (canvasProcessor) {
            CanvasProcessorWidgetQt* canvasWidget = new CanvasProcessorWidgetQt();
            canvasWidget->setProcessor(canvasProcessor);
            canvasWidget->initialize();
            canvasProcessor->getCanvas()->setNetworkEvaluator(inviwoApp.getProcessorNetworkEvaluator());
            canvasProcessor->setProcessorWidget(canvasWidget);
            if (i == 0) mainWin.setCentralWidget(canvasWidget);
            canvasWidget->show();
            dynamic_cast<CanvasGL*>(canvasProcessor->getCanvas())->resize(
                uvec2(canvasProcessor->getCanvasSize().x, canvasProcessor->getCanvasSize().y));
        }
    }

    inviwoApp.getProcessorNetwork()->setModified(true);
    inviwoApp.getProcessorNetwork()->unlock();

    if (cmdparser->getCaptureAfterStartup()) {
        std::string path = cmdparser->getOutputPath();

        if (path.empty()) path = inviwoApp.getPath(InviwoApplication::PATH_IMAGES);

        inviwoApp.getProcessorNetworkEvaluator()->saveSnapshotAllCanvases(
            path, cmdparser->getSnapshotName());
    }

    mainWin.showFullScreen();

    if (cmdparser->getQuitApplicationAfterStartup()) {
        inviwoApp.closeInviwoApplication();
        app.quit();
        return 0;
    }

    return app.exec();
}
