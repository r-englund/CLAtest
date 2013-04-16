#ifndef IVW_COLLAPSIVEGROUPBOXWIDGETQT_H
#define IVW_COLLAPSIVEGROUPBOXWIDGETQT_H

//Qt
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QLineEdit>
#include <QToolButton>
#include <QGroupBox>
#include <QPushButton>

//Widgets
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/properties/property.h>


namespace inviwo {

    class IVW_QTWIDGETS_API CollapsiveGroupBoxWidgetQt : public PropertyWidgetQt {

        Q_OBJECT;

    public:
        CollapsiveGroupBoxWidgetQt();

        void updateFromProperty();
        void addProperty(Property* tmpProperty);
        QVBoxLayout* vLayout_;
        void generatePropertyWidgets();

    private:
        QPushButton* btnCollapse_;
        QGroupBox* groupBox_;
        std::vector<Property*> properties_;

        void generateWidget();

        public slots:
            void hide();
            void show();
    };

} // namespace

#endif IVW_COLLAPSIVEGROUPBOXWIDGETQT_H