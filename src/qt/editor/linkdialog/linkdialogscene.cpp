/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2014 Inviwo Foundation
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
 * Contact: Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/qt/editor/linkdialog/linkdialogscene.h>
#include <inviwo/qt/editor/linkdialog/linkdialoggraphicsitems.h>
#include <inviwo/qt/editor/linkdialog/linkdialogprocessorgraphicsitems.h>
#include <inviwo/qt/editor/linkdialog/linkdialogpropertygraphicsitems.h>
#include <inviwo/qt/editor/linkdialog/linkdialogcurvegraphicsitems.h>

#include <inviwo/core/network/processornetwork.h>
#include <inviwo/core/network/processornetworkobserver.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/properties/propertyowner.h>
#include <inviwo/core/properties/property.h>
#include <inviwo/core/links/propertylink.h>

#include <QTimeLine>

namespace inviwo {

LinkDialogGraphicsScene::LinkDialogGraphicsScene(QWidget* parent)
    : QGraphicsScene(parent)
    , src_(0)
    , dest_(0)
    , currentScrollSteps_(0)
    , linkCurve_(0)
    , startProperty_(0)
    , endProperty_(0)
    , processorNetwork_(0)
    , expandProperties_(false)
    , mouseOnLeftSide_(false) {

    // The defalt bsp tends to crash...  
    setItemIndexMethod(QGraphicsScene::NoIndex);
}

QGraphicsItem* LinkDialogGraphicsScene::getPropertyGraphicsItemOf(Property* property) {
    LinkDialogPropertyGraphicsItem* graphicsItem = 0;

    for (size_t i = 0; i < processorGraphicsItems_.size(); i++) {
        std::vector<LinkDialogPropertyGraphicsItem*> propertyItems =
            processorGraphicsItems_[i]->getPropertyItemList();

        for (size_t j = 0; j < propertyItems.size(); j++) {
            if (propertyItems[j]->getGraphicsItemData() == property) {
                graphicsItem = propertyItems[j];
                break;
            }
        }

        if (graphicsItem) break;
    }

    if (!graphicsItem) {
        //This is slightly expensive search to do it only if you don't find required item in direct children
        for (size_t i = 0; i < processorGraphicsItems_.size(); i++) {

            std::vector<LinkDialogPropertyGraphicsItem*> propertyItems =
                processorGraphicsItems_[i]->getPropertyItemList();

            for (size_t j = 0; j < propertyItems.size(); j++) {
                std::vector<LinkDialogPropertyGraphicsItem*> subPropertyItems =
                    propertyItems[j]->getSubPropertyItemList(true);

                for (size_t k = 0; k < subPropertyItems.size(); k++) {
                    if ( subPropertyItems[k]->getGraphicsItemData() == property) {
                        graphicsItem = subPropertyItems[k];
                        break;
                    }
                }

                if (graphicsItem) break;
            }

            if (graphicsItem) break;
        }
    }

    return graphicsItem;
}

void LinkDialogGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* e) {

    LinkDialogPropertyGraphicsItem* tempProperty =
        getSceneGraphicsItemAt<LinkDialogPropertyGraphicsItem>(e->scenePos());

    if (!startProperty_ && tempProperty) {
        startProperty_ = tempProperty;

        if (linkCurve_) {
            linkCurve_->hide();
            removeItem(linkCurve_);
            delete linkCurve_;
            linkCurve_ = 0;
        }

        QPointF center = startProperty_->getShortestBoundaryPointTo(e->scenePos());
        linkCurve_ = new DialogCurveGraphicsItem(center, e->scenePos());
        linkCurve_->setZValue(2.0);
        addItem(linkCurve_);
        linkCurve_->show();
        e->accept();
    } else
        QGraphicsScene::mousePressEvent(e);

    return;
}

void LinkDialogGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* e) {

    QPointF pos = e->scenePos();
    if (pos.x() > linkDialogWidth/2 ) {
        mouseOnLeftSide_ = false;
    }
    else {
        mouseOnLeftSide_ = true;
    }

    if (linkCurve_) {
        QPointF center = startProperty_->getShortestBoundaryPointTo(e->scenePos());
        linkCurve_->setStartPoint(center) ;
        linkCurve_->setEndPoint(e->scenePos());
        e->accept();
    } else
        QGraphicsScene::mouseMoveEvent(e);
}

void LinkDialogGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* e) {
    if (linkCurve_) {
        delete linkCurve_;
        linkCurve_ = NULL;
        endProperty_ = getSceneGraphicsItemAt<LinkDialogPropertyGraphicsItem>(e->scenePos());

        if (endProperty_ && (endProperty_!=startProperty_)) {
            Property* sProp = startProperty_->getGraphicsItemData();
            Property* eProp = endProperty_->getGraphicsItemData();

            if (sProp->getOwner()->getProcessor()!=eProp->getOwner()->getProcessor()) {
                bool src2dst = SimpleCondition::canLink(sProp, eProp);
                bool dst2src = SimpleCondition::canLink(eProp, sProp);
                if (src2dst &&dst2src)
                    addPropertyLink(sProp, eProp, true);
                else if (src2dst)
                    addPropertyLink(sProp, eProp, false);
                else if (dst2src)
                    addPropertyLink(eProp, sProp, false);
            }
        }

        startProperty_ = NULL;
        endProperty_ = NULL;
        e->accept();
    } else {
        QGraphicsScene::mouseReleaseEvent(e);
    }

    startProperty_ = NULL;
    endProperty_ = NULL;
}

void LinkDialogGraphicsScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* e) {
    LinkDialogPropertyGraphicsItem* propertyItem =
        getSceneGraphicsItemAt<LinkDialogPropertyGraphicsItem>(e->scenePos());

    if (propertyItem) {
        DialogConnectionGraphicsItem* propertyLink =
            propertyItem->getArrowConnectionAt(e->scenePos());

        if (propertyLink) {
            if (isPropertyLinkBidirectional(propertyLink)) {
                makePropertyLinkBidirectional(propertyLink, false);

                if (propertyLink->getStartProperty() == propertyItem)
                    switchPropertyLinkDirection(propertyLink);
            } else {
                if (propertyLink->getStartProperty() == propertyItem)
                    makePropertyLinkBidirectional(propertyLink, true);
            }
        }
        else {

            bool expand = false;
            if (propertyItem->isExpanded()) expand = true;

            expandOrCollapseLinkedPropertyItems(propertyItem, !expand);
        }

        e->accept();
    } else
        QGraphicsScene::mouseDoubleClickEvent(e);
}


void LinkDialogGraphicsScene::keyPressEvent(QKeyEvent* keyEvent) {
    if (keyEvent->key() == Qt::Key_Delete) {
        QList<QGraphicsItem*> selectedGraphicsItems = selectedItems();

        for (int i=0; i<selectedGraphicsItems.size(); i++) {
            DialogConnectionGraphicsItem* connectionGraphicsItem = qgraphicsitem_cast<DialogConnectionGraphicsItem*>(selectedGraphicsItems[i]);

            if (connectionGraphicsItem) {
                removeConnectionFromCurrentList(connectionGraphicsItem);
                removePropertyLink(connectionGraphicsItem);
            }
        }
    }

    QGraphicsScene::keyPressEvent(keyEvent);
}

void LinkDialogGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent* e) {
    DialogConnectionGraphicsItem* linkGraphicsItem = getSceneGraphicsItemAt<DialogConnectionGraphicsItem>(e->scenePos()) ;

    if (linkGraphicsItem && linkGraphicsItem->getPropertyLink()) {
        QMenu menu;
        QAction* deleteAction = menu.addAction("Delete");
        QAction* biDirectionAction = menu.addAction("BiDirectional");
        biDirectionAction->setCheckable(true);
        QAction* switchDirection = menu.addAction("Switch Direction");

        if (isPropertyLinkBidirectional(linkGraphicsItem))
            biDirectionAction->setChecked(true);
        else
            biDirectionAction->setChecked(false);

        QAction* result = menu.exec(QCursor::pos());

        if (result == deleteAction) {
            removeConnectionFromCurrentList(linkGraphicsItem);
            removePropertyLink(linkGraphicsItem);
        }
        else if (result == biDirectionAction) {
            if (biDirectionAction->isChecked())
                makePropertyLinkBidirectional(linkGraphicsItem, true);
            else
                makePropertyLinkBidirectional(linkGraphicsItem, false);
        }
        else if (result == switchDirection)
            switchPropertyLinkDirection(linkGraphicsItem);
    }
}

void LinkDialogGraphicsScene::wheelEvent(QGraphicsSceneWheelEvent* e) {

    float yIncrement = processorItemHeight*(1.0f/10.0f);
        
    //note:delta can be positive or negative (wheel rotated away from user or towards user)
    int numDegrees = e->delta() / 8;
    int numSteps = numDegrees / 15;
    yIncrement*=numSteps;

    currentScrollSteps_ = numSteps;

    QTimeLine *anim = new QTimeLine(750, this);
    anim->setUpdateInterval(20);
    connect(anim, SIGNAL(valueChanged(qreal)), SLOT(executeTimeLine(qreal)));
    connect(anim, SIGNAL(finished()), SLOT(terminateTimeLine()));
    anim->start();
    e->accept();
    //QGraphicsScene::wheelEvent(e);
}

void LinkDialogGraphicsScene::offsetItems(float yIncrement, bool scrollLeft) {
    //QPointF zoomOffset = allViews[0]->mapToScene(0, yIncrement);
    QPointF scrollOffset = QPointF(0.0f, yIncrement);

    LinkDialogProcessorGraphicsItem* procGraphicsItem=0;
    foreach(procGraphicsItem, processorGraphicsItems_) {
        QPointF pos = procGraphicsItem->scenePos();
        if (scrollLeft && pos.x()>=linkDialogWidth/2) continue;
        if (!scrollLeft && pos.x()<linkDialogWidth/2) continue;
        procGraphicsItem->setPos(pos.x()+scrollOffset.x(), pos.y()+scrollOffset.y());
        std::vector<LinkDialogPropertyGraphicsItem*> propItems = procGraphicsItem->getPropertyItemList();
        for (size_t i=0; i<propItems.size(); i++) {
            propItems[i]->updatePositionBasedOnIndex();
            const std::vector<DialogConnectionGraphicsItem*> connections = propItems[i]->getConnectionGraphicsItems();
            for (size_t j=0; j<connections.size(); j++) {
                connections[j]->updateConnectionDrawing();
            }
        }
    }
}

void LinkDialogGraphicsScene::executeTimeLine(qreal x) {
    float yIncrement = processorItemHeight*(0.09f)*(currentScrollSteps_);
    offsetItems(yIncrement, mouseOnLeftSide_);
}

void LinkDialogGraphicsScene::terminateTimeLine() {
    sender()->~QObject();
}


void LinkDialogGraphicsScene::addPropertyLink(Property* sProp, Property* eProp,
                                              bool bidirectional) {
    LinkDialogPropertyGraphicsItem* startProperty =
        qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemOf(sProp));
    LinkDialogPropertyGraphicsItem* endProperty =
        qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemOf(eProp));
    addPropertyLink(startProperty, endProperty, bidirectional);
    DialogConnectionGraphicsItem* propertyLinkItem =
        getConnectionGraphicsItem(startProperty, endProperty);

    if (bidirectional) makePropertyLinkBidirectional(propertyLinkItem, true);

    addConnectionToCurrentList(propertyLinkItem);
}

int LinkDialogGraphicsScene::currentLinkItemsCount() {
    return static_cast<int>(currentConnectionGraphicsItems_.size());
}

void LinkDialogGraphicsScene::setExpandProperties(bool expand) { 
    if (expand!=expandProperties_) {
        expandProperties_ = expand;
    }
}

void LinkDialogGraphicsScene::expandOrCollapseLinkedProcessorItems(LinkDialogProcessorGraphicsItem* procGraphicsItem, bool collapse) {
    std::vector<LinkDialogPropertyGraphicsItem*> propItems = procGraphicsItem->getPropertyItemList();
    LinkDialogPropertyGraphicsItem* propertyItem=0;
    foreach(propertyItem, propItems) {
        expandOrCollapseLinkedPropertyItems(propertyItem, collapse);
    }
}

void LinkDialogGraphicsScene::expandOrCollapseLinkedPropertyItems(LinkDialogPropertyGraphicsItem* propertyItem, bool expand) {
    if (propertyItem->hasSubProperties()) {

        propertyItem->setAnimate(true);

        if (expand) propertyItem->expand(true);
        else propertyItem->collapse(true);

        std::vector<LinkDialogPropertyGraphicsItem*> subProps = propertyItem->getSubPropertyItemList(true);
        for (size_t i=0; i<subProps.size(); i++) {
            std::vector<Property*> linkedSubProps = processorNetwork_->getLinkedProperties(subProps[i]->getGraphicsItemData());
            for (size_t j=0; j<linkedSubProps.size(); j++) {
                Property* parentProperty = dynamic_cast<Property*>(linkedSubProps[j]->getOwner());
                if (parentProperty) {
                    LinkDialogPropertyGraphicsItem* endP =
                        qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemOf(parentProperty));
                    if(endP){
                        endP->setAnimate(true);
                        if (expand) endP->expand(true);
                        else endP->collapse(true);
                    }
                }
            }
        }

        LinkDialogProcessorGraphicsItem* procGraphicsItem=0;
        foreach(procGraphicsItem, processorGraphicsItems_)
            procGraphicsItem->updatePropertyItemPositions(true);
    }
}

bool LinkDialogGraphicsScene::isPropertyExpanded(Property* property) {
    LinkDialogPropertyGraphicsItem* propItem =
        qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemOf(property));
    if(propItem) return propItem->isExpanded();
    return false;
}

void LinkDialogGraphicsScene::addPropertyLink(PropertyLink* propertyLink) {
    //For adding representations for existing links in the network
    //LogInfo("Adding Property Link.");
    LinkDialogPropertyGraphicsItem* startProperty =  qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemOf(
                propertyLink->getSourceProperty()));
    LinkDialogPropertyGraphicsItem* endProperty =  qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemOf(
                propertyLink->getDestinationProperty()));

    if (startProperty && endProperty)
        initializePorpertyLinkRepresentation(startProperty, endProperty, propertyLink);
}

void LinkDialogGraphicsScene::addPropertyLink(LinkDialogPropertyGraphicsItem* startProperty, LinkDialogPropertyGraphicsItem* endProperty, bool bidirectional) {
    //For adding new links to network and creating representations
    //LogInfo("Adding Property Link.");
    Property* sProp = startProperty->getGraphicsItemData();
    Property* eProp = endProperty->getGraphicsItemData();

    PropertyLink* propertyLink = processorNetwork_->getLink(sProp, eProp);

    if (!propertyLink) {
        propertyLink = processorNetwork_->addLink(sProp, eProp);
    }

    if (propertyLink) 
        initializePorpertyLinkRepresentation(startProperty, endProperty, propertyLink);
}

void LinkDialogGraphicsScene::removeCurrentPropertyLinks() {
    DialogConnectionGraphicsItem* propertyLink=0;
    std::vector<DialogConnectionGraphicsItem*> tempList = currentConnectionGraphicsItems_;
    foreach(propertyLink, currentConnectionGraphicsItems_)
    removeConnectionFromCurrentList(propertyLink);
    foreach(propertyLink, tempList)
    removePropertyLink(propertyLink);
}

void LinkDialogGraphicsScene::removeAllPropertyLinks() {
    DialogConnectionGraphicsItem* propertyLink=0;
    std::vector<DialogConnectionGraphicsItem*> tempList = connectionGraphicsItems_;
    foreach(propertyLink, connectionGraphicsItems_)
    removeConnectionFromCurrentList(propertyLink);
    foreach(propertyLink, tempList)
    removePropertyLink(propertyLink);
}

void LinkDialogGraphicsScene::removePropertyLink(DialogConnectionGraphicsItem* propertyLink) {
    //LogInfo("Removing Property Link.");
    LinkDialogPropertyGraphicsItem* startProperty = propertyLink->getStartProperty();
    LinkDialogPropertyGraphicsItem* endProperty = propertyLink->getEndProperty();

    PropertyLink* link = processorNetwork_->getLink(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());
    if (link) {
        propertyLink->hide();
        connectionGraphicsItems_.erase(std::remove(connectionGraphicsItems_.begin(), connectionGraphicsItems_.end(), propertyLink),
            connectionGraphicsItems_.end());
        removeItem(propertyLink);
        propertyLink->deinitialize();
        cleanupAfterRemoveLink(propertyLink);
        delete propertyLink;
        processorNetwork_->removeLink(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());
        processorNetwork_->removeLink(endProperty->getGraphicsItemData(), startProperty->getGraphicsItemData());
        startProperty->prepareGeometryChange();
        endProperty->prepareGeometryChange();
    }
}

void LinkDialogGraphicsScene::cleanupAfterRemoveLink(DialogConnectionGraphicsItem* propertyLink) {
    LinkDialogPropertyGraphicsItem* startProperty = propertyLink->getStartProperty();
    LinkDialogPropertyGraphicsItem* endProperty = propertyLink->getEndProperty();

    //re-assign connection ids
    for (size_t i=0; i<connectionGraphicsItems_.size(); i++) {
        if (connectionGraphicsItems_[i] == propertyLink) continue;
        else if (connectionGraphicsItems_[i]->getStartProperty() == startProperty) {
            size_t index = connectionGraphicsItems_[i]->getStartArrowHeadIndex();

            if (index > propertyLink->getStartArrowHeadIndex())
                connectionGraphicsItems_[i]->setStartArrowHeadIndex(index-1);
        }
        else if (connectionGraphicsItems_[i]->getEndProperty() == startProperty) {
            size_t index = connectionGraphicsItems_[i]->getEndArrowHeadIndex();

            if (index > propertyLink->getStartArrowHeadIndex())
                connectionGraphicsItems_[i]->setEndArrowHeadIndex(index-1);
        }
    }

    for (size_t i=0; i<connectionGraphicsItems_.size(); i++) {
        if (connectionGraphicsItems_[i] == propertyLink) continue;
        else if (connectionGraphicsItems_[i]->getEndProperty() == endProperty) {
            size_t index = connectionGraphicsItems_[i]->getEndArrowHeadIndex();

            if (index > propertyLink->getEndArrowHeadIndex())
                connectionGraphicsItems_[i]->setEndArrowHeadIndex(index-1);
        }
        else if (connectionGraphicsItems_[i]->getStartProperty() == endProperty) {
            size_t index = connectionGraphicsItems_[i]->getStartArrowHeadIndex();

            if (index > propertyLink->getEndArrowHeadIndex())
                connectionGraphicsItems_[i]->setStartArrowHeadIndex(index-1);
        }
    }

    for (size_t i=0; i<connectionGraphicsItems_.size(); i++)
        connectionGraphicsItems_[i]->updateConnectionDrawing();
}

bool LinkDialogGraphicsScene::isPropertyLinkBidirectional(DialogConnectionGraphicsItem* propertyLink) {
    LinkDialogPropertyGraphicsItem* startProperty = propertyLink->getStartProperty();
    LinkDialogPropertyGraphicsItem* endProperty = propertyLink->getEndProperty();

    return processorNetwork_->isLinkedBidirectional(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());
}

void LinkDialogGraphicsScene::makePropertyLinkBidirectional(DialogConnectionGraphicsItem* propertyLink, bool isBidirectional) {
    LinkDialogPropertyGraphicsItem* startProperty = propertyLink->getStartProperty();
    LinkDialogPropertyGraphicsItem* endProperty = propertyLink->getEndProperty();

    PropertyLink* propLink = processorNetwork_->getLink(endProperty->getGraphicsItemData(), startProperty->getGraphicsItemData());
    if (isBidirectional) {
        if (!propLink) processorNetwork_->addLink(endProperty->getGraphicsItemData(), startProperty->getGraphicsItemData());
    }
    else {
        if (propLink) processorNetwork_->removeLink(endProperty->getGraphicsItemData(), startProperty->getGraphicsItemData());
        }

    propertyLink->updateConnectionDrawing();
}

void LinkDialogGraphicsScene::switchPropertyLinkDirection(DialogConnectionGraphicsItem* propertyLink) {
    if (!propertyLink->isBidirectional()) {
        propertyLink->switchDirection();
        propertyLink->updateConnectionDrawing();
    }

    update();
}

void LinkDialogGraphicsScene::addConnectionToCurrentList(DialogConnectionGraphicsItem* propertyLink) {
    if (std::find(currentConnectionGraphicsItems_.begin(), currentConnectionGraphicsItems_.end(),
                  propertyLink)==currentConnectionGraphicsItems_.end())
        currentConnectionGraphicsItems_.push_back(propertyLink);
}

void LinkDialogGraphicsScene::removeConnectionFromCurrentList(DialogConnectionGraphicsItem* propertyLink) {
    currentConnectionGraphicsItems_.erase(std::remove(currentConnectionGraphicsItems_.begin(), currentConnectionGraphicsItems_.end(),
                                          propertyLink),
                                          currentConnectionGraphicsItems_.end());
}

DialogConnectionGraphicsItem* LinkDialogGraphicsScene::getConnectionGraphicsItem(LinkDialogPropertyGraphicsItem* outProperty,
        LinkDialogPropertyGraphicsItem* inProperty) {
    for (size_t i=0; i<connectionGraphicsItems_.size(); i++) {
        if ((connectionGraphicsItems_[i]->getStartProperty() == outProperty && connectionGraphicsItems_[i]->getEndProperty() == inProperty) ||
            (connectionGraphicsItems_[i]->getStartProperty() == inProperty && connectionGraphicsItems_[i]->getEndProperty() == outProperty))
            return connectionGraphicsItems_[i];
    }

    return 0;
}

void LinkDialogGraphicsScene::initializePorpertyLinkRepresentation(LinkDialogPropertyGraphicsItem* outProperty,
        LinkDialogPropertyGraphicsItem* inProperty,
        PropertyLink* propertyLink) {
    
    DialogConnectionGraphicsItem* cItem = 0;
    cItem = getConnectionGraphicsItem(outProperty, inProperty);

    if (!cItem) {
        cItem = new DialogConnectionGraphicsItem(outProperty, inProperty, propertyLink);
        addItem(cItem);
        connectionGraphicsItems_.push_back(cItem);
    }

    cItem->show();

    for (size_t i=0; i<connectionGraphicsItems_.size(); i++)
        connectionGraphicsItems_[i]->updateConnectionDrawing();
}


void LinkDialogGraphicsScene::initScene(Processor* srcProcessor,
                                        Processor* dstProcessor) {

    clearSceneRepresentations();

    int xPosition = linkDialogWidth / 4;
    int yPosition = processorItemHeight;
    int xIncrement = linkDialogWidth / 2;
    int yIncrement = processorItemHeight;

    
    addProcessorsItemsToScene(srcProcessor, xPosition, yPosition);
    yPosition += yIncrement;
    

    xPosition += xIncrement;
    yPosition = processorItemHeight;

    
    addProcessorsItemsToScene(dstProcessor, xPosition, yPosition);
    yPosition += yIncrement;    

    //add links
    std::vector<PropertyLink*> networkLinks =
        processorNetwork_->getLinksBetweenProcessors(srcProcessor, dstProcessor);

    std::vector<PropertyLink*> pairList;
    PropertyLink* pair = 0;

    std::vector<PropertyLink*> propertyLinks;  // uni directional links only
    std::vector<bool> directionFlags;

    for (size_t j = 0; j < networkLinks.size(); j++){ 
        if (std::find(pairList.begin(), pairList.end(), networkLinks[j]) == pairList.end()) {
            propertyLinks.push_back(networkLinks[j]);
            Property* srcProp = networkLinks[j]->getSourceProperty();
            Property* dstProp = networkLinks[j]->getDestinationProperty();
            bool bidirectional = processorNetwork_->isLinkedBidirectional(srcProp, dstProp);

            if (bidirectional) {
                pair = processorNetwork_->getLink(networkLinks[j]->getDestinationProperty(),networkLinks[j]->getSourceProperty());
                pairList.push_back(pair);
            }
            directionFlags.push_back(bidirectional);
        }
    }

    pairList.clear();
    for (size_t j = 0; j < propertyLinks.size(); j++) {
        Property* srcProp = propertyLinks[j]->getSourceProperty();
        Property* dstProp = propertyLinks[j]->getDestinationProperty();
        bool bidirectional = directionFlags[j];
        addPropertyLink(srcProp, dstProp, bidirectional);
    }

    LinkDialogProcessorGraphicsItem* procGraphicsItem=0;
    foreach(procGraphicsItem, processorGraphicsItems_)
        expandOrCollapseLinkedProcessorItems(procGraphicsItem, expandProperties_);

    currentConnectionGraphicsItems_.clear();
}

void LinkDialogGraphicsScene::clearSceneRepresentations() {
    processorGraphicsItems_.clear();
    connectionGraphicsItems_.clear();
    currentConnectionGraphicsItems_.clear();
    clear();
}

void LinkDialogGraphicsScene::addProcessorsItemsToScene(Processor* processor, int xPosition, int yPosition) {
    LinkDialogProcessorGraphicsItem* procGraphicsItem = new LinkDialogProcessorGraphicsItem();
    processorGraphicsItems_.push_back(procGraphicsItem);

    //There is only one view which owns this scene
    //TODO: Any other better ways to map to current scene.
    //Verify if procGraphicsItem can map to current scene
    QList<QGraphicsView*> allViews = views();

    if (!allViews.size()) {
        //should not come here
        return;
    }

    procGraphicsItem->setPos(allViews[0]->mapToScene(xPosition, yPosition));
    procGraphicsItem->setProcessor(processor, expandProperties_);
    addItem(procGraphicsItem);
    procGraphicsItem->show();

    std::vector<LinkDialogPropertyGraphicsItem*> propItems = procGraphicsItem->getPropertyItemList();
    for (size_t i=0; i<propItems.size(); i++)
        addItem(propItems[i]);
}

void LinkDialogGraphicsScene::onProcessorNetworkDidAddLink(PropertyLink* propertyLink) {

}

void LinkDialogGraphicsScene::onProcessorNetworkDidRemoveLink(PropertyLink* propertyLink) {

}

} //namespace