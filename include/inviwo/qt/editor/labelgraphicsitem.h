#ifndef IVW_LABELGRAPHICSITEM_H
#define IVW_LABELGRAPHICSITEM_H

#include <QGraphicsTextItem>

namespace inviwo {

class LabelGraphicsItem : public QGraphicsTextItem {

public:
    LabelGraphicsItem(QGraphicsItem* parent);
    ~LabelGraphicsItem();

protected:
    void focusOutEvent(QFocusEvent* event);

    unsigned int labelHeight_;
};

} // namespace

#endif // IVW_LABELGRAPHICSITEM_H