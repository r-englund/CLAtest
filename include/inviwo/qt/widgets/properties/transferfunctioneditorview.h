#ifndef IVW_TRANSFERFUNCTIONEDITORVIEW_H
#define IVW_TRANSFERFUNCTIONEDITORVIEW_H

//Inviwo includes
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditor.h>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>

//Qt includes
#include <QtEvents>
#include <QGraphicsScene>
#include <QGraphicsView>

namespace inviwo {
class IVW_QTWIDGETS_API TransferFunctionEditorView : public QGraphicsView {

	Q_OBJECT

public:    
    void setMaskMin( const int maskMin );
    void setMaskMax( const int maskMax );

signals:
	void resized();

protected:
	void resizeEvent ( QResizeEvent * event );
	void drawForeground(QPainter *painter, const QRectF &rect);

private:
    float viewMaskMin_;
    float viewMaskMax_;
};
} // namespace

#endif // IVW_TRANSFERFUNCTIONEDITORVIEW_H