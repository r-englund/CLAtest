#ifndef IVW_TRANSFERPROPERTYWIDGETQT_H
#define IVW_TRANSFERPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QCheckBox>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <inviwo/qt/editor/transfereditor.h>

namespace inviwo {

class IVW_QTWIDGETS_API TransferFunctionPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    TransferFunctionPropertyWidgetQt(TransferFunctionProperty* property);
    void updateFromProperty();

private:
    static const std::string logSource_;
    TransferFunctionProperty* property_;
	QGraphicsView*	view;
    QCheckBox* checkBox_;
    QGraphicsScene* scene;
	TransferEditor* editor;

    void generateWidget();

    QGraphicsView* paintview;
    QGraphicsScene* paintscene;
    ImageRAMfloat16* transferImage;
    //TransferFunction* func;

public slots:
    void setPropertyValue();
};

} // namespace

#endif // IVW_TRANSFERPROPERTYWIDGETQT_H