#ifndef IVW_COLLAPSIVEGROUPBOXWIDGETQT_H
#define IVW_COLLAPSIVEGROUPBOXWIDGETQT_H

//Qt
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <QLineEdit>
#include <QToolButton>
#include <QGroupBox>
#include <QPushButton>

//Widgets
#include <inviwo/qt/widgets/eventpropertymanager.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
#include <inviwo/qt/widgets/propertylistwidget.h>

#include <inviwo/core/properties/property.h>

namespace inviwo {

    class IVW_QTWIDGETS_API CollapsiveGroupBoxWidgetQt : public PropertyWidgetQt {

        Q_OBJECT

    public:
        CollapsiveGroupBoxWidgetQt(std::string identifier, std::string displayName= "");

        virtual std::string getIdentifier() const;
        virtual void setIdentifier(const std::string& identifier);

        virtual std::string getDisplayName() const;
        virtual void setDisplayName(const std::string& displayName);

        void setVisibilityMode(PropertyVisibility::VisibilityMode visibilityMode){visibilityMode_ = visibilityMode;};
        PropertyVisibility::VisibilityMode getVisibilityMode(){return visibilityMode_;};

        void updateFromProperty();
        void addProperty(Property* tmpProperty);
        QVBoxLayout* vLayout_;
        void generatePropertyWidgets();
        void generateEventPropertyWidgets(EventPropertyManager* eventPropertyManager);
        std::vector<Property*> getProperties();
        bool isCollapsed() { return collapsed_; };
        std::vector<PropertyWidgetQt*> getPropertyWidgets(){return propertyWidgets_; };
        void addWidget(QWidget* widget);

        virtual void serialize(IvwSerializer& s) const;
        virtual void deserialize(IvwDeserializer& d);


signals:
        void visibilityModified();


    private:
        QToolButton* btnCollapse_;
        std::string identifier_;
        std::string displayName_;
        QGroupBox* groupBox_;
        std::vector<Property*> properties_;
        std::vector<PropertyWidgetQt*> propertyWidgets_;
        bool collapsed_;
        EditableLabelQt* label_;
        void generateContextMenu();
        void generateWidget();
        void updateContextMenu();
        PropertyVisibility::VisibilityMode visibilityMode_;
        void updateWidgets();

        QMenu* contextMenu_;
        QActionGroup* viewModeActionGroup_;
        QMenu* viewModeItem_;

        public slots:
            void hide();
            void show();
            void updateVisibility();
            void showContextMenu(const QPoint& pos);
            void setDeveloperViewMode(bool value);
            void setApplicationViewMode(bool value);

            protected slots:
                void setGroupDisplayName();
                void propertyModified();

    };

} // namespace

#endif //IVW_COLLAPSIVEGROUPBOXWIDGETQT_H