#ifndef IVW_PROCESSORNETWORKEVALUATOR_H
#define IVW_PROCESSORNETWORKEVALUATOR_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/interaction/events/mouseevent.h"
#include "inviwo/core/interaction/events/resizeevent.h"
#include "inviwo/core/network/processornetwork.h"
#include "inviwo/core/properties/linkevaluator.h"

namespace inviwo {

class Canvas;

class ProcessorNetworkEvaluator {

friend class Processor;

public:
    ProcessorNetworkEvaluator(ProcessorNetwork* processorNetwork);
    virtual ~ProcessorNetworkEvaluator();

    void setProcessorNetwork(ProcessorNetwork* processorNetwork);
    void registerCanvas(Canvas* canvas, std::string associatedProcessName="CanvasProcessorGL");
    void deRegisterCanvas(Canvas *canvas);
    void setDefaultRenderContext(Canvas* canvas) {renderContext_ = canvas;}
    void initializeNetwork();

    void evaluatePropertyLinks(Property* sourceProperty);    
    void evaluate();
    void propagateMouseEvent(Canvas* canvas, MouseEvent* event);
    void propagateResizeEvent(Canvas* canvas, ResizeEvent* resizeEvent);

    bool repaintRequired() { return repaintRequired_; }

private:
    bool hasBeenVisited(Processor* processor);
    bool hasBeenVisited(Property* property);
    std::vector<Processor*> getDirectPredecessors(Processor* processor);
    void traversePredecessors(Processor* processor);
    void sortTopologically();

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
    Canvas* renderContext_;
    LinkEvaluator* linkEvaluator_;
 
    bool repaintRequired_;

    static const std::string logSource_; ///< Source string to be displayed for log messages.
};

} // namespace

#endif // IVW_PROCESSORNETWORKEVALUATOR_H
