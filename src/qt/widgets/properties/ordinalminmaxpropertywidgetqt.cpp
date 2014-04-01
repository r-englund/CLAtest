#include <inviwo/qt/widgets/properties/ordinalminmaxpropertywidgetqt.h>

namespace inviwo {

BaseOrdinalMinMaxPropertyWidgetQt::BaseOrdinalMinMaxPropertyWidgetQt(Property* property)
    : PropertyWidgetQt(property) {
}
    
BaseOrdinalMinMaxPropertyWidgetQt::~BaseOrdinalMinMaxPropertyWidgetQt(){
}

void BaseOrdinalMinMaxPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(7);

    label_ = new EditableLabelQt(this, property_->getDisplayName());
    hLayout->addWidget(label_);
    
    QHBoxLayout* hSliderLayout = new QHBoxLayout();
    QWidget* sliderWidget = new QWidget();
    sliderWidget->setLayout(hSliderLayout);
    hSliderLayout->setContentsMargins(0,0,0,0);
    
    spinBoxMin_ = new CustomDoubleSpinBoxQt(this);
    spinBoxMin_->setKeyboardTracking(false); // don't emit the valueChanged() signal while typing
    spinBoxMin_->setFixedWidth(50);
    hSliderLayout->addWidget(spinBoxMin_);
    
    slider_ = new RangeSliderQt(Qt::Horizontal, this);
    hSliderLayout->addWidget(slider_);
    
    spinBoxMax_ = new CustomDoubleSpinBoxQt(this);
    spinBoxMax_->setKeyboardTracking(false); // don't emit the valueChanged() signal while typing
    spinBoxMax_->setFixedWidth(50);
    hSliderLayout->addWidget(spinBoxMax_);
    
    hLayout->addWidget(sliderWidget);
    setLayout(hLayout);
    
    QSizePolicy slidersPol = sliderWidget->sizePolicy();
    slidersPol.setHorizontalStretch(3);
    sliderWidget->setSizePolicy(slidersPol);
    
    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    
    connect(slider_, SIGNAL(valuesChanged(int,int)), this, SLOT(updateFromSlider(int,int)));
    connect(spinBoxMin_, SIGNAL(valueChanged(double)), this, SLOT(updateFromSpinBoxMin(double)));
    connect(spinBoxMax_, SIGNAL(valueChanged(double)), this, SLOT(updateFromSpinBoxMax(double)));
}

void BaseOrdinalMinMaxPropertyWidgetQt::setPropertyDisplayName() {
    this->property_->setDisplayName(label_->getText());
}


} // namespace

