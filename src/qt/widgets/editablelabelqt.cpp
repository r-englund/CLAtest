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

#include <inviwo/qt/widgets/editablelabelqt.h>

namespace inviwo{

EditableLabelQt::EditableLabelQt(QWidget *parent , std::string text, bool shortenText)
    : QWidget(parent)
    , text_(text)
    , contextMenu_(NULL)
    , shortenText_(shortenText) {
    generateWidget();
}

EditableLabelQt::EditableLabelQt(QWidget *parent , std::string text, QMenu* contextMenu, bool shortenText)
    : QWidget(parent)
    , text_(text)
    , contextMenu_(contextMenu)
    , shortenText_(shortenText) {
    generateWidget();
}

void EditableLabelQt::generateWidget(){
    QHBoxLayout* hLayout = new QHBoxLayout();
    label_ = new QLabel(this);
    if (shortenText_)
        label_->setText(QString::fromStdString(shortenText()));
    else
        label_->setText(QString::fromStdString(text_));

    lineEdit_ = new QLineEdit(this);
    hLayout->addWidget(lineEdit_);
    lineEdit_->hide();
    lineEdit_->setAlignment(Qt::AlignLeft);
    hLayout->addWidget(label_);
    setLayout(hLayout);

    if (contextMenu_ == NULL)
        contextMenu_ = new QMenu(this);
    contextMenu_->addAction("Rename");

    label_->setContextMenuPolicy(Qt::CustomContextMenu);

    connect(label_,SIGNAL(customContextMenuRequested(const QPoint&)),this,SLOT(showContextMenu(const QPoint&)));
    connect(lineEdit_, SIGNAL(editingFinished()),this, SLOT(finishEditing()));
}

void EditableLabelQt::edit(){
    label_->hide();
    lineEdit_->setText(QString::fromStdString(text_));
    lineEdit_->show();
    lineEdit_->setContentsMargins(0,0,0,0);
    lineEdit_->setFocus();
    lineEdit_->selectAll();
}

void EditableLabelQt::mouseDoubleClickEvent( QMouseEvent* e ){
    edit();
}

void EditableLabelQt::finishEditing() {
    lineEdit_->hide();
    text_ = lineEdit_->text().toLocal8Bit().constData();
    if (shortenText_)
        label_->setText(QString::fromStdString(shortenText()));    
    else
        label_->setText(QString::fromStdString(text_));
    label_->show();
    emit textChanged();
}

void EditableLabelQt::setText(std::string txt) {
    text_ = txt; 
    edit(); 
    finishEditing();
}

void EditableLabelQt::showContextMenu( const QPoint& pos ){
    QPoint globalPos = label_->mapToGlobal(pos);
    QAction* selecteditem = contextMenu_->exec(globalPos);
    if (selecteditem == contextMenu_->actions().at(0)) {
        edit();
    }
}

std::string EditableLabelQt::shortenText(){
    if (text_.length()>25) {
        std::string shortText = text_.substr(0,15)+"..."+text_.substr(text_.length()-7,7);
        label_->setToolTip(QString::fromStdString(text_));
        return shortText;
    }else{
        label_->setToolTip(QString::fromStdString(""));
        return text_;
    }
    
}

void EditableLabelQt::setShortenText(bool shorten) {
    shortenText_ = shorten;
}

} //namespace