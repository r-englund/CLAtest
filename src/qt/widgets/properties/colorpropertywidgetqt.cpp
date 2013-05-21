#include <inviwo/qt/widgets/properties/colorpropertywidgetqt.h>
#include <QHBoxLayout>

#include <inviwo/core/properties/vectorproperties.h>

namespace inviwo {


ColorPropertyWidgetQt::ColorPropertyWidgetQt(Property* property) : property_(property) {
    generateWidget();
    updateFromProperty();
}

void ColorPropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    currentColor_ = new QColor();
    colorDialog_ = new QColorDialog();

    btnColor_ = new QPushButton();
    btnColor_->setFixedWidth(100);
    btnColor_->setFixedHeight(30);
    connect(btnColor_,SIGNAL(clicked()),this,SLOT(openColorDialog()));
    
    connect(colorDialog_,SIGNAL(currentColorChanged(QColor)),this, SLOT(setPropertyValue()));

    hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
    hLayout->addWidget(btnColor_);
    setLayout(hLayout);
}

void ColorPropertyWidgetQt::updateFromProperty(){
    if (dynamic_cast<IntVec4Property*>(property_)) {
        ivec4 colorVector = dynamic_cast<IntVec4Property*>(property_)->get();
        currentColor_->setRgb(colorVector.x, colorVector.y, colorVector.z, colorVector.w);
    } else if (dynamic_cast<FloatVec4Property*>(property_)) {
        vec4 colorVector = dynamic_cast<FloatVec4Property*>(property_)->get();
        int xVal = static_cast<int>(colorVector.x*255);
        int yVal = static_cast<int>(colorVector.y*255);
        int zVal = static_cast<int>(colorVector.z*255);
        int wVal = static_cast<int>(colorVector.w*255);
        currentColor_->setRgb(xVal, yVal, zVal, wVal);
    }
    QColor topColor = currentColor_->lighter();
    QColor bottomColor = currentColor_->darker();
    btnColor_->setStyleSheet("QPushButton { background: qlineargradient( \
                                                          x1:0, y1:0, x2:0, y2:1, \
                                                          stop:0 "+topColor.name()+", \
                                                          stop: 0.1 "+currentColor_->name()+", \
                                                          stop:0.9 "+currentColor_->name()+", \
                                                          stop:1 "+bottomColor.name()+"); }");
    colorDialog_->setCurrentColor(*currentColor_);
}

QColor ColorPropertyWidgetQt::getCurrentColor(){
    return *currentColor_;
}

void ColorPropertyWidgetQt::setPropertyValue() {
    if (dynamic_cast<IntVec4Property*>(property_)) {
        dynamic_cast<IntVec4Property*>(property_)->set(ivec4(colorDialog_->currentColor().red(),
                                                            colorDialog_->currentColor().green(),
                                                            colorDialog_->currentColor().blue(),
                                                            colorDialog_->currentColor().alpha()));
    }
    if (dynamic_cast<FloatVec4Property*>(property_)) {
        dynamic_cast<FloatVec4Property*>(property_)->set(vec4(static_cast<float>(colorDialog_->currentColor().red())/255,
                                                            static_cast<float>(colorDialog_->currentColor().green())/255,
                                                            static_cast<float>(colorDialog_->currentColor().blue())/255,
                                                            static_cast<float>(colorDialog_->currentColor().alpha())/255));
    }
    QColor topColor = currentColor_->lighter();
    QColor bottomColor = currentColor_->darker();
    btnColor_->setStyleSheet("QPushButton { background: qlineargradient( \
                                           x1:0, y1:0, x2:0, y2:1, \
                                           stop:0 "+topColor.name()+", \
                                           stop: 0.1 "+currentColor_->name()+", \
                                           stop:0.9 "+currentColor_->name()+", \
                                           stop:1 "+bottomColor.name()+"); }");
    emit modified();
}

void ColorPropertyWidgetQt::openColorDialog() {
    colorDialog_->open();
}

}//namespace