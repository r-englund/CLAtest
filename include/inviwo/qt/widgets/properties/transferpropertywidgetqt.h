#ifndef IVW_TRANSFERPROPERTYWIDGETQT_H
#define IVW_TRANSFERPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QCheckBox>
#include <QPixmap>
#include <QPainter>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/transferproperty.h>

namespace inviwo {

class IVW_QTWIDGETS_API TransferPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    TransferPropertyWidgetQt(TransferProperty* property);

    void updateFromProperty();

private:
    TransferProperty* property_;
    QCheckBox* checkBox_;

    void generateWidget();

public slots:
    void setPropertyValue();
};

} // namespace

#endif // IVW_TRANSFERPROPERTYWIDGETQT_H