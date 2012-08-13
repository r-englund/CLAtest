#include "inviwo/qt/editor/networkeditorview.h"

#include <QCursor>
#include <QMatrix>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QApplication>

namespace inviwo {

NetworkEditorView::NetworkEditorView(QWidget* parent) : QGraphicsView(parent),
                                                        zoomLevel_(3)
{
    setNetworkEditor(NetworkEditor::instance());
    setRenderHint(QPainter::Antialiasing, true);
    //zoomValue_ = calculateScaleFor(zoomLevel_);
}

NetworkEditorView::~NetworkEditorView() {}


void NetworkEditorView::setNetworkEditor(NetworkEditor* networkEditor) {
    networkEditor_ = networkEditor;
    QGraphicsView::setScene(networkEditor);
}

NetworkEditor* NetworkEditorView::getNetworkEditor() const {
    return networkEditor_;
}

int NetworkEditorView::zoomLevel() const {
    return zoomLevel_;
}

double NetworkEditorView::zoomScale() const {
    return zoomValue_;
}

void NetworkEditorView::setZoomLevel(int zoomLevel) {
    if (zoomLevel_ == zoomLevel)
        return;

    if (zoomLevel < -10) zoomLevel = -10;
    else if (zoomLevel > 10) zoomLevel = 10;

    zoomLevel_ = zoomLevel;
    zoomValue_ = calculateScaleFor(zoomLevel);

    QMatrix matrix;
    matrix.scale(zoomValue_, zoomValue_);
    setMatrix(matrix);

    emit zoomLevelChanged(zoomLevel_);
    emit zoomScaleChanged(zoomValue_);
}

void NetworkEditorView::zoomIn() {
    setZoomLevel(zoomLevel_++);
}

void NetworkEditorView::zoomOut() {
    setZoomLevel(zoomLevel_--);
}

void NetworkEditorView::zoomFull() {
    setZoomLevel(10);
}

void NetworkEditorView::zoomOne() {
    setZoomLevel(0);
}

void NetworkEditorView::zoomFit() {
    QRectF bBox = networkEditor_->itemsBoundingRect();
    fitInView(bBox, Qt::KeepAspectRatio);

    float scale = matrix().m11();
    zoomLevel_ = calculateZoomLevelFor(scale);

    emit zoomLevelChanged(zoomLevel_);
    emit zoomScaleChanged(scale);
}

void NetworkEditorView::mousePressEvent(QMouseEvent* e) {
    QGraphicsView::mousePressEvent(e);
    zoomFit();
}

void NetworkEditorView::mouseMoveEvent(QMouseEvent* e) {
    QGraphicsView::mouseMoveEvent(e);
}

void NetworkEditorView::mouseReleaseEvent(QMouseEvent* e) {
    QGraphicsView::mouseReleaseEvent(e);
}

void NetworkEditorView::resizeEvent(QResizeEvent* e) {
    QGraphicsView::resizeEvent(e);
    if (zoomLevel_ != 0) {
        int oldZoomLevel = zoomLevel_;
        zoomLevel_ = 0;
        setZoomLevel(oldZoomLevel);
    }
}

void NetworkEditorView::wheelEvent(QWheelEvent* e) {
    int newZoomLevel = zoomLevel_;
    if (e->delta() > 0) newZoomLevel++;
    else newZoomLevel--;
    setZoomLevel(newZoomLevel);
    e->accept();
}

float NetworkEditorView::calculateScaleFor(int zoomLevel) const {
    if (!networkEditor_)
        return 1.0f;

    float canvasWidth = networkEditor_->width();
    float canvasHeight = networkEditor_->height();

    float canvasSize = canvasWidth > canvasHeight ? canvasWidth : canvasHeight;
    float viewSize = canvasWidth > canvasHeight ?
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
    if (!networkEditor_)
        return 0;

    float canvasWidth = networkEditor_->width();
    float canvasHeight = networkEditor_->height();

    float canvasSize = canvasWidth > canvasHeight ? canvasWidth : canvasHeight;
    float viewSize = canvasWidth > canvasHeight ?
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