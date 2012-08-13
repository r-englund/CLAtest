#ifndef IVW_NETWORKEDITORVIEW_H
#define IVW_NETWORKEDITORVIEW_H

#include <QGraphicsView>

#include "networkeditor.h"

namespace inviwo {

class NetworkEditorView : public QGraphicsView {

public:
    NetworkEditorView(QWidget* parent=0);
    ~NetworkEditorView();

    void setNetworkEditor(NetworkEditor* networkEditor);
    NetworkEditor* getNetworkEditor() const;

protected:
    void mouseDoubleClickEvent(QMouseEvent* e);
    void resizeEvent(QResizeEvent* re);
    void wheelEvent(QWheelEvent * e);

private:
    NetworkEditor* networkEditor_;

    int zoomLevel_;
    float zoomValue_;

    void setZoomLevel(int zoomLevel);
    float calculateScaleFor(int zoomLevel) const;
    int calculateZoomLevelFor(float scale) const;
};

} // namespace

#endif // IVW_NETWORKEDITORVIEW_H