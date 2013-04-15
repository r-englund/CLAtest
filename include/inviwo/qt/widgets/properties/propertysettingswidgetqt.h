#ifndef IVW_PROPERTYSETTINGSWIDGETQT_H
#define IVW_PROPERTYSETTINGSWIDGETQT_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QGridLayout>
#include <inviwo/core/properties/scalarproperties.h>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/buttonpropertywidgetqt.h>
#include <inviwo/core/properties/buttonproperty.h>


namespace inviwo {
class FloatPropertyWidgetQt;
class IVW_QTWIDGETS_API PropertySettingsWidgetQt : public QWidget {

    Q_OBJECT;
public:
    PropertySettingsWidgetQt(Property *property);
    
private:
    Property* property_;
    QLineEdit* lineEditMax_;
    QLineEdit* lineEditMin_;
    QLineEdit* lineEditIcrement_;
    ButtonProperty btnPropertyApply_;
    //ButtonProperty btnPropertyReload_;
    ButtonProperty btnPropertyCancel_;
    ButtonPropertyWidgetQt* btnApply_;
    //ButtonPropertyWidgetQt* btnReload_;
    ButtonPropertyWidgetQt* btnCancel_;
    void generateWidget();

public slots:
    void save();
    void reload();
    void cancel();

};

}//namespace

#endif //IVW_PROPERTYSETTINGSWIDGETQT_H



