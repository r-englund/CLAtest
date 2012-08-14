#include "inviwo/qt/editor/propertylistwidget.h"

namespace inviwo {

PropertyListWidget::PropertyListWidget(QWidget* parent) : InviwoDockWidget(tr("Properties"), parent) {
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setMinimumWidth(200);
}

PropertyListWidget::~PropertyListWidget() {}

} // namespace