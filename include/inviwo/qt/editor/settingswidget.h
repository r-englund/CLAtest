#ifndef IVW_SETTINGSWIDGET_H
#define IVW_SETTINGSWIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <inviwo/qt/editor/inviwodockwidget.h>

#include <QLayout>
#include <QVBoxLayout>

namespace inviwo {

class IVW_QTEDITOR_API SettingsWidget : public InviwoDockWidget {
Q_OBJECT
public:
    SettingsWidget(QWidget* parent);
    ~SettingsWidget();

    void loadSettings();
    void saveSettings();

private:
    QVBoxLayout* vLayout_;
};

} // namespace

#endif // IVW_SETTINGSWIDGET_H