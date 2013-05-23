#ifndef IVW_PROCESSORWIDGET_H
#define IVW_PROCESSORWIDGET_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/metadata/processorwidgetmetadata.h>

namespace inviwo {

class Processor;
class IVW_CORE_API ProcessorWidget {

public:
    ProcessorWidget();
    virtual ~ProcessorWidget();

    virtual ProcessorWidget* create() const = 0;
    virtual void initialize() = 0;    
    virtual void setVisible(bool visible);
    virtual void show();
    virtual void hide();    
    virtual void setDimensions(ivec2);
    virtual void move(ivec2);
    virtual void setProcessor(Processor* processor) {processor_ = processor;}
    virtual Processor* getProcessor() {return processor_;}

    bool getVisibilityMetaData();

private:
    ProcessorWidgetMetaData* metaData_;

protected:
    Processor* processor_;
    bool initialized_; 

    ivec2 getPositionMetaData();
    ivec2 getDimensionMetaData();
};

} // namespace

#endif // IVW_PROCESSORWIDGET_H
