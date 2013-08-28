#ifndef IVW_VISIBILITYWIDGETQT_H
#define IVW_VISIBILITYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>


#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/properties/baseoptionproperty.h>
#include <inviwo/core/properties/optionproperties.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/customcomboboxqt.h>
#include <typeinfo>

namespace inviwo {

    class IVW_QTWIDGETS_API VisibilityWidgetQt : public PropertyWidgetQt {

        Q_OBJECT;

    public:

        VisibilityWidgetQt(BaseOptionProperty* property);

        void updateFromProperty();

    private:

        BaseOptionProperty* property_;
        CustomComboBoxQt* comboBox_;
        EditableLabelQt* label_;
        void generateWidget();
        void fillComboBox();

        public slots:
            void optionChanged();
            
signals:
     void optionChanged(int option);
    };

} // namespace

#endif // IVW_VISIBILITYWIDGETQT_H