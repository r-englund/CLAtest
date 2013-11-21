#ifndef IVW_LABELGRAPHICSITEM_H
#define IVW_LABELGRAPHICSITEM_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QGraphicsTextItem>
#include <inviwo/core/util/observer.h>

namespace inviwo {

class IVW_QTWIDGETS_API LabelGraphicsItem : public QGraphicsTextItem, public VoidObservable {

public:
    LabelGraphicsItem(QGraphicsItem* parent);
    ~LabelGraphicsItem();

    QString text() const;
    void setText(const QString &);

    QString croppedText() const;
    void setCrop(int, int);
    bool isCropped() const;

    void setNoFocusOut();
    bool isFocusOut() const;

protected:
    bool doCrop(const QString &str);
    void updateCrop();

    void keyPressEvent(QKeyEvent* event);
    void focusInEvent(QFocusEvent* event);
    void focusOutEvent(QFocusEvent* event);

private:
    int maxBefore_;
    int maxAfter_;
    unsigned int labelHeight_;
    bool focusOut_;
    QString orgText_;
};

} // namespace

#endif // IVW_LABELGRAPHICSITEM_H