#include <QApplication>
#include <QLayout>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>
#include <QMimeData>

#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/qt/widgets/properties/htmllistwidgetqt.h>

namespace inviwo {

void HtmlTree::mousePressEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton)
        dragStartPosition_ = e->pos();
    QTreeWidget::mousePressEvent(e);
}

void HtmlTree::mouseMoveEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton) {
        if ((e->pos() - dragStartPosition_).manhattanLength() < QApplication::startDragDistance())
            return;

        QTreeWidgetItem* selectedProcessor = itemAt(dragStartPosition_);
        if (selectedProcessor)
            new HtmlDragObject(this, selectedProcessor->text(0));
    }
}

HtmlTreeWidget::HtmlTreeWidget(QWidget* parent): QWidget(parent) {
    setObjectName("HtmlTreeWidget");    
    //QFrame* frame = new QFrame();
    //frame->setObjectName("HtmlTreeWidgetFrame");
    QVBoxLayout* vLayout = new QVBoxLayout();

    QLineEdit* lineEdit = new QLineEdit();
    lineEdit->setPlaceholderText("Filter Html object list...");
    connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(addTagsToTree(const QString&)));
    vLayout->addWidget(lineEdit);

    processorTree_ = new HtmlTree(this);
    processorTree_->setHeaderHidden(true);
    addTagsToTree();
    processorTree_->expandAll();
    vLayout->addWidget(processorTree_);

    //frame->setLayout(vLayout);
    setLayout(vLayout);
}

HtmlTreeWidget::~HtmlTreeWidget() {}

void HtmlTreeWidget::addTagsToTree(const QString& text) {
    processorTree_->clear();
    //Add tag items
    processorTree_->sortItems(0, Qt::AscendingOrder);
}

static QString mimeType = "inviwo/HtmlDragObject";

HtmlDragObject::HtmlDragObject(QWidget* source, const QString className) : QDrag(source) {
    QByteArray byteData;
    {
        QDataStream ds(&byteData, QIODevice::WriteOnly);
        ds << className;
    }

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(mimeType, byteData);
    mimeData->setData("text/plain", className.toLatin1().data());
    setMimeData(mimeData);

    start(Qt::MoveAction);
}

bool HtmlDragObject::canDecode(const QMimeData* mimeData) {
    if (mimeData->hasFormat(mimeType)) return true;
    else return false;
}

bool HtmlDragObject::decode(const QMimeData* mimeData, QString& className) {
    QByteArray byteData = mimeData->data(mimeType);
    if (byteData.isEmpty())
        return false;

    QDataStream ds(&byteData, QIODevice::ReadOnly);
    ds >> className;
    return true;
}

} // namespace