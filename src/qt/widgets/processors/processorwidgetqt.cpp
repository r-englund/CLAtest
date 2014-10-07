/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file authors: Timo Ropinski, Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/processors/processorwidgetqt.h>
#include <apps/inviwo/inviwomainwindow.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QMoveEvent>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>

namespace inviwo {

ProcessorWidgetQt::ProcessorWidgetQt() : QWidget(NULL), ProcessorWidget() {}

ProcessorWidgetQt::ProcessorWidgetQt(const ProcessorWidgetQt& rhs) 
    : QWidget(NULL)
    , ProcessorWidget(rhs) {}

ProcessorWidgetQt& ProcessorWidgetQt::operator=(const ProcessorWidgetQt& that) {
    if (this != &that) {
        ProcessorWidget::operator=(that);
    }
    return *this;
}

ProcessorWidgetQt::~ProcessorWidgetQt() {}

void ProcessorWidgetQt::initialize() {
    ProcessorWidget::initialize();
    ivec2 dim = ProcessorWidget::getDimension();
    QWidget::resize(dim.x, dim.y);

    // Check if geometry is on screen and alter otherwise
    // TODO: Desktop screen info should be added to system capabilities
    ivec2 pos = ProcessorWidget::getPosition();
    QDesktopWidget* desktop = QApplication::desktop();
    int primaryScreenIndex = desktop->primaryScreen();
    QRect wholeScreenGeometry = desktop->screenGeometry(primaryScreenIndex);

    for (int i = 0; i < desktop->screenCount(); i++) {
        if (i != primaryScreenIndex)
            wholeScreenGeometry = wholeScreenGeometry.united(desktop->screenGeometry(i));
    }

    wholeScreenGeometry.setRect(wholeScreenGeometry.x() - 10, wholeScreenGeometry.y() - 10,
                                wholeScreenGeometry.width() + 20,
                                wholeScreenGeometry.height() + 20);
    QPoint bottomRight = QPoint(pos.x + this->width(), pos.y + this->height());
    InviwoApplicationQt* app = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());
    if (app) {
        QPoint appPos = app->getMainWindow()->pos();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
        QPoint offset = app->getWindowDecorationOffset();
        pos -= vec2(offset.x(), offset.y());
#endif

        if (!wholeScreenGeometry.contains(QPoint(pos.x, pos.y)) ||
            !wholeScreenGeometry.contains(bottomRight)) {// If the widget is outside visible screen
            pos = ivec2(appPos.x(), appPos.y());
            pos += offsetWidget();
            QWidget::move(pos.x, pos.y);
        } else {
            if (!(pos.x == 0 && pos.y == 0))
                // TODO: Detect if processor position is set. Need to figure out better way.
                QWidget::move(pos.x, pos.y);
            else {
                pos = ivec2(appPos.x(), appPos.y());
                pos += offsetWidget();
                QWidget::move(pos.x, pos.y);
            }
        }
    }
}

ivec2 ProcessorWidgetQt::offsetWidget(){
    static int offsetCounter = 0;
    static ivec2 baseOffset(350,100);

    ivec2 pos(0,0);
    pos += baseOffset + ivec2(40*offsetCounter++);

    if(offsetCounter==10){ //reset offset
        offsetCounter = 0;
        baseOffset.x += 200; 
        if(baseOffset.x >= 800){
            baseOffset.x = 350;
            baseOffset.y += 100;
            if(baseOffset.y>=800){
                baseOffset.y = 100; 
            }
        }
    }
    return pos;
}

void ProcessorWidgetQt::deinitialize() {
    ProcessorWidget::deinitialize();
}

void ProcessorWidgetQt::setVisible(bool visible) {
    // The subsequent events will call ProcessorWidget.
    QWidget::setVisible(visible);
}

void ProcessorWidgetQt::show() {
    ProcessorWidgetQt::setVisible(true);
}

void ProcessorWidgetQt::hide() {
    ProcessorWidgetQt::setVisible(false);
}

void ProcessorWidgetQt::setPosition(glm::ivec2 pos) {
    //ProcessorWidget::setPosition(pos); Will be called by the Move event.
    QWidget::move(pos.x, pos.y);
}
    
void ProcessorWidgetQt::move(ivec2 pos) {
    ProcessorWidgetQt::setPosition(pos);
}

void ProcessorWidgetQt::setDimension(ivec2 dimensions) {
    // ProcessorWidget::setDimension(dimensions);  Will be called by the Resize event.
    QWidget::resize(dimensions.x, dimensions.y);
}

void ProcessorWidgetQt::resizeEvent(QResizeEvent* event) {
    ProcessorWidget::setDimension(ivec2(event->size().width(), event->size().height()));
    QWidget::resizeEvent(event);
}

void ProcessorWidgetQt::showEvent(QShowEvent* event) {
    ProcessorWidget::setVisible(true);
    QWidget::showEvent(event);
}

void ProcessorWidgetQt::closeEvent(QCloseEvent* event) {
    ProcessorWidget::setVisible(false);
    QWidget::closeEvent(event);
}

void ProcessorWidgetQt::hideEvent(QHideEvent* event) {
    ProcessorWidget::setVisible(false);
    QWidget::hideEvent(event);
}
    
void ProcessorWidgetQt::moveEvent(QMoveEvent* event) {
    ProcessorWidget::setPosition(ivec2(event->pos().x(), event->pos().y()));
    QWidget::moveEvent(event);
}

} // namespace
