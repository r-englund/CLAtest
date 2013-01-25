#ifndef IVW_PROCESSORWIDGETQT_H
#define IVW_PROCESSORWIDGETQT_H

#include "inviwo/qt/widgets/inviwoqtwidgetsdefine.h"
#include <QWidget>
#include "inviwo/core/processors/processorwidget.h"

namespace inviwo {

class IVW_QTWIDGETS_API ProcessorWidgetQt : public ProcessorWidget, public QWidget {

public:
    ProcessorWidgetQt(Processor* processor, QWidget* parent);
    virtual ~ProcessorWidgetQt();

    virtual void show();
    virtual void hide();

    protected:
    void resizeEvent(QResizeEvent*);
    void closeEvent(QCloseEvent *);
    void moveEvent(QMoveEvent* event);
};

} // namespace

#endif // IVW_PROCESSORWIDGETQT_H
