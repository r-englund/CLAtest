#ifndef IVW_DATAFLOWEDITORVIEW_H
#define IVW_DATAFLOWEDITORVIEW_H

#include <QGraphicsView>

#include "networkeditor.h"

namespace inviwo {

class NetworkEditorView : public QGraphicsView {
    Q_OBJECT

public:
    NetworkEditorView(QWidget* parent=0);
    ~NetworkEditorView();

    void setNetworkEditor(NetworkEditor* networkEditor);
    NetworkEditor* getNetworkEditor() const;

    int zoomLevel() const;
    double zoomScale() const;

public slots:
    void setZoomLevel(int zoomLevel);
    void zoomIn();
    void zoomOut();
    void zoomFull();
    void zoomOne();
    void zoomFit();

signals:
    void zoomLevelChanged(int zl);
    void zoomScaleChanged(double zs);

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent* e);
    void resizeEvent(QResizeEvent* re);
    void wheelEvent(QWheelEvent * e);

private:
    NetworkEditor* networkEditor_;

    int zoomLevel_;
    float zoomValue_;

    float calculateScaleFor(int zoomLevel) const;
    int calculateZoomLevelFor(float scale) const;
};

} // namespace

#endif // IVW_DATAFLOWEDITORVIEW_H