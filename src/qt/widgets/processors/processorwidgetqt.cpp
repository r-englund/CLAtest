#include <inviwo/qt/widgets/processors/processorwidgetqt.h>
#include <QResizeEvent>
#include <QMoveEvent>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

ProcessorWidgetQt::ProcessorWidgetQt(Processor* processor, QWidget* parent)
    : ProcessorWidget(processor), QWidget(parent)
{
    ivec2 pos = ProcessorWidget::getPositionMetaData();
    QWidget::move(pos.x, pos.y);

    ivec2 dim = ProcessorWidget::getDimensionMetaData();
    QWidget::resize(dim.x, dim.y);

    if (ProcessorWidget::getVisibilityMetaData())
        setVisible(true);
    else
        setVisible(false);
}

ProcessorWidgetQt::~ProcessorWidgetQt() {}

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
    ProcessorWidget::resize(ivec2(event->size().width(), event->size().height()) );
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
