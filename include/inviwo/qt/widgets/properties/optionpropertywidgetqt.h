#ifndef IVW_OPTIONPROPERTYWIDGETQT_H

#define IVW_OPTIONPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>

#include <QComboBox>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/property.h>

namespace inviwo {

class IVW_QTWIDGETS_API OptionPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:

    OptionPropertyWidgetQt(BaseOptionProperty* property);

    void updateFromProperty();

private:

    BaseOptionProperty* property_;
    QComboBox* comboBox_;

    void generateWidget();
    void fillComboBox();

public slots:
     void optionChanged();
};

} // namespace

#endif // IVW_OPTIONPROPERTYWIDGETQT_H