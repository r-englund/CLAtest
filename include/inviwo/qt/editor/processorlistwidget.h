 /*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2013 Inviwo Foundation
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
 * Main file authors: Erik Sundén, Timo Ropinski
 *
 *********************************************************************************/

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