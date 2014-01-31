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
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>

namespace inviwo {

TransferFunctionPropertyWidgetQt::TransferFunctionPropertyWidgetQt(TransferFunctionProperty* property) : property_(property){
    PropertyWidgetQt::setProperty(property_);
    generateWidget();
    updateFromProperty();
    PropertyWidgetQt::generateContextMenu();
}

TransferFunctionPropertyWidgetQt::~TransferFunctionPropertyWidgetQt() {
    delete transferFunctionDialog_;
    delete btnOpenTF_;
}

void TransferFunctionPropertyWidgetQt::generateWidget(){
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
    ivec2 widgetDimension = getEditorWidget()->getEditorDimensionMetaData();
    transferFunctionDialog_->resize(QSize(widgetDimension.x, widgetDimension.y));

    ivec2 widgetPosition = getEditorWidget()->getEditorPositionMetaData();
    transferFunctionDialog_->move(widgetPosition.x, widgetPosition.y);
    
    bool visible = getEditorWidget()->getEditorVisibilityMetaData();
    if (!visible ) transferFunctionDialog_->hide();
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
        tfPainter.drawLine(static_cast<int>(property_->getMask().x*gradientWidth), 0, static_cast<int>(property_->getMask().x*gradientWidth), btnOpenTF_->height());
    }
    if (property_->getMask().y < 1.0f) {
        tfPainter.fillRect(static_cast<int>(property_->getMask().y*gradientWidth), 0, 
            static_cast<int>(gradientWidth-(property_->getMask().y*gradientWidth)+10), btnOpenTF_->height(), QColor(25,25,25,150));
        tfPainter.drawLine(static_cast<int>(property_->getMask().y*gradientWidth), 0, static_cast<int>(property_->getMask().y*gradientWidth), btnOpenTF_->height());
    }
    btnOpenTF_->setIcon(tfPixmap);
    btnOpenTF_->setIconSize(btnOpenTF_->size());
}

void TransferFunctionPropertyWidgetQt::setPropertyValue() {}

void TransferFunctionPropertyWidgetQt::openTransferFunctionDialog() {
    transferFunctionDialog_->setVisible(true);
}

void TransferFunctionPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}

}//namespace