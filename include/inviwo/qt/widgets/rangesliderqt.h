#ifndef IVW_RANGESLIDERQT_H
#define IVW_RANGESLIDERQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QtGui/QSplitter>
#include <QResizeEvent>

namespace inviwo {

class IVW_QTWIDGETS_API RangeSliderQt : public QSplitter {
    Q_OBJECT;

public:
    RangeSliderQt(Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = NULL);
    virtual ~RangeSliderQt();

public slots:
    int minValue();
    int maxValue();
    int minRange();
    int maxRange();

    void setValue(int, int);
    void setMinValue(int);
    void setMaxValue(int);

    void setRange(int, int);
    void setMinRange(int);
    void setMaxRange(int);

signals:
    void currentRangeChanged(int min, int max);
    void valueChanged(int value);

protected:
    void calculateInternalToExternalConversion();
    void resizeEvent(QResizeEvent *event);

protected slots:
    void updateSplitterPosition(int pos, int idx);

private:
    int internalRange_[2];
    int internalValue_[2];

    int range_[2];
    int value_[2];

    float fromInternalToExternal_;
};

}//namespace

#endif // IVW_RANGESLIDERQT_H