#ifndef IVW_CANVASPROCESSOR_H
#define IVW_CANVASPROCESSOR_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/ports/imageport.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/util/canvas.h>

namespace inviwo {

class IVW_CORE_API CanvasProcessor : public Processor {
public:
    CanvasProcessor();
    ~CanvasProcessor();

    virtual void initialize();
    virtual void deinitialize();

    virtual void process();

    void setCanvas(Canvas* canvas) { canvas_ = canvas; }
    Canvas* getCanvas() const { return canvas_; }
    virtual void invalidate(PropertyOwner::InvalidationLevel invalidationLevel);

    bool isEndProcessor() { return true; }
    
    void createSnapshot();
    void createSnapshot(const char* filePath);

protected:
    ImageInport inport_;
    TemplateOptionProperty<ImageLayerType> visibleLayer_;
    ButtonProperty snapshotButton_;

private:
    Canvas* canvas_;
};

} // namespace

#endif // IVW_CANVASPROCESSOR_H
