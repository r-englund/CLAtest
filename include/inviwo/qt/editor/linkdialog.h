
#ifndef  IVW_LINKDIALOG_H
#define  IVW_LINKDIALOG_H

#include <inviwo/qt/editor/inviwoqteditordefine.h>
#include <QGraphicsView>
#include <QDialog>
#include <QGraphicsRectItem>
#include <QPushButton>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QPainterPath>
#include <QDialogButtonBox>

#include <inviwo/core/network/processorlink.h>
#include <inviwo/core/network/processornetwork.h>
#include <inviwo/qt/editor/labelgraphicsitem.h>
#include <inviwo/qt/editor/connectiongraphicsitem.h>

static const qreal LINKDIALOG_PROCESSOR_GRAPHICSITEM_DEPTH = 1.0f;
static const qreal LINKDIALOG_PROPERTY_GRAPHICSITEM_DEPTH = 2.0f;
static const qreal LINKDIALOG_CONNECTION_GRAPHICSITEM_DEPTH = 3.0f;

namespace inviwo {

IVW_QTEDITOR_API enum InviwoLinkUserGraphicsItemType {
    LinkDialogProcessorGraphicsItemType = 4,
    LinkDialogPropertyGraphicsItemType = 5,
    LinkDialogCurveGraphicsItemType = 6
};


class IVW_QTEDITOR_API DialogCurveGraphicsItem : public CurveGraphicsItem {

public:
    DialogCurveGraphicsItem(QPointF startPoint, QPointF endPoint, ivec3 color=ivec3(38,38,38), bool layoutOption=false, bool dragMode=true);
    ~DialogCurveGraphicsItem();
    virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    //override for qgraphicsitem_cast (refer qt documentation)
    enum { Type = UserType + LinkDialogCurveGraphicsItemType };
    int type() const  {return Type; }

};

class LinkDialogPropertyGraphicsItem;
class LinkDialogProcessorGraphicsItem;

class IVW_QTEDITOR_API DialogConnectionGraphicsItem : public DialogCurveGraphicsItem {

public:

    DialogConnectionGraphicsItem(LinkDialogPropertyGraphicsItem* startProperty, LinkDialogPropertyGraphicsItem* endProperty,
                                 PropertyLink* propertyLink, ProcessorLink* processorLink, bool layoutOption=true);
    ~DialogConnectionGraphicsItem();

    virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);

    QRectF boundingRect() const;

    LinkDialogPropertyGraphicsItem* getStartProperty() const { return startPropertyGraphicsItem_; }
 
    LinkDialogPropertyGraphicsItem* getEndProperty() const { return endPropertyGraphicsItem_; }

    void switchDirection();

    bool isBidirectional();
    void initialize();
    void deinitialize();
    void setStartArrowHeadIndex(size_t index) {startArrowHeadIndex_ = index;}
    void setEndArrowHeadIndex(size_t index) {endArrowHeadIndex_ = index;}
    size_t getStartArrowHeadIndex() { return startArrowHeadIndex_;}
    size_t getEndArrowHeadIndex() { return endArrowHeadIndex_;}
    void updateStartEndPoint();
    void updateConnectionDrawing();

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e);

private:
    LinkDialogPropertyGraphicsItem* startPropertyGraphicsItem_;
    LinkDialogPropertyGraphicsItem* endPropertyGraphicsItem_;
    size_t startArrowHeadIndex_;
    size_t endArrowHeadIndex_;
    PropertyLink* propertyLink_;
    ProcessorLink* processorLink_;
};


/*---------------------------------------------------------------------------------------*/

template <typename T>
class IVW_QTEDITOR_API GraphicsItemData : public QGraphicsRectItem {
public:
    GraphicsItemData(T* item=0) : QGraphicsRectItem() {item_ = item;}
    T* getGraphicsItemData() {return item_;}
    void setGraphicsItemData(T* item) {item_ = item;}
private:
    T* item_;
};

class LinkDialogPropertyGraphicsItem;

class IVW_QTEDITOR_API LinkDialogProcessorGraphicsItem : public GraphicsItemData<Processor> {

public:
    LinkDialogProcessorGraphicsItem();
    ~LinkDialogProcessorGraphicsItem();

    void setProcessor(Processor* processor);
    Processor* getProcessor() {return getGraphicsItemData();}

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

class IVW_QTEDITOR_API LinkDialogPropertyGraphicsItem : public GraphicsItemData<Property> {

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

    QPointF calculateArrowCenter(size_t curPort, bool computeRight) const;
    QRectF calculateArrowRect(size_t curPort, bool computeRight) const;
    QRectF calculateArrowRect(DialogConnectionGraphicsItem* cItem, bool computeRight=true) const;
    DialogConnectionGraphicsItem* getArrowConnectionAt(const QPointF pos) const;
    bool isArrowPointedRight(DialogConnectionGraphicsItem* cItem);

    //void addArrow() {arrowCount_++;}
    //int getArrowCount() {return arrowCount_;}
    //void removeArrow() {arrowCount_--;}

    void prepareGeometryChange() {QGraphicsItem::prepareGeometryChange();}

    void addConnectionGraphicsItem(DialogConnectionGraphicsItem*);
    size_t getConnectionGraphicsItemCount() const;
    void removeConnectionGraphicsItem(DialogConnectionGraphicsItem*);

protected:
    virtual void paint(QPainter* p, const QStyleOptionGraphicsItem* options, QWidget* widget);
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:

    LabelGraphicsItem* classLabel_;
    LinkDialogProcessorGraphicsItem* processorGraphicsItem_;
    //int arrowCount_;
    std::vector<DialogConnectionGraphicsItem*> connectionItems_;
};

/*---------------------------------------------------------------------------------------*/


class IVW_QTEDITOR_API LinkDialogGraphicsScene : public QGraphicsScene {
public:
    LinkDialogGraphicsScene(QWidget* parent=0);
    ~LinkDialogGraphicsScene() {}

    template <typename T>
    T* getSceneGraphicsItemAt(const QPointF pos, const Qt::ItemSelectionMode mode= Qt::IntersectsItemShape, Qt::SortOrder order=Qt::DescendingOrder) const {    
        QList<QGraphicsItem*> graphicsItems =items(pos, mode, order);
        if (graphicsItems.size() > 0) {
            for (int i=0; i<graphicsItems.size(); i++) {
                T* graphicsItem = qgraphicsitem_cast<T*>(graphicsItems[i]);
                if (graphicsItem)
                    return graphicsItem;
            }
        }
        return 0;
    }
    
    QGraphicsItem* getPropertyGraphicsItemAt(Property* property);
    void setNetwork(ProcessorNetwork* network) {processorNetwork_ = network;}

    void initScene(std::vector<Processor*> srcProcessorList, std::vector<Processor*> dstProcessorList);
    void removeCurrentPropertyLinks();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* e);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* e);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* e);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e);
    void keyPressEvent(QKeyEvent* keyEvent);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* e);
    void addPropertyLink(LinkDialogPropertyGraphicsItem* outProperty, LinkDialogPropertyGraphicsItem* inProperty);
    void removePropertyLink(DialogConnectionGraphicsItem* propertyLink);
    void cleanupAfterRemoveLink(DialogConnectionGraphicsItem* propertyLink);
    void addPropertyLink(PropertyLink* propertyLink);
    bool isPropertyLinkBidirectional(DialogConnectionGraphicsItem* propertyLink);
    void makePropertyLinkBidirectional(DialogConnectionGraphicsItem* propertyLink, bool isBidirectional);
    void switchPropertyLinkDirection(DialogConnectionGraphicsItem* propertyLink);    
    void initializePorpertyLinkRepresentation(LinkDialogPropertyGraphicsItem* outProperty, LinkDialogPropertyGraphicsItem* inProperty, PropertyLink* propLink);
    void addProcessorsItemsToScene(Processor *prcoessor, int xPosition, int yPosition);
    DialogConnectionGraphicsItem* getConnectionGraphicsItem(LinkDialogPropertyGraphicsItem* , LinkDialogPropertyGraphicsItem*);

private:
    DialogCurveGraphicsItem* linkCurve_;
    LinkDialogPropertyGraphicsItem* startProperty_;
    LinkDialogPropertyGraphicsItem* endProperty_;

    std::vector<LinkDialogProcessorGraphicsItem*> processorGraphicsItems_;
    std::vector<DialogConnectionGraphicsItem*> connectionGraphicsItems_;
    std::vector<DialogConnectionGraphicsItem*> currentConnectionGraphicsItems_;
    
    ProcessorNetwork* processorNetwork_;

    void addConnectionToCurrentList(DialogConnectionGraphicsItem*);
    void removeConnectionFromCurrentList(DialogConnectionGraphicsItem*);

};

/*---------------------------------------------------------------------------------------*/

class IVW_QTEDITOR_API LinkDialogGraphicsView : public QGraphicsView {
public:
    LinkDialogGraphicsView(QWidget* parent=0);
    ~LinkDialogGraphicsView();
    void setDialogScene(LinkDialogGraphicsScene* scene);
protected:
    void mouseDoubleClickEvent(QMouseEvent* e);
private:
    LinkDialogGraphicsScene* scene_;
};

/*---------------------------------------------------------------------------------------*/

class IVW_QTEDITOR_API LinkDialog : public QDialog {
Q_OBJECT
public:
    LinkDialog(std::vector<ProcessorLink*> processorLinks, ProcessorNetwork* network, QWidget* parent);    
    LinkDialog(Processor* src, Processor *dest, ProcessorNetwork* network, QWidget* parent);

private slots:
   void clickedOkayButton();
   void clickedCancelButton();

private:   
    void initDialog();

    LinkDialogGraphicsView* linkDialogView_;
    LinkDialogGraphicsScene* linkDialogScene_;
    QDialogButtonBox* okayCancelbuttonBox_;    
};

/*---------------------------------------------------------------------------------------*/

} //namespace

#endif //IVW_LINKDIALOG_H
