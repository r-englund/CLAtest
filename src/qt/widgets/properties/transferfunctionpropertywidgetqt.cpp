#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>
#include <QWheelEvent>


namespace inviwo {

    TransferFunctionPropertyWidgetQt::TransferFunctionPropertyWidgetQt(TransferFunctionProperty* property) : property_(property)
    /*intColor_("IntColor","IntColor", ivec4(20),ivec4(0), ivec4(10),ivec4(1),PropertyOwner::INVALID_OUTPUT,PropertySemantics::Color)*/
    {
        generateWidget();
        updateFromProperty();
        
    }

    void TransferFunctionPropertyWidgetQt::generateWidget() {
        zoom_ = 0;
        QVBoxLayout* vLayout = new QVBoxLayout();

        transferFunction_ = &property_->get();
        editor_ = new TransferFunctionEditor(this, transferFunction_, &points_);
        editor_->setSceneRect(0,0,255,100);
        editor_->setBackgroundBrush(Qt::transparent);

        editorview_ = new QGraphicsView(this);
        editorview_->setFixedSize(257, 102);
        editorview_->scale(1, -1);
        editorview_->setScene(editor_);
        editorview_->show();

        paintscene_ = new QGraphicsScene(this);
        paintview_ = new QGraphicsView(this);
        paintview_->setFixedSize(257, 52);
        paintview_->scale(1, -1);
        paintview_->setScene(paintscene_); 
        paintview_->show();

        checkBox_ = new QCheckBox();
        connect(checkBox_, SIGNAL(pressed()), this, SLOT(setPropertyValue()));

        //intColor_ = IntVec4Property("IntColor","IntColor", ivec4(20),ivec4(0), ivec4(10),ivec4(1),PropertyOwner::INVALID_OUTPUT,PropertySemantics::Color);
        //IntVec4PropertyWidgetQt* wigget = new IntVec4PropertyWidgetQt(intColor_);

        setLayout(vLayout);
        
        vLayout->addWidget(checkBox_);
        vLayout->addWidget(editorview_);
        vLayout->addWidget(paintview_);
    }


    void TransferFunctionPropertyWidgetQt::wheelEvent(QWheelEvent* e) {
        double scaleFactor = 1.05; ///< Zoom in/out by 5%
        if(e->delta() > 0) {
            zoom_++;
            editorview_->scale(scaleFactor, scaleFactor);
        } else if(zoom_ > 0){
            zoom_--;
            editorview_->scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        }
        editorview_->update();
        editorview_->setSceneRect(0,0,20,20);
        e->accept();
    }

    void TransferFunctionPropertyWidgetQt::setPropertyValue() {
        LogInfo("setPropertyValue");
    }


    void TransferFunctionPropertyWidgetQt::updateFromProperty() {
        QPen pen;

        Image* img = transferFunction_->getData();
        ImageRAMVec4float32 * imgRam = img->getEditableRepresentation<ImageRAMVec4float32>();
        vec4* data = static_cast<vec4*>(imgRam->getData());

        if (checkBox_->isChecked())
        {
            for (int i = 0; i < 256 ; i++)
            {
                pen.setColor(QColor::fromRgbF(data[i].a, data[i].a, data[i].a, 1.0f));
                paintscene_->addLine(i, 0, i, 50, pen);
            }
        }
        this->update();
        property_->invalidate();
    }
} // namespace