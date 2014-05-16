#include <inviwo/qt/widgets/properties/ordinalminmaxtextpropertywidgetqt.h>

namespace inviwo {

BaseOrdinalMinMaxTextPropertyWidgetQt::BaseOrdinalMinMaxTextPropertyWidgetQt(Property* property)
    : PropertyWidgetQt(property) {
}
    
BaseOrdinalMinMaxTextPropertyWidgetQt::~BaseOrdinalMinMaxTextPropertyWidgetQt(){
}

void BaseOrdinalMinMaxTextPropertyWidgetQt::generateWidget() {
    makeEditorWidgets();
    
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->setContentsMargins(0, 0, 0, 0);
    hLayout->setSpacing(7);

    label_ = new EditableLabelQt(this, property_->getDisplayName());
    hLayout->addWidget(label_);
    
    QHBoxLayout* hSliderLayout = new QHBoxLayout();
    QWidget* sliderWidget = new QWidget();
    sliderWidget->setLayout(hSliderLayout);
    hSliderLayout->setContentsMargins(0,0,0,0);
    
    QLabel* minLabel = new QLabel("Min:");
    hSliderLayout->addWidget(minLabel);
    hSliderLayout->addWidget(min_);

    QLabel* maxLabel = new QLabel("Max:");
    hSliderLayout->addWidget(maxLabel);
    hSliderLayout->addWidget(max_);

    hLayout->addWidget(sliderWidget);
    setLayout(hLayout);
    
    QSizePolicy slidersPol = sliderWidget->sizePolicy();
    slidersPol.setHorizontalStretch(3);
    sliderWidget->setSizePolicy(slidersPol);
    
    connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
    connect(min_, SIGNAL(valueChanged()), this, SLOT(updateFromMin()));
    connect(max_, SIGNAL(valueChanged()), this, SLOT(updateFromMax()));
}

void BaseOrdinalMinMaxTextPropertyWidgetQt::setPropertyDisplayName() {
    this->property_->setDisplayName(label_->getText());
}

} // namespace

