#ifndef IVW_SETTINGSWIDGET_H
#define IVW_SETTINGSWIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/qt/widgets/inviwodockwidget.h>

#include <QLayout>
#include <QVBoxLayout>

namespace inviwo {

class IVW_QTEDITOR_API SettingsWidget : public InviwoDockWidget {
Q_OBJECT
public:
    SettingsWidget(QWidget* parent);
    SettingsWidget(QString title, QWidget* parent);
    ~SettingsWidget();
    virtual void generateWidget();
    virtual void loadSettings();
    virtual void saveSettings();

protected:
    QVBoxLayout* vLayout_;
};

} // namespace

#endif // IVW_SETTINGSWIDGET_H