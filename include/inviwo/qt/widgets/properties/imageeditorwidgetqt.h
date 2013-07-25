#ifndef IVW_IMAGEEDITORWIDGETQT_H
#define IVW_IMAGEEDITORWIDGETQT_H

//QT includes
#include <QFile>
#include <QCheckBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QTextStream>
#include <QToolBar>
#include <QToolButton>
#include <QWidget>
#include <QMainWindow>
#include <QDesktopServices>
#include <QUrl>
#include <QPainter>
#include <QLabel>
#include <QImage>
#include <QProgressDialog>
#include <QPixmap>
#include <QMouseEvent>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>


//Property includes
#include <inviwo/core/properties/buttonproperty.h>
#include <inviwo/core/properties/imageeditorproperty.h>
#include <inviwo/core/properties/property.h>

//Widget includes
#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/properties/buttonpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/filepropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/stringpropertywidgetqt.h>


namespace inviwo{

/////////////////////////////////////////////////
// Simple Graphics scene
class SimpleGraphicsScene : public QGraphicsScene {
    Q_OBJECT
public:
    SimpleGraphicsScene(QWidget* parent=0);
signals:
    void status(const QString&);
};


/////////////////////////////////////////////////
// Simple Graphics view
class SimpleGraphicsView : public QGraphicsView {
public:
    SimpleGraphicsView(QWidget* parent=0);
    ~SimpleGraphicsView();
    void setDialogScene(QGraphicsScene* scene);
protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseDoubleClickEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QGraphicsScene* scene_;
    QPoint startPoint_;
    bool rubberBandActive_;
};


/////////////////////////////////////////////////
// Image Labeling widget
class ImageEditorWidgetQt;
class IVW_QTWIDGETS_API ImageLabelWidget : public QWidget{
    Q_OBJECT;
public:
    ImageLabelWidget();
    bool saveDialog();
    void setParent(ImageEditorWidgetQt*);
    QGraphicsScene *getScene() {return scene_;}
    QGraphicsView *getView() {return view_;}
    void addRectangle();
    void addBackGroundImage(std::string imagePath);
    void generateWidget();
    ImageEditorWidgetQt* mainParentWidget_;
    QToolBar* toolBar_;
    QToolButton *reDoButton_;
    QToolButton *reLoadButton_;
    QToolButton *saveButton_;
    QToolButton *unDoButton_;
    std::string tmpPropertyValue_;   
    SimpleGraphicsScene* scene_;
    SimpleGraphicsView* view_;
    QImage* backGroundImage_;  
protected:
    void closeEvent(QCloseEvent *);
    /*void showEvent(QShowEvent *);*/
};

/////////////////////////////////////////////////
// Image Editor widget
class IVW_QTWIDGETS_API ImageEditorWidgetQt : public PropertyWidgetQt{    
    Q_OBJECT;
public:    
    ImageEditorWidgetQt(Property* property);
    ~ImageEditorWidgetQt();
    void updateFromProperty();
    bool saveDialog();
private:
    QToolButton* btnEdit_;
    FilePropertyWidgetQt* fileWidget_;
    Property* property_;   
    ImageLabelWidget* imageLabelWidget_;
    std::string tmpPropertyValue_;
    QCheckBox* checkBox_;
    void generateWidget();   
public slots:
    void loadImageLabel();    
    void editImageLabel();    
    void setPropertyValue();
    bool writeImageLabel();
};


}//namespace

#endif //IVW_IMAGEEDITORWIDGETQT_H