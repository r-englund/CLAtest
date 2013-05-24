#ifndef IVW_CANVASPROCESSORWIDGET_H
#define IVW_CANVASPROCESSORWIDGET_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <modules/openglqt/canvasqt.h>
#include <inviwo/qt/widgets/processors/processorwidgetqt.h>

namespace inviwo {

class IVW_QTWIDGETS_API CanvasProcessorWidgetQt : public ProcessorWidgetQt {

public:
    CanvasProcessorWidgetQt();
    CanvasProcessorWidgetQt(QWidget* parent);
    virtual ~CanvasProcessorWidgetQt();

    virtual void initialize();
    virtual ProcessorWidget* create() const;
    virtual void show();
    virtual void hide();

protected:
    void resizeEvent(QResizeEvent*);
    void showEvent(QShowEvent*);
    void closeEvent(QCloseEvent*);

private:
    CanvasQt* canvas_;
};

} // namespace

#endif // IVW_CANVASPROCESSORWIDGET_H
