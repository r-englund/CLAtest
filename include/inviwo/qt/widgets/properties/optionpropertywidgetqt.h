#ifndef IVW_OPTIONPROPERTYWIDGETQT_H

#define IVW_OPTIONPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>

#include <QComboBox>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <typeinfo>

namespace inviwo {

class IVW_QTWIDGETS_API OptionPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:

    OptionPropertyWidgetQt(BaseOptionProperty* property);

    void updateFromProperty();

private:

    BaseOptionProperty* property_;
    QComboBox* comboBox_;
    EditableLabelQt* label_;
    void generateWidget();
    void fillComboBox();

public slots:
     void optionChanged();
     void setPropertyDisplayName();
};

} // namespace

#endif // IVW_OPTIONPROPERTYWIDGETQT_H