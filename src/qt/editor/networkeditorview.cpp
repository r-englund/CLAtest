#include "inviwo/qt/editor/networkeditorview.h"

#include <QCursor>
#include <QMatrix>
#include <QPainter>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QApplication>

namespace inviwo {

    NetworkEditorView::NetworkEditorView(QWidget* parent) : QGraphicsView(parent) {
        setDataFlowEditor(NetworkEditor::instance());
        setRenderHint(QPainter::Antialiasing, true);
    }

    NetworkEditorView::~NetworkEditorView() {}


    void NetworkEditorView::setDataFlowEditor(NetworkEditor* dataFlowEditor) {
        dataFlowEditor_ = dataFlowEditor;
        QGraphicsView::setScene(dataFlowEditor);
    }

    NetworkEditor* NetworkEditorView::getDataFlowEditor() const {
        return dataFlowEditor_;
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

        QApplication::setOverrideCursor(Qt::WaitCursor);
        QMatrix matrix;
        matrix.scale(zoomValue_, zoomValue_);
        setMatrix(matrix);
        QApplication::restoreOverrideCursor();

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
        QRectF bBox = dataFlowEditor_->itemsBoundingRect();
        fitInView(bBox, Qt::KeepAspectRatio);

        float scale = matrix().m11();
        zoomLevel_ = calculateZoomLevelFor(scale);

        emit zoomLevelChanged(zoomLevel_);
        emit zoomScaleChanged(scale);
    }

    void NetworkEditorView::mousePressEvent(QMouseEvent* e) {
        QGraphicsView::mousePressEvent(e);
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
        if (e->delta() > 0) zoomIn();
        else zoomOut();
        e->accept();
    }

    float NetworkEditorView::calculateScaleFor(int zoomLevel) const {
        if (!dataFlowEditor_)
            return 1.0f;

        float canvasWidth = dataFlowEditor_->width();
        float canvasHeight = dataFlowEditor_->height();

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
        if (!dataFlowEditor_)
            return 0;

        float canvasWidth = dataFlowEditor_->width();
        float canvasHeight = dataFlowEditor_->height();

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