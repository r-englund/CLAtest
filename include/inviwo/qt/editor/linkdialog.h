
#ifndef  IVW_LINKDIALOG_H
#define  IVW_LINKDIALOG_H

#include <QGraphicsView>
#include <QDialog>
#include <QGraphicsRectItem>
#include <QPushButton>
#include <QGraphicsLineItem>
#include <QPainterPath>

#include "inviwo/core/network/processorlink.h"
#include "inviwo/core/network/processornetwork.h"
#include "inviwo/qt/editor/labelgraphicsitem.h"
#include "inviwo/qt/editor/connectiongraphicsitem.h"

static const qreal LINKDIALOG_PROCESSOR_GRAPHICSITEM_DEPTH = 1.0f;
static const qreal LINKDIALOG_PROPERTY_GRAPHICSITEM_DEPTH = 2.0f;

namespace inviwo {

enum InviwoLinkUserGraphicsItemType {
    LinkDialogProcessorGraphicsItemType = 4,
    LinkDialogPropertyGraphicsItemType = 5,
    LinkDialogCurveGraphicsItemType = 6
};


class DialogCurveGraphicsItem : public CurveGraphicsItem {

public:
    DialogCurveGraphicsItem(QPointF startPoint, QPointF endPoint, bool layoutOption=false, ivec3 color=ivec3(38,38,38), bool dragMode=true);
    ~DialogCurveGraphicsItem();    

    //override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + LinkDialogCurveGraphicsItemType };
    int type() const  {return Type; }

};

class LinkDialogPropertyGraphicsItem;
class LinkDialogProcessorGraphicsItem;

class DialogConnectionGraphicsItem : public DialogCurveGraphicsItem {

public:

    DialogConnectionGraphicsItem(LinkDialogPropertyGraphicsItem* outProperty, LinkDialogPropertyGraphicsItem* inProperty,
                           bool layoutOption=true);
    ~DialogConnectionGraphicsItem();

    LinkDialogPropertyGraphicsItem* getOutProperty() const { return outPropertyGraphicsItem_; }
 
    LinkDialogPropertyGraphicsItem* getInProperty() const { return inPropertyGraphicsItem_; }

private:
    LinkDialogPropertyGraphicsItem* outPropertyGraphicsItem_;
    LinkDialogPropertyGraphicsItem* inPropertyGraphicsItem_;
};


/*---------------------------------------------------------------------------------------*/

class LinkDialogGraphicsScene : public QGraphicsScene {
public:
    LinkDialogGraphicsScene(QWidget* parent=0);
    ~LinkDialogGraphicsScene() {}
    QGraphicsItem* getPropertyGraphicsItemAt(const QPointF pos) const;
    QGraphicsItem* getPropertyGraphicsItemAt(Property* property);
    void setNetwork(ProcessorNetwork* network) {processorNetwork_ = network;}

    void initScene(std::vector<Processor*> srcProcessorList, std::vector<Processor*> dstProcessorList);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    void keyPressEvent(QKeyEvent* keyEvent);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* e);
    void addPropertyLink(LinkDialogPropertyGraphicsItem* outProperty, LinkDialogPropertyGraphicsItem* inProperty);
    void addPropertyLink(PropertyLink* propertyLink);
    
    void initializePorpertyLinkRepresentation(LinkDialogPropertyGraphicsItem* outProperty, LinkDialogPropertyGraphicsItem* inProperty);
    void addProcessorsItemsToScene(Processor *prcoessor, int xPosition, int yPosition);

private:
    DialogCurveGraphicsItem* linkCurve_;
    LinkDialogPropertyGraphicsItem* startProperty_;
    LinkDialogPropertyGraphicsItem* endProperty_;

    std::vector<LinkDialogProcessorGraphicsItem*> processorGraphicsItems_;
    std::vector<DialogConnectionGraphicsItem*> connectionGraphicsItems_;
    
    ProcessorNetwork* processorNetwork_;
};


/*---------------------------------------------------------------------------------------*/

class LinkDialogGraphicsView : public QGraphicsView {
public:
    LinkDialogGraphicsView(QWidget* parent=0);
    ~LinkDialogGraphicsView();
    void setDialogScene(LinkDialogGraphicsScene* scene);
};

/*---------------------------------------------------------------------------------------*/

template <typename T>
class GraphicsItemData : public QGraphicsRectItem {
public:
    GraphicsItemData(T* item=0) : QGraphicsRectItem() {item_ = item;}
    T* getGraphicsItemData() {return item_;}
    void setGraphicsItemData(T* item) {item_ = item;}
private:
    T* item_;
};

class LinkDialogPropertyGraphicsItem;

class LinkDialogProcessorGraphicsItem : public GraphicsItemData<Processor> {

public:
    LinkDialogProcessorGraphicsItem();
    ~LinkDialogProcessorGraphicsItem();

    void setProcessor(Processor* processor);
    Processor* getProcessor() {return getGraphicsItemData();}

    //void addToItemsToScene(LinkDialogGraphicsView *linkDialogView);

    std::vector<LinkDialogPropertyGraphicsItem*> getPropertyItemList() {return propertyGraphicsItems_;}

    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

    //override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + LinkDialogProcessorGraphicsItemType };
    int type() const  {return Type; }

protected:
    virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    LabelGraphicsItem* nameLabel_;
    LabelGraphicsItem* classLabel_;
    std::vector<LinkDialogPropertyGraphicsItem*> propertyGraphicsItems_;
};

/*---------------------------------------------------------------------------------------*/

class LinkDialogPropertyGraphicsItem : public GraphicsItemData<Property> {

public:
    LinkDialogPropertyGraphicsItem(LinkDialogProcessorGraphicsItem* , Property* );
    ~LinkDialogPropertyGraphicsItem();

    void setProperty(Property* property);

    QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;

    //override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + LinkDialogPropertyGraphicsItemType };
    int type() const  {return Type; }

    QPointF getShortestBoundaryPointTo(LinkDialogPropertyGraphicsItem*);
    QPointF getShortestBoundaryPointTo(QPointF);

    void updatePositionBasedOnProcessor();

protected:
    virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    
    LabelGraphicsItem* classLabel_;
    LinkDialogProcessorGraphicsItem* processorGraphicsItem_;
};

/*---------------------------------------------------------------------------------------*/

class LinkDialog : public QDialog {
Q_OBJECT
public:
    LinkDialog(std::vector<ProcessorLink*> processorLinks, ProcessorNetwork* network, QWidget* parent);    
    LinkDialog(Processor* src, Processor *dest, ProcessorNetwork* network, QWidget* parent);

private slots:
   void handleButton(QAbstractButton* b);

private:   
    void initDialog();

    LinkDialogGraphicsView* linkDialogView_;
    LinkDialogGraphicsScene* linkDialogScene_;
    QPushButton* leftButton_;
    QPushButton* rightButton_;
    QPushButton* bidirectionButton_;

    //Start and End Properties
    //Curve graphics item
};

/*---------------------------------------------------------------------------------------*/

} //namespace

#endif //IVW_LINKDIALOG_H
