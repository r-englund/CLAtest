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

#ifndef EDITABLELABELQT_H
#define EDITABLELABELQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QMenu>
namespace inviwo {

class IVW_QTWIDGETS_API EditableLabelQt: public QWidget{
    Q_OBJECT
public:
    EditableLabelQt(QWidget *parent , std::string text, bool shortenText=true);
    EditableLabelQt(QWidget *parent , std::string text, QMenu* contextMenu, bool shortenText=true);
    void edit();
    std::string getText(){return text_;};
    void setText(std::string txt);
    void setContextMenu(QMenu* menu){contextMenu_ = menu;};
    void setShortenText(bool shorten);
public slots:
    void finishEditing();
    void showContextMenu(const QPoint& pos);

private:
    QLabel* label_;
    QLineEdit* lineEdit_;
    std::string text_;
    void generateWidget();
    QMenu* contextMenu_;
    void mouseDoubleClickEvent( QMouseEvent * event );
    std::string shortenText();
    bool shortenText_;

    signals:
        void textChanged();
};
}//namespace

#endif //EDITABLELABELQT_H