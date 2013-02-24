#ifndef IVW_KEYMAPPROPERTYWIDGETQT_H
#define IVW_KEYMAPPROPERTYWIDGETQT_H


#include <QWidget>
#include <inviwo/core/properties/property.h>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <QMainWindow>


namespace inviwo{
  
    class IVW_QTWIDGETS_API KeyMapPropertyWidgetQt : public PropertyWidgetQt{

        Q_OBJECT;

    public:

        KeyMapPropertyWidgetQt(Property* property);
        void updateFromProperty();

    private:

        Property* property_;
        QWidget* window_;
        void generateWidget();

    };


}//namespace

#endif //IVW_KEYMAPPROPERTYWIDGETQT_H