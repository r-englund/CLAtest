#include <inviwo/qt/editor/settingswidget.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>

#include <QFrame>

namespace inviwo {

SettingsWidget::SettingsWidget(QWidget* parent) : InviwoDockWidget(tr("Settings"), parent) {
    setObjectName("SettingsWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QFrame* frame = new QFrame();
    vLayout_ = new QVBoxLayout(frame);
    vLayout_->setSpacing(0);

    frame->setLayout(vLayout_);
    setWidget(frame);
}

SettingsWidget::~SettingsWidget() {}

//Load settings from QSettings
void SettingsWidget::loadSettings() {
    Settings* settings = InviwoApplication::getRef().getSettings();
    std::vector<Property*> properties = settings->getProperties();
    for (size_t i=0; i<properties.size(); i++) {
        Property* curProperty = properties[i];
        PropertyWidgetQt* propertyWidget = PropertyWidgetFactoryQt::getRef().create(curProperty);
        vLayout_->addWidget(propertyWidget);
        curProperty->registerPropertyWidget(propertyWidget);
    }
    vLayout_->addStretch(1);
}

//Save application settings to QSettings
void SettingsWidget::saveSettings() {
}

} // namespace