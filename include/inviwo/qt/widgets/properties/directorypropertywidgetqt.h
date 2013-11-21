#ifndef IVW_DIRECTORYPROPERTYWIDGETQT_H
#define IVW_DIRECTORYPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <QLineEdit>
#include <QToolButton>

#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/properties/directoryproperty.h>
#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/stringpropertywidgetqt.h>

namespace inviwo {

class IVW_QTWIDGETS_API DirectoryPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:
    DirectoryPropertyWidgetQt(DirectoryProperty* property);

    void updateFromProperty();

private:
    DirectoryProperty* property_;
    QLineEdit* lineEdit_;
    QToolButton* openButton_;
    EditableLabelQt* directoryLabel_;
    void generateWidget();

public slots:
    void setPropertyValue();
    void setPropertyTreeInfo(std::string path);
    void setPropertyDisplayName();
};

} // namespace

#endif // IVW_DIRECTORYPROPERTYWIDGETQT_H