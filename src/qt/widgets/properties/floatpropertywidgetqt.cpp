#include <inviwo/qt/widgets/properties/floatpropertywidgetqt.h>

namespace inviwo {

FloatPropertyWidgetQt::FloatPropertyWidgetQt(FloatProperty* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void FloatPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    slider_ = new QSlider();
    slider_->setOrientation(Qt::Horizontal);
    connect(slider_, SIGNAL(valueChanged(int)), this, SLOT(setPropertyValue()));
    hLayout->addWidget(slider_);
    setLayout(hLayout);
    
    slider_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(slider_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint&)));

}

void FloatPropertyWidgetQt::setPropertyValue() {
    float valuef = (static_cast<float>(slider_->value())-slider_->minimum()) / (slider_->maximum()-slider_->minimum());
    property_->set(valuef);
}

void FloatPropertyWidgetQt::updateFromProperty() {
    float valuef = property_->get();
    int value = slider_->minimum() + static_cast<int>(ceilf(valuef * (slider_->maximum()-slider_->minimum())));
    slider_->setValue(value);
}

void FloatPropertyWidgetQt::showContextMenu(const QPoint& pos) {

    QPoint globalPos = slider_->mapToGlobal(pos);

    QMenu settingsMenu;

    settingsMenu.addAction("SetMax");
    settingsMenu.addAction("SetMin");

    QAction* selecteditem = settingsMenu.exec(globalPos);
    if (selecteditem) {
        std::cout<<"Inne i menu" << std::endl;
    }


}

} // namespace
