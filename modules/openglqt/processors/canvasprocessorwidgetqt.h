#ifndef IVW_CANVASPROCESSORWIDGET_H
#define IVW_CANVASPROCESSORWIDGET_H

#include <modules/openglqt/openglqtmoduledefine.h>
#include <modules/openglqt/canvasqt.h>
#include <inviwo/qt/widgets/processors/processorwidgetqt.h>

namespace inviwo {

class IVW_MODULE_OPENGLQT_API CanvasProcessorWidgetQt : public ProcessorWidgetQt {
    Q_OBJECT
public:
    CanvasProcessorWidgetQt();
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
