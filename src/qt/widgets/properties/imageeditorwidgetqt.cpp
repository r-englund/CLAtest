#include <inviwo/qt/widgets/properties/imageeditorwidgetqt.h>
#include <inviwo/core/common/inviwomodule.h>

namespace inviwo {

/////////////////////////////////////////////////
// Simple Graphics Scene
SimpleGraphicsScene::SimpleGraphicsScene(QWidget* parent) : QGraphicsScene(parent) { }


/////////////////////////////////////////////////
// Simple Graphics Rectangle Item with label 
// used by Simple Graphics View

SimpleWithRectangleLabel::SimpleWithRectangleLabel(QPointF rectSize, QGraphicsScene* scene) 
    : QGraphicsRectItem(), label_(0) {
    setRect(0, 0, rectSize.x(), rectSize.y());
    label_ = new LabelGraphicsItem(this);
    label_->setPos(0, 0);
    label_->setDefaultTextColor(Qt::black);
    label_->setFont(QFont("Segoe", 10, QFont::Black, false));
    label_->setCrop(8, 7);
}

SimpleWithRectangleLabel::~SimpleWithRectangleLabel() {}

void SimpleWithRectangleLabel::updateLabelPosition() {
   //set offset
}

void SimpleWithRectangleLabel::setLabel(std::string label) {
    label_->setText(QString::fromStdString(label));
}

std::string SimpleWithRectangleLabel::getLabel() { return label_->text().toStdString(); }

void SimpleWithRectangleLabel::editLabel() {
    setFocus();
    label_->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsFocusable);
    label_->setTextInteractionFlags(Qt::TextEditorInteraction);
    label_->setFocus();
}


/////////////////////////////////////////////////
// Simple Graphics view

SimpleGraphicsView::SimpleGraphicsView(QWidget* parent) : QGraphicsView(parent), scene_(0), rubberBandActive_(false) {    
    setRenderHint(QPainter::Antialiasing, true);
    setMouseTracking(true);
    setDragMode(QGraphicsView::RubberBandDrag);
}

void SimpleGraphicsView::setDialogScene(QGraphicsScene* scene) {
    //scene->setBackgroundBrush(QBrush(Qt::lightGray));
    setScene(scene);
    scene_ = scene;
}

void SimpleGraphicsView::addRectangle(QPointF mStartPoint, QPointF deltaPoint) {
    //QAbstractGraphicsShapeItem *i = scene_->addRect( mStartPoint.x(), mStartPoint.y(),  deltaPoint.x(), deltaPoint.y() );    
    SimpleWithRectangleLabel *i = new SimpleWithRectangleLabel(deltaPoint, scene_);
    i->setPos(mStartPoint.x(), mStartPoint.y());
    scene_->addItem(i);
    i->setLabel("Box");
    i->updateLabelPosition();
    i->setFlag(QGraphicsItem::ItemIsMovable);
    i->setBrush( QColor(0,0,128,0) );
    i->setPen( QPen(QColor(255, 0, 0), 2) );
    i->setZValue(255);
}

void SimpleGraphicsView::mouseDoubleClickEvent(QMouseEvent* e) {

    QPoint currentPoint = e->pos();
    QList<QGraphicsItem*> graphicsItems =items(e->pos()); 

    //graphicsItems.size()==1 because of background pixmap item
    if(e->button()==Qt::LeftButton && graphicsItems.size()>1) {
        //Delete rectangle             
        for (int i=0; i<graphicsItems.size(); i++) {
            SimpleWithRectangleLabel *rectItem = qgraphicsitem_cast<SimpleWithRectangleLabel*>(graphicsItems[i]);
            if (rectItem) {                    
                rectItem->editLabel(); 
            }
        }
    }

    QGraphicsView::mouseDoubleClickEvent(e);
}

SimpleGraphicsView::~SimpleGraphicsView() {}

void SimpleGraphicsView::mousePressEvent(QMouseEvent* e) {

    QPoint currentPoint = e->pos();
    QList<QGraphicsItem*> graphicsItems =items(e->pos());    
    //graphicsItems.size()==1 because of background pixmap item
    if(e->button()==Qt::LeftButton && graphicsItems.size()<2) {
        //Left click on canvas region, where there is no rectangle item
        startPoint_ = currentPoint;
        rubberBandActive_ = true;
    }
    else {
        rubberBandActive_ = false;
        if (e->modifiers() == Qt::ControlModifier) {
            //Delete rectangle             
            for (int i=0; i<graphicsItems.size(); i++) {
                QGraphicsRectItem *rectItem = qgraphicsitem_cast<QGraphicsRectItem*>(graphicsItems[i]);
                if (rectItem) {                    
                    scene_->removeItem(rectItem);                    
                }
            }
        }
    }
    //e->accept();
    QGraphicsView::mousePressEvent(e);    
}

std::vector<ImgRect> SimpleGraphicsView::getRectList() {
     std::vector<ImgRect> rectList;
     QList<QGraphicsItem*> graphicsItems = items(); 
     for (int i=0; i<graphicsItems.size(); i++) {
         SimpleWithRectangleLabel *rectItem = qgraphicsitem_cast<SimpleWithRectangleLabel*>(graphicsItems[i]);
         if (rectItem) {
            ImgRect r;
            r.rect_ = QRectF(rectItem->mapRectToScene(rectItem->rect()));
            r.label_ = rectItem->getLabel();
            rectList.push_back(r) ;             
         }
     }

     return rectList;
}

void SimpleGraphicsView::mouseReleaseEvent(QMouseEvent *e)
{
    QPointF mendPoint = mapToScene(e->pos());
    QPointF mStartPoint = mapToScene(startPoint_);
    QPointF deltaPoint = mendPoint - mStartPoint;

    if (rubberBandActive_ && ( deltaPoint.x()>5 && deltaPoint.y()>5) ) {        
        if (e->modifiers() == Qt::ShiftModifier) {
            //add a square
            (deltaPoint.x()>deltaPoint.y())?deltaPoint.setY(deltaPoint.x()):deltaPoint.setX(deltaPoint.y());
        }
        addRectangle(mStartPoint, deltaPoint);
    }
    rubberBandActive_ = false;
    //e->accept();
    QGraphicsView::mouseReleaseEvent(e);
}

/////////////////////////////////////////////////
// Image Labeling widget
ImageLabelWidget::ImageLabelWidget() : scene_(0), view_(0) {
    generateWidget();
}

void ImageLabelWidget::closeEvent(QCloseEvent *event) {
    mainParentWidget_->saveDialog();
    if (QWidget::isVisible())
        QWidget::hide();
    event->ignore();
}

void ImageLabelWidget::generateWidget(){
    
    scene_ = new SimpleGraphicsScene(this);
    scene_->setSceneRect(0, 0, 10, 10);
    view_ = new SimpleGraphicsView(this);
    view_->setDialogScene(scene_);
    scene_->setBackgroundBrush(QBrush(Qt::lightGray));

    QVBoxLayout* editorLayout = new QVBoxLayout();
    editorLayout->setSpacing(0);
    editorLayout->setMargin(0);
    toolBar_ = new QToolBar();

    saveButton_ = new QToolButton();
    saveButton_->setIcon(QIcon(":/icons/save.png")); // Temporary icon
    saveButton_->setToolTip("Save file");
    unDoButton_ = new QToolButton();
    unDoButton_->setIcon(QIcon(":/icons/arrow_left.png")); // Temporary icon
    unDoButton_->setToolTip("Undo");
    reDoButton_ = new QToolButton();
    reDoButton_->setIcon(QIcon(":/icons/arrow_right.png")); // Temporary icon
    reDoButton_->setToolTip("Redo");
    reLoadButton_ = new QToolButton();
    reLoadButton_->setIcon(QIcon(":/icons/inviwo_tmp.png")); // Temporary icon
    reLoadButton_->setToolTip("Reload");

    toolBar_->addWidget(saveButton_);
    toolBar_->addSeparator();
    toolBar_->addWidget(unDoButton_);
    toolBar_->addSeparator();
    toolBar_->addWidget(reDoButton_);
    toolBar_->addSeparator();
    toolBar_->addWidget(reLoadButton_);
    toolBar_->addSeparator();   
    
    editorLayout->addWidget(toolBar_);
    editorLayout->addWidget(view_);
    setLayout(editorLayout);
 
    //test rectangle
    //addRectangleTest();

    //connect(unDoButton_,SIGNAL(pressed()),editor_,SLOT(undo()));
    //connect(reDoButton_,SIGNAL(pressed()),editor_,SLOT(redo()));
}

void ImageLabelWidget::addRectangleTest() {
    QAbstractGraphicsShapeItem *i = scene_->addRect( 0, 0 , 25, 25 );
    i->setFlag(QGraphicsItem::ItemIsMovable);
    i->setBrush( QColor(0,0,128,0) );
    i->setPen( QPen(QColor(255, 0, 0), 2) );
    i->setZValue(255);
}

void ImageLabelWidget::addBackGroundImage(std::string imagePath) {
    scene_->clear();
    backGroundImage_ = new QImage(imagePath.c_str());
    QGraphicsPixmapItem *i = scene_->addPixmap( QPixmap(imagePath.c_str()) );    
    i->setZValue(1);    
    scene_->setSceneRect(0, 0, backGroundImage_->width(), backGroundImage_->height());
    //resize(image.size()*4/3);
    setFixedSize(backGroundImage_->size()*1.7f);
}

void ImageLabelWidget::setParent(ImageEditorWidgetQt* tmp){
    mainParentWidget_ = tmp;
}

/////////////////////////////////////////////////
// Image Editor widget
ImageEditorWidgetQt::ImageEditorWidgetQt(Property* property) : property_(property), imageLabelWidget_(0) {
    generateWidget();
    updateFromProperty();
}

ImageEditorWidgetQt::~ImageEditorWidgetQt() {
    if (imageLabelWidget_) {
        imageLabelWidget_->hide();
        imageLabelWidget_->deleteLater();
    }
}

void ImageEditorWidgetQt::generateWidget() {

    QHBoxLayout* hLayout = new QHBoxLayout();
    btnEdit_ = new QToolButton();
    btnEdit_->setIcon(QIcon(":/icons/edit.png"));

    if (dynamic_cast<FileProperty*>(property_)) {
        fileWidget_ = new FilePropertyWidgetQt(static_cast<FileProperty*>(property_));
        connect(btnEdit_,SIGNAL(clicked()),this,SLOT(editImageLabel()));
        hLayout->addWidget(fileWidget_);
    }

    hLayout->addWidget(btnEdit_);
    setLayout(hLayout);
    hLayout->setContentsMargins(QMargins(0,0,0,0)); 

    imageLabelWidget_= new ImageLabelWidget();
    imageLabelWidget_->setParent(this);
    imageLabelWidget_->hide();
}

void ImageEditorWidgetQt::setPropertyValue() {}

void ImageEditorWidgetQt::editImageLabel(){
    
    ImageEditorProperty* imageEditorProperty = static_cast<ImageEditorProperty*>(property_);
    if (imageEditorProperty) {
        if (static_cast<FileProperty*>(property_)->get() == "")
            fileWidget_->setPropertyValue();        
        connect(imageLabelWidget_->saveButton_, SIGNAL(pressed()), this, SLOT(writeImageLabel()));
        connect(imageLabelWidget_->reLoadButton_, SIGNAL(pressed()), this, SLOT(loadImageLabel()));
        loadImageLabel();
        imageLabelWidget_->show();
    }    
}

void ImageEditorWidgetQt::loadImageLabel(){
    if (tmpPropertyValue_!=static_cast<FileProperty*>(property_)->get()) {
        tmpPropertyValue_ = static_cast<FileProperty*>(property_)->get();
        imageLabelWidget_->addBackGroundImage(tmpPropertyValue_);
        
        ImageEditorProperty* imageProperty  = static_cast<ImageEditorProperty*>(property_);
        const std::vector<ImageLabel*> labels = imageProperty->getLabels();

        for (size_t i=0; i<labels.size(); i++) {
            QPointF topLeft(labels[i]->getTopLeft()[0], labels[i]->getTopLeft()[1]);
            QPointF rectSize(labels[i]->getSize()[0], labels[i]->getSize()[1]);
            imageLabelWidget_->view_->addRectangle(topLeft, rectSize);
        }
    }
}

//Function writes content of the textEditor_ to the file
bool ImageEditorWidgetQt::writeImageLabel(){
    //Close the file to open it with new flags
    ImageEditorProperty* imageEditorProperty = dynamic_cast<ImageEditorProperty*>(property_);
    if (imageEditorProperty) {
        //Save labels
        std::stringstream ss;               
        std::vector<ImgRect> rectList = imageLabelWidget_->view_->getRectList(); 

        QSize dim = imageLabelWidget_->backGroundImage_->size();
        imageEditorProperty->setDimensions(ivec2(dim.width(), dim.height()) );

        //if (rectList.size()) 
            imageEditorProperty->clearLabels();

        for (size_t i=0; i<rectList.size(); i++) {
            vec2 topLeft(rectList[i].rect_.topLeft().x(), rectList[i].rect_.topLeft().y());
            vec2 rectSize(rectList[i].rect_.size().width(), rectList[i].rect_.size().height());
            imageEditorProperty->addLabel(topLeft, rectSize, rectList[i].label_);            
        }
    }
    return true;
}

bool ImageEditorWidgetQt::saveDialog(){
    if (imageLabelWidget_->getView()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
            tr("The image has been modified.\n"
            "Do you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save){
            writeImageLabel();              
        }
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void ImageEditorWidgetQt::updateFromProperty() {
    StringProperty* stringProp = dynamic_cast<StringProperty*>(property_);
    FileProperty* fileProp = dynamic_cast<FileProperty*>(property_);
    if (fileProp) {
        fileWidget_->updateFromProperty();
        loadImageLabel();
    }    
}



} // namespace
