/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_CANVASPROCESSORWIDGET_H
#define IVW_CANVASPROCESSORWIDGET_H

#include <modules/openglqt/openglqtmoduledefine.h>
#include <modules/openglqt/canvasqt.h>
#include <inviwo/qt/widgets/processors/processorwidgetqt.h>

namespace inviwo {

class IVW_MODULE_OPENGLQT_API CanvasProcessorWidgetQt : public ProcessorWidgetQt {
    Q_OBJECT
public:
    CanvasProcessorWidgetQt();
    virtual ~CanvasProcessorWidgetQt();

    virtual void initialize();
    virtual ProcessorWidget* create() const;
    virtual void show();
    virtual void hide();

protected:
    void resizeEvent(QResizeEvent*);
    void showEvent(QShowEvent*);
    void closeEvent(QCloseEvent*);

private:
    CanvasQt* canvas_;
};

} // namespace

#endif // IVW_CANVASPROCESSORWIDGET_H
