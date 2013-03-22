#include <inviwo/qt/widgets/properties/transferpropertywidgetqt.h>

namespace inviwo {
    const std::string TransferPropertyWidgetQt::logSource_ = "TransferPropertyWidget";
    TransferPropertyWidgetQt::TransferPropertyWidgetQt(TransferProperty* property) : property_(property) {
        generateWidget();
        updateFromProperty();
    }

    void TransferPropertyWidgetQt::generateWidget() {
        QVBoxLayout* vLayout = new QVBoxLayout();

        paintscene = new QGraphicsScene(this);
        paintview = new QGraphicsView(this);
        paintview->setFixedSize(257, 102);
        paintview->scale(1, -1);
        paintview->setScene(paintscene); 
        paintview->show();

        editor = new TransferEditor(this);
        editor->setSceneRect(0,0,255,100);
        editor->setBackgroundBrush(Qt::transparent);

        view = new QGraphicsView(this);
        view->setFixedSize(257, 102);
        view->scale(1, -1);
        view->setInteractive(true);
        view->setScene(editor); 
        view->show();

        vLayout->addWidget(view);
        vLayout->addWidget(paintview);
        setLayout(vLayout);

        checkBox_ = new QCheckBox();
        connect(checkBox_, SIGNAL(pressed()), this, SLOT(setPropertyValue()));
        vLayout->addWidget(checkBox_);
    }


    void TransferPropertyWidgetQt::setPropertyValue() {
        TransferFunc* out = new TransferFunc();
        out->setAlpha(editor->getTransferValues());
        property_->set(*out);
    }

    void TransferPropertyWidgetQt::updateFromProperty() {
        QPen pen;
        float* val = editor->getTransferValues();
        for (int i = 0; i < 256 ; i++)
        {
            pen.setColor(QColor::fromRgbF(val[i], val[i], val[i], 1.0f));
            paintscene->addLine(i, 0, i, 100, pen);
        }
        this->update();
    }
} // namespace
