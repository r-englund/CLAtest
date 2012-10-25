#ifndef IVW_PROCESSORWIDGETQT_H
#define IVW_PROCESSORWIDGETQT_H

#include <QWidget>
#include "inviwo/core/processors/processorwidget.h"

namespace inviwo {

class ProcessorWidgetQt : public ProcessorWidget, public QWidget {

public:
    ProcessorWidgetQt(Processor* processor, QWidget* parent);
    virtual ~ProcessorWidgetQt();

    virtual void show();
    virtual void hide();

    protected:
    void resizeEvent(QResizeEvent*);
};

} // namespace

#endif // IVW_PROCESSORWIDGETQT_H
