#ifndef IVW_CANVASPROCESSORWIDGET_H
#define IVW_CANVASPROCESSORWIDGET_H

#include "inviwo/qt/widgets/canvasqt.h"
#include "inviwo/qt/widgets/processors/processorwidgetqt.h"

namespace inviwo {

    class CanvasProcessorWidget : public ProcessorWidgetQt {

    public:
        CanvasProcessorWidget(Processor* processor, QWidget* parent);
        virtual ~CanvasProcessorWidget();

        virtual void initialize();

        virtual void show();
        virtual void hide();

    protected:

    void resizeEvent(QResizeEvent*);

    private:
        CanvasQt* canvas_;
    };

} // namespace

#endif // IVW_CANVASPROCESSORWIDGET_H
