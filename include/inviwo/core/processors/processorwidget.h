#ifndef IVW_PROCESSORWIDGET_H
#define IVW_PROCESSORWIDGET_H

namespace inviwo {

class Processor;
class ProcessorWidget {

public:
    ProcessorWidget(Processor* processor);
    virtual ~ProcessorWidget();

    virtual void initialize();

    virtual void show();
    virtual void hide();

protected:
    Processor* processor_;
    bool initialized_;
};

} // namespace

#endif // IVW_PROCESSORWIDGET_H
