#ifndef IVW_STUDY_PARAMETER_LIST_WIDGET_H
#define IVW_STUDY_PARAMETER_LIST_WIDGET_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/core/util/observer.h>
#include <QWidget>
#include <QListWidget>
#include <QDialog>
#include <QPushButton>

namespace inviwo {

class FileProperty;

class IVW_QTWIDGETS_API StudyParameterListWidget : public QWidget, public VoidObserver {
    Q_OBJECT
public:
    StudyParameterListWidget(QWidget* parent);
    virtual ~StudyParameterListWidget(); 
    void notify();
    void addWorkspace();
public slots:
    void onListItemDoubleClicked(QListWidgetItem * item, int column);
    void clickedClearButton();
protected:
    void buildWidgets();
    void updateStudyParameterList();    
    FileProperty* addWorkspaceProperty_;
    QListWidget* propertyList_;
};

class IVW_QTWIDGETS_API StudyParameterDialog : public QDialog {
    Q_OBJECT
public:
    StudyParameterDialog(QWidget* parent);
    ~StudyParameterDialog() {}
    private slots:
        void clickedOkayButton();
        void clickedCancelButton();
private:
    void initDialog();    
    StudyParameterListWidget* studyParameterListWidget_;
};

}


#endif // IVW_STUDY_PARAMETER_LIST_WIDGET_H

