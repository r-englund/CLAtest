#ifndef IVW_CANVAS_H
#define IVW_CANVAS_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/network/processornetworkevaluator.h"
#include "inviwo/core/interaction/events/eventhandler.h"
#include "inviwo/core/interaction/events/keyboardevent.h"
#include "inviwo/core/interaction/events/mouseevent.h"
#include "inviwo/core/interaction/events/timerevent.h"

namespace inviwo {

class Canvas {
public:
    Canvas(ivec2 dimensions);
    virtual ~Canvas();

    virtual void initialize();
    virtual void deinitialize();

    void setNetworkEvaluator(ProcessorNetworkEvaluator* networkEvaluator) { networkEvaluator_ = networkEvaluator; };

    static ProcessorNetworkEvaluator* networkEvaluator_; //TODO: should this rather be private?

protected:
    ivec2 dimensions_;
};

} // namespace

#endif // IVW_CANVAS_H
