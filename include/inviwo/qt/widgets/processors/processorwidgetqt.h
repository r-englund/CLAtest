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

#ifndef IVW_PROCESSORWIDGETQT_H
#define IVW_PROCESSORWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QWidget>
#include <inviwo/core/processors/processorwidget.h>

namespace inviwo {

class IVW_QTWIDGETS_API ProcessorWidgetQt : public QWidget, public ProcessorWidget {
    Q_OBJECT

public:
    ProcessorWidgetQt();
    virtual ~ProcessorWidgetQt();

    virtual ProcessorWidget* create() const = 0;
    virtual void initialize() = 0;    
    virtual void setVisible(bool visible);
    virtual void show();
    virtual void hide();
    virtual void move(ivec2 pos);
	virtual void setDimensions(ivec2 dimensions);

protected:
    virtual void resizeEvent(QResizeEvent*);
    virtual void closeEvent(QCloseEvent*);
    virtual void showEvent(QShowEvent*);
    virtual void moveEvent(QMoveEvent*);
};

} // namespace

#endif // IVW_PROCESSORWIDGETQT_H
