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
 * Primary author : Sathish Kottravel
 *
 **********************************************************************/

#ifndef IVW_PROPERTY_SELECTION_TREE_WIDGET_H
#define IVW_PROPERTY_SELECTION_TREE_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>

namespace inviwo {

class Property;
class ProcessorNetwork;

class IVW_QTWIDGETS_API PropertySelectionTree : public QTreeWidget {

public:
    PropertySelectionTree(QWidget* parent) : QTreeWidget(parent) {};
    ~PropertySelectionTree() {};

protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseMoveEvent(QMouseEvent* e);
private:
    QPoint dragStartPosition_;
};

class IVW_QTWIDGETS_API PropertySelectionTreeWidget : public QWidget {
    Q_OBJECT
public:
    PropertySelectionTreeWidget();
    ~PropertySelectionTreeWidget();
    void addProcessorNetwork(ProcessorNetwork* processorNetwork, std::string workspaceFileName="CurrentWorkspace");
public slots:
    void clear();
    std::vector<Property*> getSelectedProperties(ProcessorNetwork* processorNetwork);
private:
    PropertySelectionTree* propertySelectionTree_;
    QPoint dragStartPosition_;
    QVBoxLayout* vLayout_;
};

class IVW_QTWIDGETS_API PropertySelectionTreeDialog : public QDialog {
    Q_OBJECT
public:
    PropertySelectionTreeDialog(ProcessorNetwork* processorNetwork,
                                std::vector<Property*>& selectedProperty,
                                QWidget* parent);
    ~PropertySelectionTreeDialog() {}
private slots:
    void clickedOkayButton();
    void clickedCancelButton();
private:
    void initDialog();
    PropertySelectionTreeWidget* selectionTree_;
    std::vector<Property*>& selectedProperties_;
    ProcessorNetwork* processorNetwork_;
};


} // namespace

#endif // IVW_PROPERTY_SELECTION_TREE_WIDGET_H