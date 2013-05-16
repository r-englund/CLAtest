#ifndef IVW_PROCESSORNETWORKEVALUATOR_H
#define IVW_PROCESSORNETWORKEVALUATOR_H

#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/interaction/events/mouseevent.h>
#include <inviwo/core/interaction/events/resizeevent.h>
#include <inviwo/core/network/processornetwork.h>
#include <inviwo/core/properties/linkevaluator.h>

namespace inviwo {

class Canvas;

class ProcessorNetworkEvaluator {

friend class Processor;

public:
    ProcessorNetworkEvaluator(ProcessorNetwork* processorNetwork);
    virtual ~ProcessorNetworkEvaluator();

    void registerCanvas(Canvas* canvas, std::string associatedProcessName="CanvasProcessorGL");
    void deregisterCanvas(Canvas* canvas);
    void saveSnapshotAllCanvases(std::string dir, std::string default_name = "", std::string ext = ".png");
    std::vector<Canvas*> getRegisteredCanvases() { return registeredCanvases_; }
    void setDefaultRenderContext(Canvas* canvas) { defaultContext_ = canvas; }
    void activateDefaultRenderContext();
    void initializeNetwork();

    void evaluatePropertyLinks(Property* sourceProperty);    
    void evaluate();
    void propagateMouseEvent(Canvas* canvas, MouseEvent* event);
    void propagateResizeEvent(Canvas* canvas, ResizeEvent* resizeEvent);
    Processor* retrieveCanvasProcessor(Canvas* canvas);

private:
    bool hasBeenVisited(Processor* processor);
    bool hasBeenVisited(Property* property);
    std::vector<Processor*> getDirectPredecessors(Processor* processor);
    void traversePredecessors(Processor* processor);
    void determineProcessingOrder();

    void propagateMouseEvent(Processor* processor, MouseEvent* mouseEvent);
    void propagateResizeEvent(Processor* processor, ResizeEvent* resizeEvent);

    bool isPortConnectedToProcessor(Port* port, Processor *processor);

    //Property Linking support
    std::vector<PropertyLink*> getConnectedPropertyLinks(Property* property);
    std::vector<Property*> getLinkedProperties(Property* property);
    void evaluatePropertyLinks(Property* sourceProperty, Property* curProperty);

    ProcessorNetwork* processorNetwork_;

    std::vector<Processor*> processorsSorted_; // the sorted list of processors obtained through topological sorting
    std::vector<Processor*> processorsVisited_; // a bool vector containing flags whether a processor has been visited during traversal
    std::vector<Property*> propertiesVisited_;

    std::vector<Canvas*> registeredCanvases_;
    Canvas* defaultContext_;
    LinkEvaluator* linkEvaluator_;
    Processor* eventInitiator_; 
};

} // namespace

#endif // IVW_PROCESSORNETWORKEVALUATOR_H
