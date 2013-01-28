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


private:
	ButtonProperty* property_;
	QPushButton* button_;

	void generateWidget();

public slots:
	void handleButton();
	//void setLabel(std::string value);
	//void setPropertyValue();
};

} //namespace

#endif // IVW_BUTTONPROPERTYWIDGETQT_H