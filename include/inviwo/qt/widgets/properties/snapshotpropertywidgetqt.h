#ifndef IVW_SNAPSHOTPROPERTYWIDGETQT_H
#define IVW_SNAPSHOTPROPERTYWIDGETQT_H

#include <QPushButton>
#include <inviwo/core/properties/snapshotproperty.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

namespace inviwo {

/** \brief Qt widget for the SnapshotProperty.
 *  Represented graphically through a simple button, which triggers
 *  a file selection dialog where the output path is set.
 */
class SnapshotPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    SnapshotPropertyWidgetQt(SnapshotProperty* property);

    void updateFromProperty();

private:
    SnapshotProperty* property_;
    QPushButton* button_;

    void generateWidget();
    void consolePrinter();

public slots:
    void handleButton();
};

} //namespace

#endif // IVW_SNAPSHOTPROPERTYWIDGETQT_H