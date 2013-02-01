#ifndef IVW_TRANSFERPROPERTYWIDGETQT_H
#define IVW_TRANSFERPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QCheckBox>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/properties/transferproperty.h>
#include "../../editor/transfereditor.h"

namespace inviwo {

class IVW_QTWIDGETS_API TransferPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:

    TransferPropertyWidgetQt(TransferProperty* property);

    void updateFromProperty();

private:
    TransferProperty* property_;
	QGraphicsView*	view;
	QGraphicsScene* scene;
    QCheckBox* checkBox_;
	TransferEditor editor;
	static const std::string logSource_;
    void generateWidget();
	void mousePressEvent(QGraphicsSceneMouseEvent * e);
	void mouseMoveEvent(QGraphicsSceneMouseEvent* e);

public slots:
    void setPropertyValue();
};

} // namespace

#endif // IVW_TRANSFERPROPERTYWIDGETQT_H