#ifndef IVW_CANVASPROCESSOR_H
#define IVW_CANVASPROCESSOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/util/canvas.h>
#include "inviwo/core/properties/snapshotproperty.h"

namespace inviwo {

class IVW_CORE_API CanvasProcessor : public Processor {
public:
    CanvasProcessor();
    ~CanvasProcessor();
    virtual Processor* create() const;

    virtual void initialize();
    virtual void deinitialize();

    virtual void process();

    void setCanvas(Canvas* canvas) { canvas_ = canvas; }
    Canvas* getCanvas() const { return canvas_; }
    virtual void invalidate();

    bool isEndProcessor() { return true; }
    
    void takeSnapshot(const char* filePath);

protected:
    ImageInport inport_;
    SnapshotProperty snapshot_;

private:
    Canvas* canvas_;
};

} // namespace

#endif // IVW_CANVASPROCESSOR_H
