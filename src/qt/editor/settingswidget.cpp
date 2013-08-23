#include <inviwo/qt/editor/settingswidget.h>
#include <inviwo/core/common/inviwoapplication.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>

#include <QFrame>
#include <QSettings>

namespace inviwo {

SettingsWidget::SettingsWidget(QString title, QWidget* parent) : InviwoDockWidget(title, parent) {
    generateWidget();
}

SettingsWidget::SettingsWidget(QWidget* parent) : InviwoDockWidget(tr("Settings"), parent) {
    generateWidget();
}

void SettingsWidget::generateWidget() {
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
    Settings* mainsettings = InviwoApplication::getRef().getSettings();
    std::vector<Property*> properties = mainsettings->getProperties();
    QSettings qmainsettings("Inviwo", "Inviwo");
    qmainsettings.beginGroup("mainsettings");
    QStringList keys = qmainsettings.allKeys();
    for (size_t i=0; i<properties.size(); i++) {
        Property* curProperty = properties[i];
        QString name = QString::fromStdString(curProperty->getIdentifier());
        if(keys.contains(name)){
            QVariant qval = qmainsettings.value(name);
            Variant val(std::string(qval.toString().toLocal8Bit().constData()));
            curProperty->setVariant(val);
        }
        PropertyWidgetQt* propertyWidget = PropertyWidgetFactoryQt::getRef().create(curProperty);
        vLayout_->addWidget(propertyWidget);
        curProperty->registerPropertyWidget(propertyWidget);
    }
    qmainsettings.endGroup();
}

//Save application settings to QSettings
void SettingsWidget::saveSettings() {
    Settings* mainsettings = InviwoApplication::getRef().getSettings();
    std::vector<Property*> properties = mainsettings->getProperties();
    QSettings qmainsettings("Inviwo", "Inviwo");
    qmainsettings.beginGroup("mainsettings");
    for (size_t i=0; i<properties.size(); i++) {
        Property* curProperty = properties[i];
        qmainsettings.setValue(QString::fromStdString(curProperty->getIdentifier()), QVariant(QString::fromStdString(curProperty->getVariant().getString())));
    }
    qmainsettings.endGroup();
}

} // namespace