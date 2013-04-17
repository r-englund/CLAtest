#ifndef IVW_MAPPINGWIDGET_H
#define IVW_MAPPINGWIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/qt/editor/inviwodockwidget.h>
#include <inviwo/core/util/observer.h>
#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

class IVW_QTEDITOR_API MappingWidget : public InviwoDockWidget, public VoidObserver {
    Q_OBJECT
public:
    MappingWidget(QWidget* parent);
    ~MappingWidget();
    void notify();
private:
    QString intToQString(int num);
    InviwoApplication* inviwoApp_;
    QLabel* label;
};

} // namespace

#endif // IVW_MAPPINGWIDGET_H