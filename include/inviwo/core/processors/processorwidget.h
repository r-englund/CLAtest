#ifndef IVW_PROCESSORWIDGET_H
#define IVW_PROCESSORWIDGET_H

#include "inviwo/core/inviwocoredefine.h"

namespace inviwo {

class Processor;
class IVW_CORE_API ProcessorWidget {

public:
    ProcessorWidget(Processor* processor);
    virtual ~ProcessorWidget();

    virtual void initialize();

    virtual void show();
    virtual void hide();
    virtual void resize();

protected:
    Processor* processor_;
    bool initialized_;
};

} // namespace

#endif // IVW_PROCESSORWIDGET_H
