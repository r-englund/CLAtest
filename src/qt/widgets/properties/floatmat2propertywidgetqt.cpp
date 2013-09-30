#include <inviwo/qt/widgets/properties/floatmat2propertywidgetqt.h>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>

namespace inviwo {

FloatMat2PropertyWidgetQt::FloatMat2PropertyWidgetQt(FloatMat2Property *property) : property_(property) {
    PropertyWidgetQt::setProperty(property_);
    generateWidget();
    updateFromProperty();
    PropertyWidgetQt::generateContextMenu();
}

void FloatMat2PropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    QGroupBox* matrixgroup = new QGroupBox();
    mat0x0_ = new QLineEdit;
    mat0x1_ = new QLineEdit;
    mat1x0_ = new QLineEdit;
    mat1x1_ = new QLineEdit;
    if (property_->getReadOnly()) {
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        mat0x0_->setDisabled(true);
        mat0x1_->setDisabled(true);
        mat1x0_->setDisabled(true);
        mat1x1_->setDisabled(true);
    }
    else
    {
        hLayout->addWidget(label_);
        connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
        connect(mat0x0_, SIGNAL(editingFinished ()),
            this, SLOT(set0x0Value()));
        connect(mat0x1_, SIGNAL(editingFinished ()),
            this, SLOT(set0x1Value()));
        connect(mat1x0_, SIGNAL(editingFinished ()),
            this, SLOT(set1x0Value()));
        connect(mat1x1_, SIGNAL(editingFinished ()),
            this, SLOT(set1x1Value()));
    }
    QGridLayout *gridLayout = new QGridLayout;

    gridLayout->addWidget(mat0x0_,0,0);
    gridLayout->addWidget(mat0x1_,0,1);
    gridLayout->addWidget(mat1x0_,1,0);
    gridLayout->addWidget(mat1x1_,1,1);

    matrixgroup->setLayout(gridLayout);
    label_ = new EditableLabelQt(property_->getDisplayName());

     hLayout->addWidget(matrixgroup);
    setLayout(hLayout);
}

void FloatMat2PropertyWidgetQt::set0x0Value() {
   float valuef =(mat0x0_->text()).toFloat();
   mat2 valueMat2 =property_->get();
   valueMat2[0][0] = valuef;
   property_->set(valueMat2);
   emit modified();
}

void FloatMat2PropertyWidgetQt::set0x1Value() {
    float valuef =(mat0x1_->text()).toFloat();
    mat2 valueMat2 =property_->get();
    valueMat2[0][1] = valuef;
    property_->set(valueMat2);
    emit modified();
}

void FloatMat2PropertyWidgetQt::set1x0Value() {
    float valuef =(mat1x0_->text()).toFloat();
    mat2 valueMat2 =property_->get();
    valueMat2[1][0] = valuef;
    property_->set(valueMat2);
    emit modified();
}

void FloatMat2PropertyWidgetQt::set1x1Value() {
    float valuef =(mat1x1_->text()).toFloat();
    mat2 valueMat2 =property_->get();
    valueMat2[1][1] = valuef;
    property_->set(valueMat2);
    emit modified();
}

void FloatMat2PropertyWidgetQt::setPropertyValue() {
    set0x0Value();
    set0x1Value();
    set1x0Value();
    set1x1Value();
}

void FloatMat2PropertyWidgetQt::updateFromProperty() {
    mat2 value =property_->get();
    mat0x0_->setText(QString::number(value[0][0]));
    mat0x1_->setText(QString::number(value[0][1]));
    mat1x0_->setText(QString::number(value[1][0]));
    mat1x1_->setText(QString::number(value[1][1]));
};

void FloatMat2PropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}


} //namespace