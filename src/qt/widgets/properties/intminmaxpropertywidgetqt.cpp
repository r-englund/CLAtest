#include <inviwo/qt/widgets/properties/intminmaxpropertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

IntMinMaxPropertyWidgetQt::IntMinMaxPropertyWidgetQt(IntMinMaxProperty *property) : property_(property) {
	generateWidget();
	updateFromProperty();
}

void IntMinMaxPropertyWidgetQt::generateWidget() {
	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));

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

    connect(slider_, SIGNAL(valuesChanged(int,int)), this, SLOT(updateFromSlider(int,int)));
    connect(spinBoxMin_, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBoxMin(int)));
    connect(spinBoxMax_, SIGNAL(valueChanged(int)), this, SLOT(updateFromSpinBoxMax(int)));
}

void IntMinMaxPropertyWidgetQt::updateFromProperty() {
    slider_->setRange(property_->getRangeMin(), property_->getRangeMax());
    spinBoxMin_->setRange(property_->getRangeMin(), property_->getRangeMax());
    spinBoxMax_->setRange(property_->getRangeMin(), property_->getRangeMax());

    spinBoxMin_->setSingleStep(property_->getIncrement());
    spinBoxMax_->setSingleStep(property_->getIncrement());

    glm::ivec2 value = property_->get();
    slider_->setValue(value.x, value.y);
    spinBoxMin_->setValue(value.x);
    spinBoxMax_->setValue(value.y);
}


void IntMinMaxPropertyWidgetQt::updateFromSlider(int valMin, int valMax){
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

void IntMinMaxPropertyWidgetQt::updateFromSpinBoxMin(int val){
    slider_->setMinValue(val);
    setPropertyValue();
}

void IntMinMaxPropertyWidgetQt::updateFromSpinBoxMax(int val){
    slider_->setMaxValue(val);
    setPropertyValue();
}

void IntMinMaxPropertyWidgetQt::setPropertyValue() {
    property_->set(ivec2(spinBoxMin_->value(), spinBoxMax_->value()));
    emit modified();
}

} //namespace