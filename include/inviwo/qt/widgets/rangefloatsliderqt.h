#ifndef IVW_RANGEFLOATSLIDERQT_H
#define IVW_RANGEFLOATSLIDERQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QtGui/QSplitter>
#include <QResizeEvent>

namespace inviwo {

class IVW_QTWIDGETS_API RangeFloatSliderQt : public QSplitter {
    Q_OBJECT;

public:
    RangeFloatSliderQt(QWidget *parent = NULL);
    virtual ~RangeFloatSliderQt();

public slots:
    int value();
    void setValue(int);

signals:
    void valueChanged(float value);

protected:
    void resizeEvent(QResizeEvent *event);
};

}//namespace

#endif // IVW_RANGEFLOATSLIDERQT_H