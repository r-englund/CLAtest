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

#include <QApplication>
#include <QLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWidget>
#include <QMimeData>

#include <inviwo/qt/editor/processorlistwidget.h>

#include <inviwo/core/util/msvc-memleak-includes.h>

#include <inviwo/core/common/inviwoapplication.h>


#include <inviwo/core/processors/processorstate.h>

namespace inviwo {

void ProcessorList::mousePressEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton)
        dragStartPosition_ = e->pos();
    QListWidget::mousePressEvent(e);
}

void ProcessorList::mouseMoveEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton) {
        if ((e->pos() - dragStartPosition_).manhattanLength() < QApplication::startDragDistance())
            return;

        QListWidgetItem* selectedProcessor = itemAt(dragStartPosition_);
        if (selectedProcessor)
            new ProcessorDragObject(this, selectedProcessor->text());
    }
}

ProcessorListWidget::ProcessorListWidget(QWidget* parent) : InviwoDockWidget(tr("Processors"), parent) {
    setObjectName("ProcessorListWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QFrame* frame = new QFrame();
    QVBoxLayout* vLayout = new QVBoxLayout(frame);
   
    QLineEdit* lineEdit = new QLineEdit();
    lineEdit->setPlaceholderText("Filter processor list...");
    connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(addProcessorsToList(const QString&)));
    vLayout->addWidget(lineEdit);

    processorList_ = new ProcessorList(this);
    addProcessorsToList();
    vLayout->addWidget(processorList_);

    frame->setLayout(vLayout);
    setWidget(frame);
}

ProcessorListWidget::~ProcessorListWidget() {}

bool ProcessorListWidget::processorFits(ProcessorFactoryObject* processor, const QString& filter) {
    return (QString::fromStdString(processor->getClassName()).contains(filter, Qt::CaseInsensitive));
}

void ProcessorListWidget::addProcessorsToList(const QString& text) {
    processorList_->clear();
    // add processors from all modules to the list
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
        std::vector<ProcessorFactoryObject*> curProcessorList = inviwoApp->getModules()[curModuleId]->getProcessors();
        for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++) {
            if (text.isEmpty() || processorFits(curProcessorList[curProcessorId], text)) {
                QString iconName = ":/icons/processor_experimental.png";
                if (curProcessorList[curProcessorId]->getCodeState() == CODE_STATE_BROKEN)
                    iconName = ":/icons/processor_broken.png";
                else if (curProcessorList[curProcessorId]->getCodeState() == CODE_STATE_STABLE)
                    iconName = ":/icons/processor_stable.png";
                processorList_->addItem(new QListWidgetItem(QIcon(iconName),
                                        QString::fromStdString(curProcessorList[curProcessorId]->getClassName())));
            }
        }
    }
    processorList_->sortItems(Qt::AscendingOrder);
}

void ProcessorTree::mousePressEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton)
        dragStartPosition_ = e->pos();
    QTreeWidget::mousePressEvent(e);
}

void ProcessorTree::mouseMoveEvent(QMouseEvent* e) {
    if (e->buttons() & Qt::LeftButton) {
        if ((e->pos() - dragStartPosition_).manhattanLength() < QApplication::startDragDistance())
            return;

        QTreeWidgetItem* selectedProcessor = itemAt(dragStartPosition_);
        if (selectedProcessor && selectedProcessor->parent())
            new ProcessorDragObject(this, selectedProcessor->text(0));
    }
}

ProcessorTreeWidget::ProcessorTreeWidget(QWidget* parent) : InviwoDockWidget(tr("Processors"), parent) {
    setObjectName("ProcessorTreeWidget");
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    QFrame* frame = new QFrame();
    QVBoxLayout* vLayout = new QVBoxLayout(frame);

    lineEdit_ = new QLineEdit();
    lineEdit_->setPlaceholderText("Filter processor list...");
    connect(lineEdit_, SIGNAL(textChanged(const QString&)), this, SLOT(addProcessorsToTree()));
    vLayout->addWidget(lineEdit_);

    QHBoxLayout* listViewLayout = new QHBoxLayout();
    listViewLayout->addWidget(new QLabel("Group processors by"));
    listView_ = new QComboBox();
    listView_->addItem("Category");
    listView_->addItem("Module");
    listView_->addItem("Code State");
    connect(listView_, SIGNAL(currentIndexChanged(int)), this, SLOT(addProcessorsToTree()));
    listView_->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    listViewLayout->addWidget(listView_);
    vLayout->addLayout(listViewLayout);

    iconStable_ = QIcon(":/icons/processor_stable.png");
    iconExperimental_ = QIcon(":/icons/processor_experimental.png");
    iconBroken_ = QIcon(":/icons/processor_broken.png");

    processorTree_ = new ProcessorTree(this);
    processorTree_->setHeaderHidden(true);
    addProcessorsToTree();
    vLayout->addWidget(processorTree_);

    frame->setLayout(vLayout);
    setWidget(frame);
}

ProcessorTreeWidget::~ProcessorTreeWidget() {}

bool ProcessorTreeWidget::processorFits(ProcessorFactoryObject* processor, const QString& filter) {
    return (QString::fromStdString(processor->getClassName()).contains(filter, Qt::CaseInsensitive));
}

QIcon* ProcessorTreeWidget::getCodeStateIcon(CodeState state){
    switch(state){
        case CODE_STATE_STABLE:
            return &iconStable_;
        case CODE_STATE_BROKEN:
            return &iconBroken_;
        default:
            return &iconExperimental_;
    }
}

void ProcessorTreeWidget::addProcessorsToTree() {
    processorTree_->clear();
    // add processors from all modules to the list
    InviwoApplication* inviwoApp = InviwoApplication::getPtr();
    if(listView_->currentIndex() == 2){ //By Stability
        QTreeWidgetItem* stableItems = new QTreeWidgetItem(QStringList(QString("Stable Processors")));
        QTreeWidgetItem* experimentalItems = new QTreeWidgetItem(QStringList(QString("Experimental Processors")));
        QTreeWidgetItem* brokenItems = new QTreeWidgetItem(QStringList(QString("Broken Processors")));
        for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
            std::vector<ProcessorFactoryObject*> curProcessorList = inviwoApp->getModules()[curModuleId]->getProcessors();
            for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++) {
                if (lineEdit_->text().isEmpty() || processorFits(curProcessorList[curProcessorId], lineEdit_->text())) {
                    QTreeWidgetItem* newItem = new QTreeWidgetItem(QStringList(QString::fromStdString(curProcessorList[curProcessorId]->getClassName())));
                    switch(curProcessorList[curProcessorId]->getCodeState()){
                        case CODE_STATE_STABLE:
                            newItem->setIcon(0, iconStable_);
                            stableItems->addChild(newItem);
                            break;
                        case CODE_STATE_BROKEN:
                            newItem->setIcon(0, iconBroken_);
                            brokenItems->addChild(newItem);
                            break;
                        default:
                            newItem->setIcon(0, iconExperimental_);
                            experimentalItems->addChild(newItem);
                    }
                }
            }
        }
        if(stableItems->childCount() > 0){
            processorTree_->addTopLevelItem(stableItems);
            stableItems->sortChildren(0, Qt::AscendingOrder);
        }
        if(experimentalItems->childCount() > 0){
            processorTree_->addTopLevelItem(experimentalItems);
            experimentalItems->sortChildren(0, Qt::AscendingOrder);
        }
        if(brokenItems->childCount() > 0){
            processorTree_->addTopLevelItem(brokenItems);
            brokenItems->sortChildren(0, Qt::AscendingOrder);
        }
    }
    else if(listView_->currentIndex() == 1){ //By Module
        for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
            QTreeWidgetItem* moduleItem = new QTreeWidgetItem(QStringList(QString::fromStdString(inviwoApp->getModules()[curModuleId]->getIdentifier())));
            std::vector<ProcessorFactoryObject*> curProcessorList = inviwoApp->getModules()[curModuleId]->getProcessors();
            for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++) {
                if (lineEdit_->text().isEmpty() || processorFits(curProcessorList[curProcessorId], lineEdit_->text())) {
                    QTreeWidgetItem* newItem = new QTreeWidgetItem(QStringList(QString::fromStdString(curProcessorList[curProcessorId]->getClassName())));
                    newItem->setIcon(0, *getCodeStateIcon(curProcessorList[curProcessorId]->getCodeState()));
                    moduleItem->addChild(newItem);
                }
            }
            if(moduleItem->childCount() > 0)
                processorTree_->addTopLevelItem(moduleItem);
        }
        processorTree_->sortItems(0, Qt::AscendingOrder);
    }
    else{ // By Category
        for (size_t curModuleId=0; curModuleId<inviwoApp->getModules().size(); curModuleId++) {
            std::vector<ProcessorFactoryObject*> curProcessorList = inviwoApp->getModules()[curModuleId]->getProcessors();
            for (size_t curProcessorId=0; curProcessorId<curProcessorList.size(); curProcessorId++) {
                if (lineEdit_->text().isEmpty() || processorFits(curProcessorList[curProcessorId], lineEdit_->text())) {
                    QString categoryName = QString::fromStdString(curProcessorList[curProcessorId]->getCategory());
                    QList<QTreeWidgetItem*> items = processorTree_->findItems(categoryName, Qt::MatchFixedString, 0);
                    if(items.empty()){
                        items.push_back(new QTreeWidgetItem(QStringList(categoryName)));
                        processorTree_->addTopLevelItem(items[0]);
                    }
                    QTreeWidgetItem* newItem = new QTreeWidgetItem(QStringList(QString::fromStdString(curProcessorList[curProcessorId]->getClassName())));
                    newItem->setIcon(0, *getCodeStateIcon(curProcessorList[curProcessorId]->getCodeState()));
                    items[0]->addChild(newItem);
                }
            }
        }
        processorTree_->sortItems(0, Qt::AscendingOrder);
    }
    processorTree_->expandAll();
}

static QString mimeType = "inviwo/ProcessorDragObject";

ProcessorDragObject::ProcessorDragObject(QWidget* source, const QString className) : QDrag(source) {
    QByteArray byteData;
    {
        QDataStream ds(&byteData, QIODevice::WriteOnly);
        ds << className;
    }

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(mimeType, byteData);
    mimeData->setData("text/plain", className.toLatin1().data());
    setMimeData(mimeData);

    start(Qt::MoveAction);
}

bool ProcessorDragObject::canDecode(const QMimeData* mimeData) {
    if (mimeData->hasFormat(mimeType)) return true;
    else return false;
}

bool ProcessorDragObject::decode(const QMimeData* mimeData, QString& className) {
    QByteArray byteData = mimeData->data(mimeType);
    if (byteData.isEmpty())
        return false;

    QDataStream ds(&byteData, QIODevice::ReadOnly);
    ds >> className;
    return true;
}

} // namespace