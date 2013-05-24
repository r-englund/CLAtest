#include <inviwo/qt/widgets/processors/processorwidgetqt.h>
#include <QResizeEvent>
#include <QMoveEvent>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

ProcessorWidgetQt::ProcessorWidgetQt()
    : ProcessorWidget(), QWidget(0)
{
    QWidget::move(0, 0);
    QWidget::resize(32, 32);
    QWidget::setVisible(false);
}

ProcessorWidgetQt::ProcessorWidgetQt(QWidget* parent)
    : ProcessorWidget(), QWidget(parent)
{    
    QWidget::move(0, 0);
    QWidget::resize(32, 32);
    QWidget::setVisible(false);
}

ProcessorWidgetQt::~ProcessorWidgetQt() {}

void ProcessorWidgetQt::initialize() {
    ProcessorWidget::initialize();
    ivec2 pos = ProcessorWidget::getPositionMetaData();
    QWidget::move(pos.x, pos.y);

    ivec2 dim = ProcessorWidget::getDimensionMetaData();
    QWidget::resize(dim.x, dim.y);

    if (ProcessorWidget::getVisibilityMetaData())
        setVisible(true);
    else
        setVisible(false);
}

void ProcessorWidgetQt::setVisible(bool visible) {
    ProcessorWidget::setVisible(visible);
    QWidget::setVisible(visible);
}

void ProcessorWidgetQt::show() {
    ProcessorWidget::show();
    QWidget::show();
}

void ProcessorWidgetQt::hide() {
    ProcessorWidget::hide();
    QWidget::hide();
}

void ProcessorWidgetQt::move(ivec2 pos) {
    ProcessorWidget::move(pos);
    QWidget::move(pos.x, pos.y);
}

void ProcessorWidgetQt::resizeEvent(QResizeEvent* event) {
    ProcessorWidget::setDimensions(ivec2(event->size().width(), event->size().height()) );
    QWidget::resizeEvent(event);
}

void ProcessorWidgetQt::showEvent(QShowEvent* event) {
    ProcessorWidget::show();
}

void ProcessorWidgetQt::closeEvent(QCloseEvent* event) {
    ProcessorWidget::hide();
}

void ProcessorWidgetQt::moveEvent(QMoveEvent* event) {
    ProcessorWidget::move(ivec2(event->pos().x(), event->pos().y()) );
    QWidget::moveEvent(event);
}

} // namespace
