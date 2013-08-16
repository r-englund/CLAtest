#ifndef IVW_COLLAPSIVEGROUPBOXWIDGETQT_H
#define IVW_COLLAPSIVEGROUPBOXWIDGETQT_H

//Qt
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QLineEdit>
#include <QToolButton>
#include <QGroupBox>
#include <QPushButton>

//Widgets
#include <inviwo/qt/editor/eventpropertymanager.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/property.h>

namespace inviwo {

class IVW_QTWIDGETS_API CollapsiveGroupBoxWidgetQt : public PropertyWidgetQt, public VoidObserver {

    Q_OBJECT;

public:
    CollapsiveGroupBoxWidgetQt(std::string identifier, std::string displayName= "");

    virtual std::string getIdentifier() const;
    virtual void setIdentifier(const std::string& identifier);

    virtual std::string getDisplayName() const;
    virtual void setDisplayName(const std::string& displayName);


    void updateFromProperty();
    void addProperty(Property* tmpProperty);
    QVBoxLayout* vLayout_;
    void generatePropertyWidgets();
	void generateEventPropertyWidgets(EventPropertyManager* eventPropertyManager);
    std::vector<Property*> getProperties();
	bool isCollapsed() { return collapsed_; };

    void notify();

private:
    //QPushButton* btnCollapse_;
    QToolButton* btnCollapse_;
    std::string identifier_;
    std::string displayName_;
    QGroupBox* groupBox_;
    std::vector<Property*> properties_;
	bool collapsed_;

    void generateWidget();

    public slots:
        void hide();
        void show();
};

} // namespace

#endif //IVW_COLLAPSIVEGROUPBOXWIDGETQT_H