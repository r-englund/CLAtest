#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>
#include <QWheelEvent>



namespace inviwo {

    TransferFunctionPropertyWidgetQt::TransferFunctionPropertyWidgetQt(TransferFunctionProperty* property) : property_(property){
        generateWidget();
        updateFromProperty();
    }

    void TransferFunctionPropertyWidgetQt::generateWidget() {

        colorDialog_ = new QColorDialog();
        //btnColor_ = new QPushButton();
        //btnColor_->setFixedWidth(100);
        //btnColor_->setFixedHeight(30);
        //connect(btnColor_,SIGNAL(clicked()),this,SLOT(setPropertyValue()));
        connect(colorDialog_,SIGNAL(currentColorChanged(QColor)),this, SLOT(setPropertyValue()));

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

        setLayout(vLayout);

        vLayout->addWidget(checkBox_);
        vLayout->addWidget(editorview_);
        vLayout->addWidget(paintview_);
        vLayout->addWidget(colorDialog_);

        data = static_cast<vec4*>(transferFunction_->getData()->getEditableRepresentation<ImageRAMVec4float32>()->getData());
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
        QColor color = colorDialog_->currentColor();
        for (int i = 0; i < (int)points_.size() ; i++){
            if (points_[i]->isSelected()){
                points_[i]->getPoint()->setRgb(new vec3(color.redF(),color.greenF(),color.blueF()));
            }
        }
        editor_->update();
        updateFromProperty();
        transferFunction_->calcTransferValues();
        property_->invalidate();
    }


    void TransferFunctionPropertyWidgetQt::updateFromProperty(){
        QPen pen;
        if (checkBox_->isChecked()){
            for (int i = 0; i < 256 ; i++){
                pen.setColor(QColor::fromRgbF(data[i].r, data[i].g, data[i].b, 1.0f));
                paintscene_->addLine(i, 0, i, 50, pen);
            }
        }
        this->update();
        property_->invalidate();
    }
} // namespace