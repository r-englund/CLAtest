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
    //TFunctionPointer<ButtonProperty>::Type funcPtr;

	void generateWidget();
    void consolePrinter();

public slots:
	void handleButton();
};

} //namespace

#endif // IVW_BUTTONPROPERTYWIDGETQT_H