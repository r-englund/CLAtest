#ifndef IVW_MAPPINGPOPUP_H
#define IVW_MAPPINGPOPUP_H

//#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/properties/eventproperty.h>
#include <QWidget>
#include <QMouseEvent>
//#include <iostream>

namespace inviwo {

class MappingPopup : public QWidget {
    Q_OBJECT;

public:
    MappingPopup(EventProperty * parentProperty);

protected:
    void  mousePressEvent ( QMouseEvent * event );

private:
    EventProperty* parentProperty_;

};

} // namespace

#endif // IVW_MAPPINGPOPUP_H