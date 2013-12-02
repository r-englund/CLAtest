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

#include <inviwo/qt/widgets/processors/processorwidgetqt.h>

#include <QApplication>
#include <QDesktopWidget>
#include <QResizeEvent>
#include <QMoveEvent>
#include <inviwo/core/common/inviwo.h>

namespace inviwo {

ProcessorWidgetQt::ProcessorWidgetQt()
    : QWidget(NULL), ProcessorWidget()
{
    QWidget::move(0, 0);
    QWidget::resize(32, 32);
    QWidget::setVisible(false);
}

ProcessorWidgetQt::~ProcessorWidgetQt() {}

void ProcessorWidgetQt::initialize() {
    ProcessorWidget::initialize();
    ivec2 pos = ProcessorWidget::getPositionMetaData();

	// check if geometry is on screen and alter otherwise
	QDesktopWidget* desktop = QApplication::desktop();
	QRect wholeScreenGeometry = desktop->screenGeometry(0);
	for (int i=1; i<desktop->screenCount(); i++)
		wholeScreenGeometry = wholeScreenGeometry.united(desktop->screenGeometry(i));
	wholeScreenGeometry.setRect(wholeScreenGeometry.x()-10, wholeScreenGeometry.y()-10,
		wholeScreenGeometry.width()+20, wholeScreenGeometry.height()+20);

	QPoint bottomRight = QPoint(pos.x+this->width(), pos.y+this->height());
	if (!wholeScreenGeometry.contains(QPoint(pos.x, pos.y)) || !wholeScreenGeometry.contains(bottomRight))
		QWidget::move(QPoint(0,0));
	else
		QWidget::move(pos.x, pos.y);
}

void ProcessorWidgetQt::setVisible(bool visible) {
    ProcessorWidget::setVisible(visible);
    QWidget::setVisible(visible);
}

void ProcessorWidgetQt::show() {
    ProcessorWidget::show();
    QWidget::show();
}

void ProcessorWidgetQt::hide() {
    ProcessorWidget::hide();
    QWidget::hide();
}

void ProcessorWidgetQt::move(ivec2 pos) {
    ProcessorWidget::move(pos);
    QWidget::move(pos.x, pos.y);
}

void ProcessorWidgetQt::setDimensions(ivec2 dimensions) {
	ProcessorWidget::setDimensions(dimensions);
	QWidget::resize(dimensions.x, dimensions.y);
}

void ProcessorWidgetQt::resizeEvent(QResizeEvent* event) {
    ProcessorWidget::setDimensions(ivec2(event->size().width(), event->size().height()) );
    QWidget::resizeEvent(event);
}

void ProcessorWidgetQt::showEvent(QShowEvent* event) {
    ProcessorWidget::show();
}

void ProcessorWidgetQt::closeEvent(QCloseEvent* event) {
    ProcessorWidget::hide();
}

void ProcessorWidgetQt::moveEvent(QMoveEvent* event) {
    ProcessorWidget::move(ivec2(event->pos().x(), event->pos().y()) );
    QWidget::moveEvent(event);
}

} // namespace
