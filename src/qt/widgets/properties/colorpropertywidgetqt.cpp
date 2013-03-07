#include <inviwo/qt/widgets/properties/colorpropertywidgetqt.h>
#include <QHBoxLayout>

#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {


ColorPropertyWidgetQt::ColorPropertyWidgetQt(Property* property) : property_(property), btnProperty_("ColorBtn","Open color dialog") {
    btnWidget_ = new ButtonPropertyWidgetQt(&btnProperty_);
    generateWidget();
    updateFromProperty();
}

void ColorPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    colorLabel_ = new QLabel(" ");
    colorLabel_->setFixedWidth(50);
    hLayout->addWidget(colorLabel_);
    
    colorDialog_ = new QColorDialog();
    btnProperty_.registerClassMemberFunction(this, &ColorPropertyWidgetQt::openColorDialog);
    hLayout->addWidget(btnWidget_);
    connect(colorDialog_,SIGNAL(currentColorChanged(QColor)),this, SLOT(setPropertyValue()));
    currentColor_ = new QColor();
    setLayout(hLayout);
}
void ColorPropertyWidgetQt::updateFromProperty(){
    if (dynamic_cast<IntVec4Property*>(property_)) {
        ivec4 colorVector = dynamic_cast<IntVec4Property*>(property_)->get();
        currentColor_->setRgb(colorVector.x, colorVector.y, colorVector.z, colorVector.w);
    }
    if (dynamic_cast<FloatVec4Property*>(property_)) {
        vec4 colorVector = dynamic_cast<FloatVec4Property*>(property_)->get();
        int xVal = static_cast<int>(colorVector.x*255);
        int yVal = static_cast<int>(colorVector.y*255);
        int zVal = static_cast<int>(colorVector.z*255);
        int wVal = static_cast<int>(colorVector.w*255);
        currentColor_->setRgb(xVal, yVal, zVal, wVal);
    }

    colorLabel_->setStyleSheet("QLabel { background-color : "+currentColor_->name() +"; }");
    colorDialog_->setCurrentColor(*currentColor_);

}


QColor ColorPropertyWidgetQt::getCurrentColor(){
    return *currentColor_;
}

void ColorPropertyWidgetQt::setPropertyValue() {

    currentColor_ = &colorDialog_->currentColor();
    if (dynamic_cast<IntVec4Property*>(property_)) {
        dynamic_cast<IntVec4Property*>(property_)->set(ivec4(currentColor_->red(),
                                                            currentColor_->green(),
                                                            currentColor_->blue(),
                                                            currentColor_->alpha()));

    }
    if (dynamic_cast<FloatVec4Property*>(property_)) {
        dynamic_cast<FloatVec4Property*>(property_)->set(vec4(static_cast<float>(currentColor_->red())/255,
                                                            static_cast<float>(currentColor_->green())/255,
                                                            static_cast<float>(currentColor_->blue())/255,
                                                            static_cast<float>(currentColor_->alpha())/255));

    }
        
        colorLabel_->setStyleSheet("QLabel { background-color : "+currentColor_->name() +"; }");
}

void ColorPropertyWidgetQt::openColorDialog() {
    colorDialog_->open();
}



}//namespace