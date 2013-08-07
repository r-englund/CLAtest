#ifndef IVW_LABELGRAPHICSITEM_H
#define IVW_LABELGRAPHICSITEM_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsTextItem>

namespace inviwo {

class IVW_QTEDITOR_API LabelGraphicsItem : public QGraphicsTextItem {

public:
    LabelGraphicsItem(QGraphicsItem* parent);
    ~LabelGraphicsItem();

    QString text() const;
    void setText(const QString &);

    QString croppedText() const;
    void setCrop(int, int);
    bool isCropped() const;

protected:
    bool doCrop(const QString &str);
    void updateCrop();
    void updateGraphicsItem();

    void keyPressEvent(QKeyEvent* event);
    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);

private:
    int maxBefore_;
    int maxAfter_;
    unsigned int labelHeight_;
    QString orgText_;
};

} // namespace

#endif // IVW_LABELGRAPHICSITEM_H