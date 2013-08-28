#ifndef IVW_STRINGPROPERTYWIDGETQT_H
#define IVW_STRINGPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <QLineEdit>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/stringproperty.h>

namespace inviwo {

class IVW_QTWIDGETS_API StringPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    StringPropertyWidgetQt(StringProperty* property);

    void updateFromProperty();

private:
    StringProperty* property_;
    QLineEdit* lineEdit_;
    EditableLabelQt* label_;

    void generateWidget();

    public slots:
        void setPropertyValue();
        void setPropertyDisplayName();
};

} // namespace

#endif // IVW_STRINGPROPERTYWIDGETQT_H