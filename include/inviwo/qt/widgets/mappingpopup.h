#ifndef IVW_MAPPINGPOPUP_H
#define IVW_MAPPINGPOPUP_H

#include <inviwo/core/properties/eventproperty.h>
#include <inviwo/qt/widgets/eventconverterqt.h>
#include <QWidget>
#include <QMouseEvent>

namespace inviwo {

class MappingPopup : public QWidget {
    Q_OBJECT;

public:
    MappingPopup(EventProperty * parentProperty);

protected:
    void  mousePressEvent ( QMouseEvent * event );

private:
    EventProperty* parentProperty_;
    EventConverterQt* eventConverter_;
};

} // namespace

#endif // IVW_MAPPINGPOPUP_H