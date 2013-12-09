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
 * Primary author : Erik Sundén
 *
 **********************************************************************/

#include <QFocusEvent>
#include <QFont>
#include <QPainter>
#include <QTextCursor>

#include <inviwo/qt/widgets/labelgraphicsitem.h>

namespace inviwo {

LabelGraphicsItem::LabelGraphicsItem(QGraphicsItem* parent)
    : QGraphicsTextItem(parent)
    , maxBefore_(0)
    , maxAfter_(0)
    , focusOut_(false)
    , orgText_(""){
}

LabelGraphicsItem::~LabelGraphicsItem() {}

QString LabelGraphicsItem::text() const{
    if(isCropped())
        return orgText_;
    else
        return toPlainText();
}

void LabelGraphicsItem::setText(const QString &str){
    if(doCrop(str)){
        if(toolTip()==orgText_)
            setToolTip(str);
        orgText_ = str;
        setPlainText(str.left(maxBefore_) + "..." + str.right(maxAfter_));
    }
    else{
        orgText_="";
        setToolTip(orgText_);
        setPlainText(str);
    }
}

QString LabelGraphicsItem::croppedText() const{
    return toPlainText();
}

void LabelGraphicsItem::setCrop(int before, int after){
    maxBefore_ = before;
    maxAfter_ = after;
    updateCrop();
}

bool LabelGraphicsItem::isCropped() const{
    return (!orgText_.isEmpty());
}

void LabelGraphicsItem::setNoFocusOut(){
    focusOut_ = false;
}

bool LabelGraphicsItem::isFocusOut() const{
    return focusOut_;
}

bool LabelGraphicsItem::doCrop(const QString &str){
    return (maxBefore_ + maxAfter_ + 2 < str.length());
}

void LabelGraphicsItem::updateCrop(){
    setText(toPlainText());
}

void LabelGraphicsItem::keyPressEvent(QKeyEvent *event){
    if (event->key() == Qt::Key_Return)
        clearFocus();
    else if (event->key() != Qt::Key_Space )
        QGraphicsTextItem::keyPressEvent(event);
}

void LabelGraphicsItem::focusInEvent(QFocusEvent* event) {
    if(isCropped())
        setPlainText(orgText_);
}

void LabelGraphicsItem::focusOutEvent(QFocusEvent* event) {
    focusOut_ = true;
    setFlags(0);
    setTextInteractionFlags(Qt::NoTextInteraction);
    QTextCursor cur = QTextCursor(textCursor());
    cur.clearSelection();
    setTextCursor(cur);
    updateCrop();
    notifyObservers();
    focusOut_ = false;
}

} // namespace
