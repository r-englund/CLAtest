#include <QMatrix>
#include <QMouseEvent>
#include <QWheelEvent>

#include <inviwo/qt/editor/networkeditorview.h>

namespace inviwo {

NetworkEditorView::NetworkEditorView(QWidget* parent) : QGraphicsView(parent),
                                                        zoomLevel_(1)
{
    setNetworkEditor(NetworkEditor::getPtr());
    setRenderHint(QPainter::Antialiasing, true);
    setMouseTracking(true);
    setDragMode(QGraphicsView::RubberBandDrag);
    setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
}

NetworkEditorView::~NetworkEditorView() {
    QGraphicsView::setScene(0);
}


void NetworkEditorView::setNetworkEditor(NetworkEditor* networkEditor) {
    networkEditor_ = networkEditor;
    QGraphicsView::setScene(networkEditor);
}

NetworkEditor* NetworkEditorView::getNetworkEditor() const {
    return networkEditor_;
}

void NetworkEditorView::setZoomLevel(int zoomLevel) {
    if (zoomLevel < -10) zoomLevel = -10;
    else if (zoomLevel > 10) zoomLevel = 10;

    zoomLevel_ = zoomLevel;
    zoomValue_ = calculateScaleFor(zoomLevel);

    QMatrix matrix;
    matrix.scale(zoomValue_, zoomValue_);
    setMatrix(matrix);
}

void NetworkEditorView::mouseDoubleClickEvent(QMouseEvent* e) {
    QGraphicsView::mouseDoubleClickEvent(e);

    if (!e->isAccepted()) {
        fitInView(networkEditor_->itemsBoundingRect(), Qt::KeepAspectRatio);

        float scale = matrix().m11();
        zoomLevel_ = calculateZoomLevelFor(scale);
        e->accept();
    }
}

void NetworkEditorView::resizeEvent(QResizeEvent* e) {
    QGraphicsView::resizeEvent(e);
    setZoomLevel(zoomLevel_);
    e->accept();
}

void NetworkEditorView::wheelEvent(QWheelEvent* e) {
    int newZoomLevel = zoomLevel_;
    if (e->delta() > 0) newZoomLevel++;
    else newZoomLevel--;
    setZoomLevel(newZoomLevel);
    e->accept();
}

float NetworkEditorView::calculateScaleFor(int zoomLevel) const {
    float editorWidth = networkEditor_->width();
    float editorHeight = networkEditor_->height();

    float canvasSize = editorWidth > editorHeight ? editorWidth : editorHeight;
    float viewSize = editorWidth > editorHeight ?
                     float(viewport()->width()) :
                     float(viewport()->height());

    // at zoom level -10 canvasSize must become viewSize
    float zoomOutFactor = (viewSize*0.95f / canvasSize);

    // at zoom level +10 canvasSize must be 3 times its size
    float zoomInFactor = 3.0f / 10.0f;

    // calculate the zoom
    float zoom = 1.0f;
    if (zoomLevel < 0)
        zoom = 1.0f + float(zoomLevel) * (1.0f - zoomOutFactor) / 10.0f;
    else
        zoom = 1.0f + zoomInFactor * float(zoomLevel);

    return zoom;
}

int NetworkEditorView::calculateZoomLevelFor(float scale) const {
    float editorWidth = networkEditor_->width();
    float editorHeight = networkEditor_->height();

    float canvasSize = editorWidth > editorHeight ? editorWidth : editorHeight;
    float viewSize = editorWidth > editorHeight ?
                     float(viewport()->width()) :
                     float(viewport()->height());

    // at zoom level -10 canvasSize must become viewSize
    float zoomOutFactor = (viewSize*0.95f / canvasSize);

    // at zoom level +10 canvasSize must be 3 times its size
    float zoomInFactor = 3.0f / 10.0f;

    float zoomLevel = 1.0f;
    if (scale < 1)
        zoomLevel = (scale - 1.0f) * 10.0f / (1.0f - zoomOutFactor);
    else
        zoomLevel = (scale - 1.0f) / zoomInFactor;

    return int(zoomLevel);
}

} // namespace