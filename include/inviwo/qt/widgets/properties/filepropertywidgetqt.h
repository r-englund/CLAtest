#ifndef IVW_FILEPROPERTYWIDGETQT_H
#define IVW_FILEPROPERTYWIDGETQT_H

#include "inviwo/qt/widgets/inviwoqtwidgetsdefine.h"
#include <QLineEdit>
#include <QToolButton>

#include "inviwo/qt/widgets/properties/propertywidgetqt.h"

#include "inviwo/core/properties/fileproperty.h"

namespace inviwo {

class IVW_QTWIDGETS_API FilePropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    FilePropertyWidgetQt(FileProperty* property);

    void updateFromProperty();

private:
    FileProperty* property_;
    QLineEdit* lineEdit_;
    QToolButton* openButton_;

    void generateWidget();

public slots:
    void setPropertyValue();
};

} // namespace

#endif // IVW_FILEPROPERTYWIDGETQT_H