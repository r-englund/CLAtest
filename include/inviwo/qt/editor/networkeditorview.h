/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_NETWORKEDITORVIEW_H
#define IVW_NETWORKEDITORVIEW_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsView>

#include "networkeditor.h"

namespace inviwo {

class IVW_QTEDITOR_API NetworkEditorView : public QGraphicsView {

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