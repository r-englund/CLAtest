#ifndef IVW_CANVAS_H
#define IVW_CANVAS_H

#include <inviwo/core/inviwocoredefine.h>
#include <inviwo/core/inviwo.h>
#include <inviwo/core/network/processornetworkevaluator.h>
#include <inviwo/core/interaction/events/eventhandler.h>
#include <inviwo/core/interaction/events/keyboardevent.h>
#include <inviwo/core/interaction/events/mouseevent.h>
#include <inviwo/core/interaction/events/timerevent.h>

namespace inviwo {

class IVW_CORE_API Canvas {
public:
    Canvas(ivec2 dimensions);
    virtual ~Canvas();

    virtual void initialize();
    virtual void deinitialize();
    virtual void switchContext();
    virtual void repaint();
    virtual void resize(ivec2 dimensions);
    virtual ivec2 size() { return dimensions_;}
    virtual void update();

    void setNetworkEvaluator(ProcessorNetworkEvaluator* networkEvaluator) { processorNetworkEvaluator_ = networkEvaluator; };

    static ProcessorNetworkEvaluator* processorNetworkEvaluator_; //TODO: should this rather be private?

protected:
    ivec2 dimensions_;
};

} // namespace

#endif // IVW_CANVAS_H
