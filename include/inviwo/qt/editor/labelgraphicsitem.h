#ifndef IVW_LABELGRAPHICSITEM_H
#define IVW_LABELGRAPHICSITEM_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsSimpleTextItem>

namespace inviwo {

class IVW_QTEDITOR_API LabelGraphicsItem : public QGraphicsSimpleTextItem {

public:
    LabelGraphicsItem(QGraphicsItem* parent);
    ~LabelGraphicsItem();

protected:
    void focusOutEvent(QFocusEvent* event);

    unsigned int labelHeight_;
};

} // namespace

#endif // IVW_LABELGRAPHICSITEM_H