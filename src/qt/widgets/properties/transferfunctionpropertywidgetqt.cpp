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

TransferFunctionPropertyWidgetQt::~TransferFunctionPropertyWidgetQt(){
    delete transferFunctionDialog_;
    delete btnOpenTF_;
}


void TransferFunctionPropertyWidgetQt::generateWidget(){
    InviwoApplicationQt* app = dynamic_cast<InviwoApplicationQt*>(InviwoApplication::getPtr());
    transferFunctionDialog_ = new TransferFunctionPropertyDialog(property_, app->getMainWindow());
    transferFunctionDialog_->setFloating(true);
    transferFunctionDialog_->setVisible(false);
    app->getMainWindow()->addDockWidget(Qt::RightDockWidgetArea, transferFunctionDialog_);

    QHBoxLayout* hLayout = new QHBoxLayout();
    btnOpenTF_ = new QPushButton();
    btnOpenTF_->setFixedSize(200, 40);
    if (property_->getReadOnly()) {
        hLayout->addWidget(new QLabel(QString::fromStdString(property_->getDisplayName())));
        btnOpenTF_->setDisabled(true);
    } else {
        label_ = new EditableLabelQt(property_->getDisplayName());
        hLayout->addWidget(label_);
        connect(btnOpenTF_, SIGNAL(clicked()), this, SLOT(openTransferFunctionDialog()));
        connect(label_, SIGNAL(textChanged()), this, SLOT(setPropertyDisplayName()));
    }
    hLayout->addWidget(btnOpenTF_);
    setLayout(hLayout);
    updateFromProperty();
}

void TransferFunctionPropertyWidgetQt::updateFromProperty() {
    btnOpenTF_->setIcon((*transferFunctionDialog_->getTFPreview()).scaled(QSize(180, 25)));
    btnOpenTF_->setIconSize(QSize(180, 25));
}


void TransferFunctionPropertyWidgetQt::setPropertyValue() {}

void TransferFunctionPropertyWidgetQt::openTransferFunctionDialog() {
    transferFunctionDialog_->setVisible(true);
}

void TransferFunctionPropertyWidgetQt::setPropertyDisplayName(){
    property_->setDisplayName(label_->getText());
}

}//namespace