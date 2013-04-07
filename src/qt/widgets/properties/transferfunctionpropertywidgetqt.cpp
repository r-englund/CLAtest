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
        paintview->setFixedSize(257, 102);
        paintview->scale(1, -1);
        paintview->setScene(paintscene); 
        paintview->show();

        transferFunc = new TransferFunction();
        //transferImage = new ImageRAMfloat16(uvec2(1,256));
        editor = new TransferEditor(this, transferFunc);
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
        //TransferFunction* out = new TransferFunction();
        //out->setAlpha(transferImage);
        //property_->set(*out);

        property_->set(*transferFunc);
    }

    void TransferFunctionPropertyWidgetQt::updateFromProperty() {
        QPen pen;
        float* data = static_cast<float*>(transferFunc->getAlpha()->getData());
        for (int i = 0; i < 256 ; i++)
        {
            pen.setColor(QColor::fromRgbF(data[i], data[i], data[i], 1.0f));
            paintscene->addLine(i, 0, i, 100, pen);
        }
        this->update();
    }
} // namespace
