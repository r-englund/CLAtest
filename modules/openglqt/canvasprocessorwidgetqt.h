/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

#ifndef IVW_CANVASPROCESSORWIDGETQT_H
#define IVW_CANVASPROCESSORWIDGETQT_H

#include <modules/openglqt/openglqtmoduledefine.h>
#include <inviwo/qt/widgets/processors/processorwidgetqt.h>

namespace inviwo {

class CanvasQt;
class CanvasProcessor;

class IVW_MODULE_OPENGLQT_API CanvasProcessorWidgetQt : public ProcessorWidgetQt {
    Q_OBJECT
public:
    CanvasProcessorWidgetQt();
    virtual ~CanvasProcessorWidgetQt();

    virtual ProcessorWidget* create() const;
    virtual void initialize();
    virtual void deinitialize();

    // Override ProcessorWidgetQt
    virtual void setVisible(bool visible);
    virtual void show();
    virtual void hide();

protected:
    // Override ProcessorWidgetQt
    virtual void resizeEvent(QResizeEvent*);
    virtual void showEvent(QShowEvent*);
    virtual void closeEvent(QCloseEvent*);

private:
    CanvasQt* canvas_;
    bool hasSharedCanvas_;
};

} // namespace

#endif // IVW_CANVASPROCESSORWIDGETQT_H
