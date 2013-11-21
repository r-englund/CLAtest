#ifndef IVW_PROCESSORWIDGETQT_H
#define IVW_PROCESSORWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QWidget>
#include <inviwo/core/processors/processorwidget.h>

namespace inviwo {

class IVW_QTWIDGETS_API ProcessorWidgetQt : public QWidget, public ProcessorWidget {
    Q_OBJECT

public:
    ProcessorWidgetQt();
    virtual ~ProcessorWidgetQt();

    virtual ProcessorWidget* create() const = 0;
    virtual void initialize() = 0;    
    virtual void setVisible(bool visible);
    virtual void show();
    virtual void hide();
    virtual void move(ivec2 pos);
	virtual void setDimensions(ivec2 dimensions);

protected:
    virtual void resizeEvent(QResizeEvent*);
    virtual void closeEvent(QCloseEvent*);
    virtual void showEvent(QShowEvent*);
    virtual void moveEvent(QMoveEvent*);
};

} // namespace

#endif // IVW_PROCESSORWIDGETQT_H
