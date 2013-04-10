#ifndef IVW_COLLAPSIVEGROUPBOXWIDGETQT_H
#define IVW_COLLAPSIVEGROUPBOXWIDGETQT_H

//Qt
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QLineEdit>
#include <QToolButton>
#include <QGroupBox>

//Widgets
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/buttonpropertywidgetqt.h>
//Properties
#include <inviwo/core/properties/compositeproperty.h>
#include <inviwo/core/properties/buttonproperty.h>

namespace inviwo {

    class IVW_QTWIDGETS_API CollapsiveGroupBoxWidgetQt : public PropertyWidgetQt {

        Q_OBJECT;

    public:
        CollapsiveGroupBoxWidgetQt(CompositeProperty* property);

        void updateFromProperty();

    private:
        CompositeProperty* property_;
        ButtonProperty btnProperty_;
        ButtonPropertyWidgetQt* btnWidget_;
        bool visible_;
        QGroupBox* groupBox_;

        void generateWidget();

        std::vector<PropertyWidgetQt*> subPropertyWidgets_;

        public slots:
            void hide();
            void show();
    };

} // namespace

#endif IVW_COLLAPSIVEGROUPBOXWIDGETQT_H