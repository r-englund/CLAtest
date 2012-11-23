#ifndef IVW_CANVASPROCESSOR_H
#define IVW_CANVASPROCESSOR_H

#include "inviwo/core/inviwo.h"
#include "inviwo/core/ports/imageport.h"
#include "inviwo/core/util/canvas.h"
#include "inviwo/qt/widgets/processors/canvasprocessorwidget.h"

namespace inviwo {

    class CanvasProcessor : public Processor {
    public:
        CanvasProcessor();
        ~CanvasProcessor();
        virtual Processor* create() const;

        virtual void initialize() throw (Exception);
        virtual void deinitialize() throw (Exception);

        virtual void process();

        virtual void createProcessorWidget();

        void setCanvas(Canvas* canvas) { canvas_ = canvas; }
        Canvas* getCanvas() const { return canvas_; }

        bool isEndProcessor() { return true; }

    protected:
        ImagePort inport_;

    private:
        Canvas* canvas_;
    };

} // namespace

#endif // IVW_CANVASPROCESSOR_H
