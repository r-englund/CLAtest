#ifndef IVW_BUTTONPROPERTYWIDGETQT_H
#define IVW_BUTTONPROPERTYWIDGETQT_H

#include <QPushButton>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/buttonproperty.h>



namespace inviwo {


class ButtonPropertyWidgetQt : public PropertyWidgetQt {

	Q_OBJECT;

public:
	ButtonPropertyWidgetQt(ButtonProperty* property);

    void updateFromProperty();
    QPushButton* getButton();
private:
	ButtonProperty* property_;
        QPushButton* button_;


	void generateWidget();

public slots:
	void handleButton();
    void visibilityModified(int mode){property_->setVisibility(static_cast<PropertyVisibility::VisibilityMode>(mode));};
};

} //namespace

#endif // IVW_BUTTONPROPERTYWIDGETQT_H