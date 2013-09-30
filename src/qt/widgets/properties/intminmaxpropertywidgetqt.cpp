#include <inviwo/qt/widgets/properties/intminmaxpropertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

IntMinMaxPropertyWidgetQt::IntMinMaxPropertyWidgetQt(IntMinMaxProperty *property) : property_(property), updatingFromProperty_(false) {
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
	updateFromProperty();
}

void IntMinMaxPropertyWidgetQt::generateWidget() {
	QHBoxLayout* hLayout = new QHBoxLayout();

    if (property_->getReadOnly()) {
        valueVec_ = property_->get();
        valueVec_ = property_->get();
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        labelMin_ = new QLabel("Min:  "+QString::number(valueVec_.x));
        labelMax_ = new QLabel("Max: " +QString::number(valueVec_.y));
 
        hLayout->addWidget(labelMin_);
        hLayout->addWidget(labelMax_);
        setLayout(hLayout);
    }
    else{
        label_ = new EditableLabelQt(property_->getDisplayName());
        hLayout->addWidget(label_);

        spinBoxMin_ = new QSpinBox(this);
        spinBoxMin_->setFixedWidth(50);
        hLayout->addWidget(spinBoxMin_);

        slider_ = new RangeSliderQt(Qt::Horizontal, this);
        slider_->setRange(0, 99);
	    hLayout->addWidget(slider_);

        spinBoxMax_ = new QSpinBox(this);
        spinBoxMax_->setFixedWidth(50);
        hLayout->addWidget(spinBoxMax_);
	    setLayout(hLayout);
        
        connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
        connect(slider_, SIGNAL(valuesChanged(int,int)), this, SLOT(updateFromSlider(int,int)));
        connect(spinBoxMin_, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBoxMin(int)));
        connect(spinBoxMax_, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBoxMax(int)));
    }
}

void IntMinMaxPropertyWidgetQt::updateFromProperty() {
    updatingFromProperty_ = true;
    valueVec_ = property_->get();
    if (property_->getReadOnly()) {
        labelMin_->setText("Min: " +QString::number(valueVec_.x));
        labelMax_->setText("Max: " +QString::number(valueVec_.y));

        updatingFromProperty_ = false;
    }
    else{

    slider_->setRange(property_->getRangeMin(), property_->getRangeMax());
    spinBoxMin_->setRange(property_->getRangeMin(), property_->getRangeMax());
    spinBoxMax_->setRange(property_->getRangeMin(), property_->getRangeMax());

    spinBoxMin_->setSingleStep(property_->getIncrement());
    spinBoxMax_->setSingleStep(property_->getIncrement());


    slider_->setValue(valueVec_.x, valueVec_.y);
    spinBoxMin_->setValue(valueVec_.x);
    spinBoxMax_->setValue(valueVec_.y);

    updatingFromProperty_ = false;
    }
}


void IntMinMaxPropertyWidgetQt::updateFromSlider(int valMin, int valMax){
    if(!updatingFromProperty_){
        bool changed = false;
        if(valMin != spinBoxMin_->value()){
            spinBoxMin_->setValue(valMin);
            changed = true;
        }
        if(valMax != spinBoxMax_->value()){
            spinBoxMax_->setValue(valMax);
            changed = true;
        }
        if(changed)
            setPropertyValue();
    }
}

void IntMinMaxPropertyWidgetQt::updateFromSpinBoxMin(int val){
    if(!updatingFromProperty_){
        slider_->setMinValue(val);
        setPropertyValue();
    }
}

void IntMinMaxPropertyWidgetQt::updateFromSpinBoxMax(int val){
    if(!updatingFromProperty_){
        slider_->setMaxValue(val);
        setPropertyValue();
    }
}

void IntMinMaxPropertyWidgetQt::setPropertyValue() {
    property_->set(ivec2(spinBoxMin_->value(), spinBoxMax_->value()));
    emit modified();
}

void IntMinMaxPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}

} //namespace