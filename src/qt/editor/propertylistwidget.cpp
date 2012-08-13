#include "inviwo/qt/editor/propertylistwidget.h"

namespace inviwo {

PropertyListWidget::PropertyListWidget(QWidget* parent) : QDockWidget(tr("Properties"), parent) {
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
}

PropertyListWidget::~PropertyListWidget() {}

} // namespace