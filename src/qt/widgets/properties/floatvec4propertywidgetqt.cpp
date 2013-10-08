#include <inviwo/qt/widgets/properties/floatvec4propertywidgetqt.h>

#include <QHBoxLayout>
#include <QLabel>

namespace inviwo {

FloatVec4PropertyWidgetQt::FloatVec4PropertyWidgetQt(FloatVec4Property* property) : property_(property) {
    PropertyWidgetQt::setProperty(property_);
    generateWidget();
    updateFromProperty();
    PropertyWidgetQt::generateContextMenu();
}

void FloatVec4PropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();

    if (property_->getReadOnly()) {
        valueVec_ = property_->get();
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        readOnlyLabel_ = new QLabel();
        hLayout->addWidget(readOnlyLabel_);
        setLayout(hLayout);
    }
    else {
        label_ = new EditableLabelQt(property_->getDisplayName());
        hLayout->addWidget(label_);

        QWidget* sliderWidget = new QWidget();
        QVBoxLayout* vLayout = new QVBoxLayout();
        sliderWidget->setLayout(vLayout);
        vLayout->setContentsMargins(0,0,0,0);
        vec4 valueVec4Max = property_->getMaxValue();
        vec4 valueVec4Min = property_->getMinValue();
        vec4 valueIncrement = property_->getIncrement();

        sliderX_ = new FloatSliderWidgetQt();  
        sliderY_ = new FloatSliderWidgetQt();
        sliderZ_ = new FloatSliderWidgetQt();
        sliderW_ = new FloatSliderWidgetQt();
       
        vLayout->addWidget(sliderX_);
        vLayout->addWidget(sliderY_);
        vLayout->addWidget(sliderZ_);
        vLayout->addWidget(sliderW_);

        hLayout->addWidget(sliderWidget);
        setLayout(hLayout);

        QSizePolicy labelPol = label_->sizePolicy();
        labelPol.setHorizontalStretch(1);
        labelPol.setControlType(QSizePolicy::Label);
        label_->setSizePolicy(labelPol);

        QSizePolicy slidersPol = sliderWidget->sizePolicy();
        slidersPol.setHorizontalStretch(3);
        sliderWidget->setSizePolicy(slidersPol);

        connect(label_, SIGNAL(textChanged()),this, SLOT(setPropertyDisplayName()));
        connect(sliderX_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
        connect(sliderY_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
        connect(sliderZ_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
        connect(sliderW_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
        generatesSettingsWidget();
    }
   
}

void FloatVec4PropertyWidgetQt::setPropertyValue() {
    vec4 valueVec4(sliderX_->getValue(),sliderY_->getValue(),sliderZ_->getValue(),sliderW_->getValue());
    property_->set(valueVec4);
    emit modified();
}


void FloatVec4PropertyWidgetQt::updateFromProperty() {
    valueVec_ = property_->get();
    if (property_->getReadOnly()) {
        readOnlyLabel_->setText(QString::number(valueVec_.x)+","
            +QString::number(valueVec_.y)+","
            +QString::number(valueVec_.z)+","
            +QString::number(valueVec_.w));

        readOnlyLabel_->setToolTip("Min: [" +QString::number((property_->getMinValue()).x)+
            ","+QString::number((property_->getMinValue()).y)+
            ","+QString::number((property_->getMinValue()).z)+
            ","+QString::number((property_->getMinValue()).w)+
            "] Max: [" +QString::number((property_->getMaxValue()).x)+
            ","+QString::number((property_->getMaxValue()).y)+
            ","+QString::number((property_->getMinValue()).z)+
            ","+QString::number((property_->getMinValue()).w)+"]");

    }
    else {
        vec4 valueVec4Max = property_->getMaxValue();
        vec4 valueVec4Min = property_->getMinValue();

        sliderX_->initValue(valueVec_.x);
        sliderY_->initValue(valueVec_.y);
        sliderZ_->initValue(valueVec_.z);
        sliderW_->initValue(valueVec_.w);

        sliderX_->setRange(valueVec4Min.x,valueVec4Max.x);
        sliderY_->setRange(valueVec4Min.y,valueVec4Max.y);
        sliderZ_->setRange(valueVec4Min.z,valueVec4Max.z);
        sliderW_->setRange(valueVec4Min.w,valueVec4Max.w);
        
        sliderX_->setValue(valueVec_.x);
        sliderY_->setValue(valueVec_.y);
        sliderZ_->setValue(valueVec_.z);
        sliderW_->setValue(valueVec_.w);

        vec4 valueIncrement = property_->getIncrement();
        sliderX_->setIncrement(valueIncrement.x);
        sliderY_->setIncrement(valueIncrement.y);
        sliderZ_->setIncrement(valueIncrement.z);
        sliderW_->setIncrement(valueIncrement.w);
    }
}

void FloatVec4PropertyWidgetQt::generatesSettingsWidget() {
    settingsWidget_ = new PropertySettingsWidgetQt(property_);

    settingsMenu_ = PropertyWidgetQt::generatePropertyWidgetMenu();
    settingsMenu_->addAction("Property settings");
    settingsMenu_->addAction("Set as Min");
    settingsMenu_->addAction("Set as Max");

    sliderX_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderY_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderZ_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderW_->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(sliderX_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuX(const QPoint&)));
    connect(sliderY_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuY(const QPoint&)));
    connect(sliderZ_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuZ(const QPoint&)));
    connect(sliderW_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenuW(const QPoint&)));

    connect(developerViewModeAction_,SIGNAL(triggered(bool)),this, SLOT(setDeveloperViewMode(bool)));
    connect(applicationViewModeAction_,SIGNAL(triggered(bool)),this, SLOT(setApplicationViewMode(bool)));

    updateContextMenu();

}

void FloatVec4PropertyWidgetQt::showContextMenuX( const QPoint& pos) {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    PropertyVisibility::VisibilityMode appVisibilityMode  = static_cast<PropertyVisibility::VisibilityMode>(static_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode"))->get());
    if (appVisibilityMode == PropertyVisibility::DEVELOPMENT) {

       QPoint globalPos = sliderX_->mapToGlobal(pos);
        QAction* selecteditem = settingsMenu_->exec(globalPos);
        if (selecteditem == settingsMenu_->actions().at(0)) {
            settingsWidget_->reload();
            settingsWidget_->show();
        }
        else if (selecteditem == settingsMenu_->actions().at(1)) {
            //Set current value of the slider to min value of the property
            vec4 vec4MinValue = property_->getMinValue();
            vec4MinValue.x = sliderX_->getValue();
            property_->setMinValue(vec4MinValue);
            updateFromProperty();
        }
        else if (selecteditem == settingsMenu_->actions().at(2)){
            //Set current value of the slider to max value of the property
            vec4 vec4MaxValue = property_->getMaxValue();
            vec4MaxValue.x = sliderX_->getValue();
            property_->setMaxValue(vec4MaxValue);
            updateFromProperty();
        }
    }
}

void FloatVec4PropertyWidgetQt::showContextMenuY( const QPoint& pos ) {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    PropertyVisibility::VisibilityMode appVisibilityMode  = static_cast<PropertyVisibility::VisibilityMode>(static_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode"))->get());
    if (appVisibilityMode == PropertyVisibility::DEVELOPMENT) {

        QPoint globalPos = sliderY_->mapToGlobal(pos);

        QAction* selecteditem = settingsMenu_->exec(globalPos);
        if (selecteditem == settingsMenu_->actions().at(0)) {
            settingsWidget_->reload();
            settingsWidget_->show();
        }
        else if (selecteditem == settingsMenu_->actions().at(1)) {
            //Set current value of the slider to min value of the property
            vec4 vec4MinValue = property_->getMinValue();
            vec4MinValue.y = sliderY_->getValue();
            property_->setMinValue(vec4MinValue);
            updateFromProperty();
        }
        else if (selecteditem == settingsMenu_->actions().at(2)){
            //Set current value of the slider to max value of the property
            vec4 vec4MaxValue = property_->getMaxValue();
            vec4MaxValue.y = sliderY_->getValue();
            property_->setMaxValue(vec4MaxValue);
            updateFromProperty();
        }

    }
}

void FloatVec4PropertyWidgetQt::showContextMenuZ( const QPoint& pos ) {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    PropertyVisibility::VisibilityMode appVisibilityMode  = static_cast<PropertyVisibility::VisibilityMode>(static_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode"))->get());
    if (appVisibilityMode == PropertyVisibility::DEVELOPMENT) {
        QPoint globalPos = sliderZ_->mapToGlobal(pos);

        QAction* selecteditem = settingsMenu_->exec(globalPos);
        if (selecteditem == settingsMenu_->actions().at(0)) {
            settingsWidget_->reload();
            settingsWidget_->show();
        }
        else if (selecteditem == settingsMenu_->actions().at(1)) {
            //Set current value of the slider to min value of the property
            vec4 vec4MinValue = property_->getMinValue();
            vec4MinValue.z = sliderZ_->getValue();
            property_->setMinValue(vec4MinValue);
            updateFromProperty();
        }
        else if (selecteditem == settingsMenu_->actions().at(2)){
            //Set current value of the slider to max value of the property
            vec4 vec4MaxValue = property_->getMaxValue();
            vec4MaxValue.z = sliderZ_->getValue();
            property_->setMaxValue(vec4MaxValue);
            updateFromProperty();
        }
    }

}

void FloatVec4PropertyWidgetQt::showContextMenuW( const QPoint& pos ) {
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    PropertyVisibility::VisibilityMode appVisibilityMode  = static_cast<PropertyVisibility::VisibilityMode>(static_cast<OptionPropertyInt*>(inviwoApp->getSettings()->getPropertyByIdentifier("viewMode"))->get());
    if (appVisibilityMode == PropertyVisibility::DEVELOPMENT) {

        QPoint globalPos = sliderW_->mapToGlobal(pos);

        QAction* selecteditem = settingsMenu_->exec(globalPos);
        if (selecteditem == settingsMenu_->actions().at(0)) {
            settingsWidget_->reload();
            settingsWidget_->show();
        }
        else if (selecteditem == settingsMenu_->actions().at(1)) {
            //Set current value of the slider to min value of the property
            vec4 vec4MinValue = property_->getMinValue();
            vec4MinValue.w = sliderW_->getValue();
            property_->setMinValue(vec4MinValue);
            updateFromProperty();
        }
        else if (selecteditem == settingsMenu_->actions().at(2)){
            //Set current value of the slider to max value of the property
            vec4 vec4MaxValue = property_->getMaxValue();
            vec4MaxValue.w = sliderW_->getValue();
            property_->setMaxValue(vec4MaxValue);
            updateFromProperty();
        }
    }
}
void FloatVec4PropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());

}

} // namespace