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

void IntMinMaxPropertyWidgetQt::updateFromProperty() {
    updatingFromProperty_ = true;

    slider_->setRange(property_->getRangeMin(), property_->getRangeMax());
    spinBoxMin_->setRange(property_->getRangeMin(), property_->getRangeMax());
    spinBoxMax_->setRange(property_->getRangeMin(), property_->getRangeMax());

    spinBoxMin_->setSingleStep(property_->getIncrement());
    spinBoxMax_->setSingleStep(property_->getIncrement());

    glm::ivec2 value = property_->get();
    slider_->setValue(value.x, value.y);
    spinBoxMin_->setValue(value.x);
    spinBoxMax_->setValue(value.y);

    updatingFromProperty_ = false;
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