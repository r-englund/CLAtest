#ifndef IVW_PYTHONINFOWIDGET_H
#define IVW_PYTHONINFOWIDGET_H

#include <modules/pythonqt/pythonqtmoduledefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/qt/widgets/inviwodockwidget.h>
#include "../python/pyinviwoobserver.h"

class QTabWidget;

namespace inviwo {

class IVW_MODULE_PYTHONQT_API PythonInfoWidget : public InviwoDockWidget
                                               , public PyInviwoObserver{ 
//    Q_OBJECT
public:
    PythonInfoWidget(QWidget *parent);
    virtual ~PythonInfoWidget();


    virtual void onModuleRegistered( PyModule* module );

private:
    void buildWidget();
    QTabWidget* tabWidget_;
};

} // namespace

#endif // IVW_PYTHONINFOWIDGET_H

