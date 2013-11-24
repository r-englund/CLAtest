#ifndef IVW_PROPERTY_SELECTION_TREE_WIDGET_H
#define IVW_PROPERTY_SELECTION_TREE_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QApplication>
#include <QDialog>
#include <QPushButton>
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/core/util/singleton.h>

namespace inviwo {

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

class IVW_QTWIDGETS_API PropertySelectionTreeDialog : public QDialog, public Singleton<PropertySelectionTreeDialog> {
    Q_OBJECT
public:
    PropertySelectionTreeDialog(ProcessorNetwork* processorNetwork,
                                std::vector<Property*> &selectedProperty,
                                QWidget* parent);
    ~PropertySelectionTreeDialog() {}
private slots:
    void clickedOkayButton();
    void clickedCancelButton();
private:
    void initDialog();    
    PropertySelectionTreeWidget* selectionTree_;
    std::vector<Property*> &selectedProperties_;
    ProcessorNetwork* processorNetwork_;
};


} // namespace

#endif // IVW_PROPERTY_SELECTION_TREE_WIDGET_H