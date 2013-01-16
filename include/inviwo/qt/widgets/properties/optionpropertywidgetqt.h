#ifndef IVW_OPTIONPROPERTYWIDGETQT_H
#define IVW_OPTIONPROPERTYWIDGETQT_H

#include "inviwo/qt/widgets/inviwoqtwidgetsdefine.h"
#include <QCheckBox>

#include "inviwo/qt/widgets/properties/propertywidgetqt.h"

#include "inviwo/core/properties/optionproperty.h"

namespace inviwo {

class IVW_QTWIDGETS_API OptionPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    OptionPropertyWidgetQt(OptionProperty* property);

    void updateFromProperty();

private:
    OptionProperty* property_;
    QCheckBox* checkBox_;

    void generateWidget();

public slots:
    void setPropertyValue();
};

} // namespace

#endif // IVW_OPTIONPROPERTYWIDGETQT_H