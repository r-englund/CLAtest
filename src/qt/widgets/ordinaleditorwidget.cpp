#include <inviwo/qt/widgets/ordinaleditorwidget.h>

namespace inviwo {

BaseOrdinalEditorWidget::BaseOrdinalEditorWidget() {
    generateWidget();
}

BaseOrdinalEditorWidget::~BaseOrdinalEditorWidget() {
}

void BaseOrdinalEditorWidget::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    editor_ = new QLineEdit(this);
    hLayout->addWidget(editor_);
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(0);
    setLayout(hLayout);
    connect(editor_, SIGNAL(editingFinished()), this, SLOT(updateFromEditor()));
}
    
void BaseOrdinalEditorWidget::updateEditor() {
    editor_->blockSignals(true);
    editor_->setText(transformValueToEditor());
    editor_->blockSignals(false);
}

void BaseOrdinalEditorWidget::updateFromEditor() {
    newEditorValue(editor_->text());
    emit valueChanged();
}

void BaseOrdinalEditorWidget::applyInit() {
    updateEditor();
}

void BaseOrdinalEditorWidget::applyValue() {
    applyInit();
    emit valueChanged();
}

} // namespace

