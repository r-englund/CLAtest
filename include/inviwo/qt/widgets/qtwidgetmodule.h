#ifndef IVW_QTWIDGETMODULE_H
#define IVW_QTWIDGETMODULE_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo {

class IVW_QTWIDGETS_API QtWidgetModule : public InviwoModule {

public:
    QtWidgetModule();
    virtual ~QtWidgetModule() {}

};

} // namespace

#endif // IVW_QTWIDGETMODULE_H

