#ifndef IVW_FILEPROPERTYWIDGETQT_H
#define IVW_FILEPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <QLineEdit>
#include <QToolButton>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/fileproperty.h>

namespace inviwo {

class IVW_QTWIDGETS_API FilePropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:
    FilePropertyWidgetQt(FileProperty* property);

    void updateFromProperty();

private:
    FileProperty* property_;
    QLineEdit* lineEdit_;
    QToolButton* openButton_;
    EditableLabelQt* label_;

    void generateWidget();

public slots:
    void setPropertyValue();
    void setPropertyDisplayName();
};

} // namespace

#endif // IVW_FILEPROPERTYWIDGETQT_H