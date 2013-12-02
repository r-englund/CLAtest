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
#include <QSpinBox>
#include <QGraphicsDropShadowEffect>


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
#include <inviwo/qt/widgets/labelgraphicsitem.h>


namespace inviwo{

/////////////////////////////////////////////////
// Simple Graphics scene
class IVW_QTWIDGETS_API SimpleGraphicsScene : public QGraphicsScene {
    Q_OBJECT
public:
    SimpleGraphicsScene(QWidget* parent=0);
signals:
    void status(const QString&);
};



/////////////////////////////////////////////////
// Simple Graphics Rectangle Item with label 
// used by Simple Graphics View
class IVW_QTWIDGETS_API SimpleWithRectangleLabel : public QGraphicsRectItem {
public:
    SimpleWithRectangleLabel(QPointF mendPoint, QGraphicsScene* scene);
    SimpleWithRectangleLabel();
    ~SimpleWithRectangleLabel();
    void updateLabelPosition();
    void setLabel(std::string label);
    std::string getLabel();
    void editLabel();
private:
    LabelGraphicsItem* label_;
};

/////////////////////////////////////////////////
// Simple Graphics view

struct ImgRect {
    QRectF rect_;
    std::string label_;
};

class IVW_QTWIDGETS_API SimpleGraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    SimpleGraphicsView(QWidget* parent=0);
    ~SimpleGraphicsView();
    void setDialogScene(QGraphicsScene* scene);
    void addRectangle(QPointF mstartPoint, QPointF mendPoint,ivec3 color = ivec3(0,0,255));
    std::vector<ImgRect> getRectList();
    void setReadOnly(bool readOnly);
    void hideLabelDescription(bool hide);
    void hideLabels(bool hide);
    void filledRectangles(bool fill);
    void setCurrentLabelPositionFromTextField(ivec2 pos);
    void setCurrentLabelPositionToTextField();
    void setScaleFactor(float scaling);
    void clearRectItems();
protected:
    void mousePressEvent(QMouseEvent* e);
    void mouseDoubleClickEvent(QMouseEvent* e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
signals:
    void currentRectItemPositionChanged(vec2 pos);
private:
    QGraphicsScene* scene_;
    QPoint startPoint_;
    bool rubberBandActive_;
    bool readOnly_;
    bool hideLabels_;
    bool hideLabelDescriptions_;
    bool fillRectangle_;
    QGraphicsRectItem* currentRectItem_;
    QGraphicsDropShadowEffect* shadowEffect_;    
};


/////////////////////////////////////////////////
// Image Labeling widget
class ImageEditorWidgetQt;
class IVW_QTWIDGETS_API ImageLabelWidget : public QWidget{
    Q_OBJECT
public:
    ImageLabelWidget();
    bool saveDialog();
    void setParent(ImageEditorWidgetQt*);
    QGraphicsScene *getScene() {return scene_;}
    QGraphicsView *getView() {return view_;}
    void addRectangleTest();
    void addBackGroundImage(std::string imagePath);
    void generateWidget();    
    void setReadOnly(bool readOnly);
    void hideLabelDescription(bool hide);
    void hideLabels(bool hide);
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
    void setToolBarVisible(bool visible);
    void setSceneScaling(float scaling);
public slots:
    void updatePositionX(int);
    void updatePositionY(int);
    void onCurrentItemPositionChange(vec2 pos);
signals:
    void rectItemPositionChanged();
protected:
    void closeEvent(QCloseEvent *);
    QSpinBox* positionX_;
    QSpinBox* positionY_;
    QLabel* labelPositionX_;
    QLabel* labelPositionY_;
    float sceneScaleFactor_;    
    /*void showEvent(QShowEvent *);*/
};

/////////////////////////////////////////////////
// Image Editor widget
class IVW_QTWIDGETS_API ImageEditorWidgetQt : public PropertyWidgetQt{    
    Q_OBJECT
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