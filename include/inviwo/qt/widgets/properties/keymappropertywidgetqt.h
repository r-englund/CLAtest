#ifndef IVW_KEYMAPPROPERTYWIDGETQT_H
#define IVW_KEYMAPPROPERTYWIDGETQT_H


#include <QWidget>
#include <QPushButton>
#include <inviwo/core/properties/property.h>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/interaction/trackballkeymapper.h>
#include <inviwo/core/properties/keymapproperty.h>
#include <inviwo/core/interaction/events/mouseevent.h>
#include <QMainWindow>


namespace inviwo{
  
    class IVW_QTWIDGETS_API KeyMapPropertyWidgetQt : public PropertyWidgetQt{

        Q_OBJECT;

    public:

        KeyMapPropertyWidgetQt(KeyMapProperty* property);
        void updateFromProperty();
        void setKeyMapper(TrackballKeyMapper *keymapper);

    private:
        QString intToQString(int num);
        TrackballKeyMapper *keymapper_;
        KeyMapProperty* property_;
        QWidget* window_;
        void generateWidget();

    };


}//namespace

#endif //IVW_KEYMAPPROPERTYWIDGETQT_H