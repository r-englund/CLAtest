#ifdef WIN32
#include <windows.h>
#endif

#include <modules/opengl/inviwoopengl.h>
#include <GL/glut.h>
#include <modules/glut/canvasglut.h>

#include <inviwo/core/inviwo.h>
#include <inviwo/core/inviwoapplication.h>
#include <inviwo/core/network/processornetwork.h>
#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/processors/canvasprocessor.h>


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
            distance->increase();
            break;
        }
        case '2': {
            Processor* processor = processorNetwork->getProcessorByName("EntryExitPoints");
            FloatProperty* distance = dynamic_cast<FloatProperty*>(processor->getPropertyByIdentifier("viewDist"));
            distance->decrease();            
            break;
        }
    }
}


void keyPressedSpecial(int /*key*/, int /*x*/, int /*y*/) {
}


int main(int argc, char** argv) {
    InviwoApplication inviwoApp("glutminimum "+IVW_VERSION, IVW_DIR);
    inviwoApp.initialize();

    glutInit(&argc, argv);

    canvas = new CanvasGLUT("glutminimum", ivec2(256,256));//, GLCanvas::RGBAD);
    canvas->initialize();
    
    glewInit();

    // Create process network
    processorNetwork = new ProcessorNetwork();
    inviwoApp.setProcessorNetwork(processorNetwork);

    // Create process network evaluator
    processorNetworkEvaluator = new ProcessorNetworkEvaluator(processorNetwork);
    processorNetworkEvaluator->setDefaultRenderContext(canvas);
    canvas->setNetworkEvaluator(processorNetworkEvaluator);

    // Load simple scene
    IvwDeserializer xmlDeserializer(inviwoApp.getPath(InviwoApplication::PATH_PROJECT, "simple.inv"));
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
