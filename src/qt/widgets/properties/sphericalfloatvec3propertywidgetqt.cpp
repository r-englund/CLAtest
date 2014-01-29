#include <inviwo/qt/widgets/properties/sphericalfloatvec3propertywidgetqt.h>

#include <QHBoxLayout>
#include <QLabel>


namespace inviwo {

SphericalFloatVec3PropertyWidgetQt::SphericalFloatVec3PropertyWidgetQt(FloatVec3Property* property)
    : property_(property)
    , valueVec3Max_(property->getMaxValue())
    , valueVec3Min_(property->getMinValue())
    , valueIncrement_(property->getIncrement()) {

    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
    generateWidget();
    updateFromProperty();

    if(!property->getReadOnly())
        generatesSettingsWidget();
}

void SphericalFloatVec3PropertyWidgetQt::generateWidget() {
    QHBoxLayout* hLayout = new QHBoxLayout();
    if(property_->getReadOnly()) {
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        readOnlyLabel_ = new QLabel();
        hLayout->addWidget(readOnlyLabel_);
        setLayout(hLayout);
    } else {
        label_ = new EditableLabelQt(this, property_->getDisplayName(), PropertyWidgetQt::generatePropertyWidgetMenu());
        hLayout->addWidget(label_);

        QWidget* sliderWidget = new QWidget();
        QVBoxLayout* vLayout = new QVBoxLayout();
        sliderWidget->setLayout(vLayout);
        vLayout->setContentsMargins(0, 0, 0, 0);
        vLayout->setSpacing(0);

        sliderR_ = new FloatSliderWidgetQt();
        sliderTheta_ = new FloatSliderWidgetQt();
        sliderPhi_ = new FloatSliderWidgetQt();

        vLayout->addWidget(sliderR_);
        vLayout->addWidget(sliderTheta_);
        vLayout->addWidget(sliderPhi_);
        hLayout->addWidget(sliderWidget);
        setLayout(hLayout);

        QSizePolicy labelPol = label_->sizePolicy();
        labelPol.setHorizontalStretch(1);
        labelPol.setControlType(QSizePolicy::Label);
        label_->setSizePolicy(labelPol);

        QSizePolicy slidersPol = sliderWidget->sizePolicy();
        slidersPol.setHorizontalStretch(3);
        sliderWidget->setSizePolicy(slidersPol);

        connect(label_, SIGNAL(textChanged()), this, SLOT(setPropertyDisplayName()));
        connect(sliderR_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
        connect(sliderTheta_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));
        connect(sliderPhi_, SIGNAL(valueChanged(float)), this, SLOT(setPropertyValue()));

    }
}

void SphericalFloatVec3PropertyWidgetQt::updateFromProperty() {
    if(property_->getReadOnly()) {
        readOnlyLabel_->setText(QString::number(property_->get().x) + ","
                                + QString::number(property_->get().y) + ","
                                + QString::number(property_->get().z));


        readOnlyLabel_->setToolTip("Min: [" + QString::number((property_->getMinValue()).x) +
                                    "," + QString::number((property_->getMinValue()).y) +
                                    "," + QString::number((property_->getMinValue()).z) +
                                    "] Max: [" + QString::number((property_->getMaxValue()).x) +
                                    "," + QString::number((property_->getMaxValue()).y) +
                                    "," + QString::number((property_->getMinValue()).z) + "]");


    } else {
        valueVec3Max_ = property_->getMaxValue();
        valueVec3Min_ = property_->getMinValue();
        valueIncrement_ = property_->getIncrement();

        vec3 initVal = cartesianToSpherical(property_->get());

        sliderR_->initValue(initVal.x);
        sliderTheta_->initValue(initVal.y);
        sliderPhi_->initValue(initVal.z);

        sliderR_->setRange(0, glm::length(valueVec3Max_));
        sliderTheta_->setRange(-M_PI, M_PI);
        sliderPhi_->setRange(-M_PI / 2.0f, M_PI / 2.0f);

        sliderR_->setValue(initVal.x);
        sliderTheta_->setValue(initVal.y);
        sliderPhi_->setValue(initVal.z);

        vec3 inc = cartesianToSpherical(valueIncrement_);

        sliderR_->setIncrement(inc.x);
        sliderTheta_->setIncrement(inc.y);
        sliderPhi_->setIncrement(inc.z);
    }
}


void SphericalFloatVec3PropertyWidgetQt::generatesSettingsWidget() {
    settingsWidget_ = new PropertySettingsWidgetQt(property_);

    settingsMenu_ = PropertyWidgetQt::generatePropertyWidgetMenu();
    settingsMenu_->addAction("Property settings");
    settingsMenu_->addAction("Set as Min");
    settingsMenu_->addAction("Set as Max");

    sliderR_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderTheta_->setContextMenuPolicy(Qt::CustomContextMenu);
    sliderPhi_->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(sliderR_, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenuX(const QPoint&)));
    connect(sliderTheta_, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenuY(const QPoint&)));
    connect(sliderPhi_, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showContextMenuZ(const QPoint&)));
    connect(developerViewModeAction_, SIGNAL(triggered(bool)), this, SLOT(setDeveloperViewMode(bool)));
    connect(applicationViewModeAction_, SIGNAL(triggered(bool)), this, SLOT(setApplicationViewMode(bool)));

    updateContextMenu();
}

void SphericalFloatVec3PropertyWidgetQt::showContextMenuX(const QPoint& pos) {
    PropertyVisibilityMode appVisibilityMode = getApplicationViewMode();
    if(appVisibilityMode == DEVELOPMENT) {
        updateContextMenu();
        QPoint globalPos = sliderR_->mapToGlobal(pos);
        QAction* selecteditem = settingsMenu_->exec(globalPos);
        if(selecteditem && selecteditem->text() == "Property settings") {
            settingsWidget_->reload();
            settingsWidget_->show();
        } else if(selecteditem && selecteditem->text() == "Set as Min") {
            //Set current value of the slider to min value of the property
            valueVec3Min_ = property_->getMinValue();
            valueVec3Min_.x = sliderR_->getValue();
            property_->setMinValue(valueVec3Min_);
            updateFromProperty();
        } else if(selecteditem && selecteditem->text() == "Set as Max") {
            //Set current value of the slider to max value of the property
            valueVec3Max_ = property_->getMaxValue();
            valueVec3Max_.x = sliderR_->getValue();
            property_->setMaxValue(valueVec3Max_);
            updateFromProperty();
        }
    }
}

void SphericalFloatVec3PropertyWidgetQt::showContextMenuY(const QPoint& pos) {
    PropertyVisibilityMode appVisibilityMode = getApplicationViewMode();
    if(appVisibilityMode == DEVELOPMENT) {
        updateContextMenu();
        QPoint globalPos = sliderTheta_->mapToGlobal(pos);
        QAction* selecteditem = settingsMenu_->exec(globalPos);
        if(selecteditem && selecteditem->text() == "Property settings") {
            settingsWidget_->reload();
            settingsWidget_->show();
        } else if(selecteditem && selecteditem->text() == "Set as Min") {
            //Set current value of the slider to min value of the property
            valueVec3Min_ = property_->getMinValue();
            valueVec3Min_.y = sliderTheta_->getValue();
            property_->setMinValue(valueVec3Min_);
            updateFromProperty();
        } else if(selecteditem && selecteditem->text() == "Set as Max") {
            //Set current value of the slider to max value of the property
            valueVec3Max_ = property_->getMaxValue();
            valueVec3Max_.y = sliderTheta_->getValue();
            property_->setMaxValue(valueVec3Max_);
            updateFromProperty();
        }
    }
}

void SphericalFloatVec3PropertyWidgetQt::showContextMenuZ(const QPoint& pos) {
    PropertyVisibilityMode appVisibilityMode = getApplicationViewMode();
    if(appVisibilityMode == DEVELOPMENT) {
        updateContextMenu();
        QPoint globalPos = sliderPhi_->mapToGlobal(pos);

        QAction* selecteditem = settingsMenu_->exec(globalPos);
        if(selecteditem && selecteditem->text() == "Property settings") {
            settingsWidget_->reload();
            settingsWidget_->show();
        } else if(selecteditem && selecteditem->text() == "Set as Min") {
            //Set current value of the slider to min value of the property
            valueVec3Min_ = property_->getMinValue();
            valueVec3Min_.z = sliderPhi_->getValue();
            property_->setMinValue(valueVec3Min_);
            updateFromProperty();
        } else if(selecteditem && selecteditem->text() == "Set as Max") {
            //Set current value of the slider to max value of the property
            valueVec3Max_ = property_->getMaxValue();
            valueVec3Max_.z = sliderPhi_->getValue();
            property_->setMaxValue(valueVec3Max_);
            updateFromProperty();
        }
    }
}

void SphericalFloatVec3PropertyWidgetQt::setPropertyValue() {
    property_->set(sphericalToCartesian(vec3(sliderR_->getValue(),
                                             sliderTheta_->getValue(),
                                             sliderPhi_->getValue())));
    emit modified();
}

void SphericalFloatVec3PropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}

vec3 SphericalFloatVec3PropertyWidgetQt::cartesianToSpherical(vec3 v) {
    float r = glm::length(v);
    float theta = std::acos(v.z / r);
    float pi = static_cast<float>(M_PI);
    while(theta < -pi) {
        theta += 2.f * pi;
    }
    while(theta > pi) {
        theta -= 2.f * pi;
    }
    float phi = std::atan(v.y / v.x);
    while(phi < -pi / 2.f) {
        phi += pi;
    }
    while(phi > pi / 2.f) {
        phi -= pi;
    }
    return vec3(r, std::acos(v.z / r), std::atan(v.y / v.x));
}

vec3 SphericalFloatVec3PropertyWidgetQt::sphericalToCartesian(vec3 v) {
    return vec3(v.x * std::sin(v.y) * std::cos(v.z),
                v.x * std::sin(v.y) * std::sin(v.z),
                v.x * std::cos(v.y));
}


} // namespace

