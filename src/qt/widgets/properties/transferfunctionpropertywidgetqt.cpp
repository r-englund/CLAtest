#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>

namespace inviwo {
    const std::string TransferFunctionPropertyWidgetQt::logSource_ = "TransferFunctionPropertyWidget";
    TransferFunctionPropertyWidgetQt::TransferFunctionPropertyWidgetQt(TransferFunctionProperty* property) : property_(property) {
        generateWidget();
        updateFromProperty();
    }

    void TransferFunctionPropertyWidgetQt::generateWidget() {
        QVBoxLayout* vLayout = new QVBoxLayout();

        paintscene = new QGraphicsScene(this);
        paintview = new QGraphicsView(this);
        paintview->setFixedSize(257, 52);
        paintview->scale(1, -1);
        paintview->setScene(paintscene); 
        paintview->show();

        transferFunc = new TransferFunction();
        editor = new TransferEditor(this, transferFunc, &points);
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


    void TransferFunctionPropertyWidgetQt::setPropertyValue() {
        property_->set(*transferFunc);
    }

    void TransferFunctionPropertyWidgetQt::updateFromProperty() {
        QPen pen;
        //LogInfo(points.size());
        float* data = static_cast<float*>(transferFunc->getAlpha()->getData());
        for (int i = 0; i < 256 ; i++)
        {
            pen.setColor(QColor::fromRgbF(data[i], data[i], data[i], 1.0f));
            paintscene->addLine(i, 0, i, 50, pen);
        }
        this->update();
    }
} // namespace
