#ifndef IVW_PROCESSORNETWORKEVALUATOR_H
#define IVW_PROCESSORNETWORKEVALUATOR_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/interaction/events/mouseevent.h"
#include "inviwo/core/network/processornetwork.h"

namespace inviwo {

class Canvas;

class ProcessorNetworkEvaluator {

friend Processor;

public:
    ProcessorNetworkEvaluator(ProcessorNetwork* processorNetwork);
    virtual ~ProcessorNetworkEvaluator();

    void setProcessorNetwork(ProcessorNetwork* processorNetwork);
    void registerCanvas(Canvas* canvas, unsigned int canvasNum=0);
    void initializeNetwork();

    void evaluate();
    void propagateMouseEvent(Canvas* canvas, MouseEvent* event);

private:
    bool hasBeenVisited(Processor* processor);
    std::vector<Processor*> getDirectPredecessors(Processor* processor);
    void traversePredecessors(Processor* processor);
    void sortTopologically();

    void propagateMouseEvent(Processor* processor, MouseEvent* mouseEvent);

    ProcessorNetwork* processorNetwork_;

    std::vector<Processor*> processorsSorted_; // the sorted list of processors obtained through topological sorting
    std::vector<Processor*> processorsVisited_; // a bool vector containing flags whether a processor has been visited during traversal

    std::vector<Canvas*> registeredCanvases_;
};

} // namespace

#endif // IVW_PROCESSORNETWORKEVALUATOR_H
