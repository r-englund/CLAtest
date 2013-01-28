#include <inviwo/qt/editor/inviwodockwidget.h>

namespace inviwo {

InviwoDockWidget::InviwoDockWidget(QString title, QWidget* parent) : QDockWidget(title, parent) {
}

InviwoDockWidget::~InviwoDockWidget() {}

} // namespace