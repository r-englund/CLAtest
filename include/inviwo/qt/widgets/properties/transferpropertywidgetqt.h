#ifndef IVW_TRANSFERPROPERTYWIDGETQT_H
#define IVW_TRANSFERPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QCheckBox>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/properties/transferproperty.h>
#include <inviwo/qt/editor/transfereditor.h>

namespace inviwo {

class IVW_QTWIDGETS_API TransferPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    TransferPropertyWidgetQt(TransferProperty* property);
    void updateFromProperty();

private:
    TransferProperty* property_;
	QGraphicsView*	view;
    QCheckBox* checkBox_;
    QGraphicsScene* scene;
	TransferEditor* editor;
	static const std::string logSource_;
    void generateWidget();

    QGraphicsView* paintview;
    QGraphicsScene* paintscene;

public slots:
    void setPropertyValue();
};

} // namespace

#endif // IVW_TRANSFERPROPERTYWIDGETQT_H