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
 * Main file authors: Timo Ropinski, Viktor Axelsson, Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>
#include <QDesktopWidget>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>

namespace inviwo {

TransferFunctionPropertyWidgetQt::TransferFunctionPropertyWidgetQt(TransferFunctionProperty* property) : property_(property), transferFunctionDialog_(NULL) {
    PropertyWidgetQt::setProperty(property_);
    generateWidget();
    updateFromProperty();
    PropertyWidgetQt::generateContextMenu();
}

TransferFunctionPropertyWidgetQt::~TransferFunctionPropertyWidgetQt() {
    transferFunctionDialog_->hide();
    delete transferFunctionDialog_;
    delete btnOpenTF_;
}

void TransferFunctionPropertyWidgetQt::generateWidget() {
    InviwoApplicationQt* app = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());
    transferFunctionDialog_ = new TransferFunctionPropertyDialog(property_, app->getMainWindow());
    setEditorWidget(transferFunctionDialog_);
    PropertyEditorWidgetDockStatus docStatus = getEditorWidget()->getEditorDockStatus();

    if (docStatus == PropertyEditorWidgetDockStatus::DockedLeft) {
        app->getMainWindow()->addDockWidget(Qt::LeftDockWidgetArea, transferFunctionDialog_);
        transferFunctionDialog_->setFloating(false);
    }
    else if (docStatus == PropertyEditorWidgetDockStatus::DockedRight) {
        app->getMainWindow()->addDockWidget(Qt::RightDockWidgetArea, transferFunctionDialog_);
        transferFunctionDialog_->setFloating(false);
    }
    else {
        app->getMainWindow()->addDockWidget(Qt::RightDockWidgetArea, transferFunctionDialog_);
        transferFunctionDialog_->setFloating(true);
    }

    // notify the transfer function dialog, that the volume with the histogram is already there
    property_->notifyObservers();
    QHBoxLayout* hLayout = new QHBoxLayout();
    btnOpenTF_ = new QPushButton();
    btnOpenTF_->setFixedSize(200, 40);

    if (property_->getReadOnly()) {
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        btnOpenTF_->setDisabled(true);
    } else {
        label_ = new EditableLabelQt(this,property_->getDisplayName());
        hLayout->addWidget(label_);
        connect(btnOpenTF_, SIGNAL(clicked()), this, SLOT(openTransferFunctionDialog()));
        connect(label_, SIGNAL(textChanged()), this, SLOT(setPropertyDisplayName()));
    }

    hLayout->addWidget(btnOpenTF_);
    setLayout(hLayout);
    updateFromProperty();
    //set widget meta data stuff
    transferFunctionDialog_->hide();    

    ivec2 widgetDimension = getEditorWidget()->getEditorDimensionMetaData();
    transferFunctionDialog_->resize(QSize(widgetDimension.x, widgetDimension.y));

    ivec2 pos = getEditorWidget()->getEditorPositionMetaData();
    
    //TODO: Desktop screen info should be added to system capabilities
    QDesktopWidget* desktop = QApplication::desktop();
    int primaryScreenIndex = desktop->primaryScreen();
    QRect wholeScreenGeometry = desktop->screenGeometry(primaryScreenIndex);
    QRect primaryScreenGeometry = desktop->screenGeometry(primaryScreenIndex);

    for (int i=0; i<desktop->screenCount(); i++) {
        if (i!=primaryScreenIndex)
            wholeScreenGeometry = wholeScreenGeometry.united(desktop->screenGeometry(i));        
    }

    wholeScreenGeometry.setRect(wholeScreenGeometry.x()-10, wholeScreenGeometry.y()-10,
        wholeScreenGeometry.width()+20, wholeScreenGeometry.height()+20);
    QPoint bottomRight = QPoint(pos.x+this->width(), pos.y+this->height());

    QPoint appPos = app->getMainWindow()->pos();

    if (!wholeScreenGeometry.contains(QPoint(pos.x, pos.y)) || !wholeScreenGeometry.contains(bottomRight)) {
        LogWarn("Widget position modified to fit into current screen")
        pos = ivec2(appPos.x(), appPos.y()) ;
        pos += ivec2( primaryScreenGeometry.width()/2, primaryScreenGeometry.height()/2);
        transferFunctionDialog_->move(pos.x, pos.y);
    }
    else {        
        if (!(pos.x == 0 && pos.y == 0))            
            transferFunctionDialog_->move(pos.x, pos.y);
        else {
            pos = ivec2(appPos.x(), appPos.y()) ;
            pos += ivec2( primaryScreenGeometry.width()/2, primaryScreenGeometry.height()/2);
            transferFunctionDialog_->move(pos.x, pos.y);
        }
    }
    
    bool visible = getEditorWidget()->getEditorVisibilityMetaData();
    if (!visible) transferFunctionDialog_->hide();
    else transferFunctionDialog_->show();
    
}

void TransferFunctionPropertyWidgetQt::updateFromProperty() {
    int gradientWidth = btnOpenTF_->width();
    QLinearGradient* gradient = transferFunctionDialog_->getTFGradient();
    gradient->setFinalStop(gradientWidth, 0);
    QPixmap tfPixmap(gradientWidth, btnOpenTF_->height());
    QPainter tfPainter(&tfPixmap);
    QPixmap checkerBoard(10, 10);
    QPainter checkerBoardPainter(&checkerBoard);
    checkerBoardPainter.fillRect(0, 0, 5, 5, Qt::lightGray);
    checkerBoardPainter.fillRect(5, 0, 5, 5, Qt::darkGray);
    checkerBoardPainter.fillRect(0, 5, 5, 5, Qt::darkGray);
    checkerBoardPainter.fillRect(5, 5, 5, 5, Qt::lightGray);
    checkerBoardPainter.end();
    tfPainter.fillRect(0, 0, gradientWidth, btnOpenTF_->height(), QBrush(checkerBoard));
    tfPainter.fillRect(0, 0, gradientWidth, btnOpenTF_->height(), *gradient);

    // draw masking indicators
    if (property_->getMask().x > 0.0f) {
        tfPainter.fillRect(0, 0, static_cast<int>(property_->getMask().x*gradientWidth), btnOpenTF_->height(), QColor(25,25,25,100));
        tfPainter.drawLine(static_cast<int>(property_->getMask().x*gradientWidth), 0, static_cast<int>(property_->getMask().x*gradientWidth),
                           btnOpenTF_->height());
    }

    if (property_->getMask().y < 1.0f) {
        tfPainter.fillRect(static_cast<int>(property_->getMask().y*gradientWidth), 0,
                           static_cast<int>(gradientWidth-(property_->getMask().y*gradientWidth)+10), btnOpenTF_->height(), QColor(25,25,25,150));
        tfPainter.drawLine(static_cast<int>(property_->getMask().y*gradientWidth), 0, static_cast<int>(property_->getMask().y*gradientWidth),
                           btnOpenTF_->height());
    }

    btnOpenTF_->setIcon(tfPixmap);
    btnOpenTF_->setIconSize(btnOpenTF_->size());
}

void TransferFunctionPropertyWidgetQt::setPropertyValue() {}

void TransferFunctionPropertyWidgetQt::openTransferFunctionDialog() {
    transferFunctionDialog_->setVisible(true);
}

void TransferFunctionPropertyWidgetQt::setPropertyDisplayName() {
    property_->setDisplayName(label_->getText());
}

}//namespace