#ifndef IVW_TRANSFERFUNCTIONPROPERTYWIDGET_H
#define IVW_TRANSFERFUNCTIONPROPERTYWIDGET_H

#include <inviwo/core/properties/properties.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/transferfunctionpropertydialog.h>

#include <QHBoxLayout>
#include <QPushButton>

namespace inviwo {

class IVW_QTWIDGETS_API TransferFunctionPropertyWidgetQt : public PropertyWidgetQt {
	Q_OBJECT;

public:
	TransferFunctionPropertyWidgetQt(TransferFunctionProperty* property);

	void updateFromProperty();

private:
	TransferFunctionProperty* property_;
	QPushButton* btnOpenDialog_;
	QColorDialog* colorDialog_;
	QColor* currentColor_;

	TransferFunctionPropertyDialog* transferFunctionDialog_;

	void generateWidget();

	public slots:
		void setPropertyValue();
		void openTransferFunctionDialog();
};

}//namespace

#endif //IVW_TRANSFERFUNCTIONPROPERTYWIDGET_H