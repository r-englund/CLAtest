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

#ifndef IVW_PROCESSORLISTWIDGET_H
#define IVW_PROCESSORLISTWIDGET_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QComboBox>
#include <QLineEdit>
#include <QListWidget>
#include <QMouseEvent>
#include <QTreeWidget>
#include <QDrag>

#include <inviwo/qt/widgets/inviwodockwidget.h>

#include <inviwo/core/processors/processorfactoryobject.h>

namespace inviwo {

class IVW_QTEDITOR_API ProcessorList : public QListWidget {

public:
    ProcessorList(QWidget* parent) : QListWidget(parent) {};
    ~ProcessorList() {};

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

private:
    QPoint dragStartPosition_;
};

class IVW_QTEDITOR_API ProcessorListWidget : public InviwoDockWidget {
    Q_OBJECT
public:
    ProcessorListWidget(QWidget* parent);
    ~ProcessorListWidget();

private:
    ProcessorList* processorList_;
    QPoint dragStartPosition_;

    bool processorFits(ProcessorFactoryObject* processor, const QString& filter);

private slots:
    void addProcessorsToList(const QString& text="");
};

class IVW_QTEDITOR_API ProcessorTree : public QTreeWidget {

public:
    ProcessorTree(QWidget* parent) : QTreeWidget(parent) {};
    ~ProcessorTree() {};

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);

private:
    QPoint dragStartPosition_;
};

class IVW_QTEDITOR_API ProcessorTreeWidget : public InviwoDockWidget {
    Q_OBJECT
public:
    ProcessorTreeWidget(QWidget* parent);
    ~ProcessorTreeWidget();

protected:
    bool processorFits(ProcessorFactoryObject* processor, const QString& filter);
    QIcon* getCodeStateIcon(CodeState);

private slots:
    void addProcessorsToTree();

private:
    ProcessorTree* processorTree_;
    QComboBox* listView_;
    QLineEdit* lineEdit_;

    QPoint dragStartPosition_;

    QIcon iconStable_;
    QIcon iconExperimental_;
    QIcon iconBroken_;
};

class IVW_QTEDITOR_API ProcessorDragObject : public QDrag {
public:
    ProcessorDragObject(QWidget* source, const QString className);

    static bool canDecode(const QMimeData* mimeData);
    static bool decode(const QMimeData* mimeData, QString& className);
};

} // namespace

#endif // IVW_PROCESSORLISTWIDGET_H