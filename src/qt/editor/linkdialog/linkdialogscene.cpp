/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2012-2014 Inviwo Foundation
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
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

#include <inviwo/qt/editor/linkdialog/linkdialogscene.h>
#include <inviwo/qt/editor/linkdialog/linkdialoggraphicsitems.h>
#include <inviwo/qt/editor/linkdialog/linkdialogprocessorgraphicsitems.h>
#include <inviwo/qt/editor/linkdialog/linkdialogpropertygraphicsitems.h>
#include <inviwo/qt/editor/linkdialog/linkdialogcurvegraphicsitems.h>

namespace inviwo {

LinkDialogGraphicsScene::LinkDialogGraphicsScene(QWidget* parent)
    : QGraphicsScene(parent)
    , src_(0)
    , dest_(0)
    , linkCurve_(0)
    , startProperty_(0)
    , endProperty_(0)
    , processorNetwork_(0)
    , expandProperties_(false) {

    // The defalt bsp tends to crash...  
    setItemIndexMethod(QGraphicsScene::NoIndex);
}

QGraphicsItem* LinkDialogGraphicsScene::getPropertyGraphicsItemAt(Property* property) {
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
            bool src2dst = SimpleCondition::canLink(sProp, eProp);
            bool dst2src = SimpleCondition::canLink(eProp, sProp);
            if (src2dst &&dst2src)
                addPropertyLink(sProp, eProp, true);
            else if (src2dst)
                addPropertyLink(sProp, eProp, false);
            else if (dst2src)
                addPropertyLink(eProp, sProp, false);
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

        e->accept();
    } else
        QGraphicsScene::mouseDoubleClickEvent(e);
}

void LinkDialogGraphicsScene::addPropertyLink(Property* sProp, Property* eProp,
                                              bool bidirectional) {
    LinkDialogPropertyGraphicsItem* startProperty =
        qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemAt(sProp));
    LinkDialogPropertyGraphicsItem* endProperty =
        qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemAt(eProp));
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

void LinkDialogGraphicsScene::addPropertyLink(PropertyLink* propertyLink) {
    //For adding representations for existing links in the network
    //LogInfo("Adding Property Link.");
    LinkDialogPropertyGraphicsItem* startProperty =  qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemAt(
                propertyLink->getSourceProperty()));
    LinkDialogPropertyGraphicsItem* endProperty =  qgraphicsitem_cast<LinkDialogPropertyGraphicsItem*>(getPropertyGraphicsItemAt(
                propertyLink->getDestinationProperty()));

    if (startProperty && endProperty)
        initializePorpertyLinkRepresentation(startProperty, endProperty, propertyLink);
}

void LinkDialogGraphicsScene::addPropertyLink(LinkDialogPropertyGraphicsItem* startProperty, LinkDialogPropertyGraphicsItem* endProperty, bool bidirectional) {
    //For adding new links to network and creating representations
    //LogInfo("Adding Property Link.");
    Property* sProp = startProperty->getGraphicsItemData();
    Property* eProp = endProperty->getGraphicsItemData();

    ///////////////////////////////////////////////////////////////////////
    //TODO: ProcessorLinks are Deprecated. To be removed

    /*
    PropertyOwner* startProcessor = src_;
    PropertyOwner* endProcessor = dest_;
    ProcessorLink* processorLink = processorNetwork_->getLink(startProcessor, endProcessor);

    if (processorLink && !processorLink->isLinked(sProp, eProp) && !processorLink->isLinked(eProp, sProp)) {
        processorLink->addPropertyLinks(sProp, eProp, startProcessor, endProcessor);
        PropertyLink* propertyLink = processorLink->getPropertyLink(sProp, eProp);

        if (propertyLink) initializePorpertyLinkRepresentation(startProperty, endProperty, propertyLink);
    }
    */

    if (!processorNetwork_->getLink(sProp, eProp)) {
        PropertyLink* propertyLink = processorNetwork_->addLink(sProp, eProp);
        if (propertyLink) initializePorpertyLinkRepresentation(startProperty, endProperty, propertyLink);
    }
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

    ///////////////////////////////////////////////////////////////////////
    //TODO: ProcessorLinks are Deprecated. To be removed

    /*
    PropertyOwner* startProcessor = src_;
    PropertyOwner* endProcessor = dest_;
    ProcessorLink* processorLink = processorNetwork_->getLink(startProcessor, endProcessor);

    if (processorLink->isLinked(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData())) {
        propertyLink->hide();
        connectionGraphicsItems_.erase(std::remove(connectionGraphicsItems_.begin(), connectionGraphicsItems_.end(), propertyLink),
                                       connectionGraphicsItems_.end());
        removeItem(propertyLink);
        propertyLink->deinitialize();
        cleanupAfterRemoveLink(propertyLink);
        delete propertyLink;
        processorLink->removePropertyLinks(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());
        startProperty->prepareGeometryChange();
        endProperty->prepareGeometryChange();
    }


    ProcessorLink* processorLink = processorNetwork_->getProcessorLink(src_, dest_);
    if (processorLink) {
        processorLink->removePropertyLinks(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());
        processorLink->removePropertyLinks(endProperty->getGraphicsItemData(), startProperty->getGraphicsItemData());
    }
    */


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

    ///////////////////////////////////////////////////////////////////////
    //TODO: ProcessorLinks are Deprecated. To be removed

    /*
    PropertyOwner* startProcessor = src_;
    PropertyOwner* endProcessor = dest_;
    ProcessorLink* processorLink = processorNetwork_->getLink(startProcessor, endProcessor);
    PropertyLink* propLink = processorLink->getPropertyLink(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());

    if (isBidirectional) {
        if (processorLink->getBidirectionalPair(propLink)==0)
            processorLink->addPropertyLinks(endProperty->getGraphicsItemData(), startProperty->getGraphicsItemData(), endProcessor, startProcessor);
    }
    else {
        if (processorLink->getBidirectionalPair(propLink))
            processorLink->removeBidirectionalPair(startProperty->getGraphicsItemData(), endProperty->getGraphicsItemData());
    }
    */

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

    ///////////////////////////////////////////////////////////////////////
    //TODO: ProcessorLinks are Deprecated. To be removed

    /*
    PropertyOwner* startProcessor = src_;
    PropertyOwner* endProcessor = dest_;
    ProcessorLink* processorLink = processorNetwork_->getLink(startProcessor, endProcessor);
    DialogConnectionGraphicsItem* cItem = new DialogConnectionGraphicsItem(outProperty, inProperty, propertyLink, processorLink);
    addItem(cItem);
    cItem->show();
    connectionGraphicsItems_.push_back(cItem);

    for (size_t i=0; i<connectionGraphicsItems_.size(); i++)
        connectionGraphicsItems_[i]->updateConnectionDrawing();
    */

    DialogConnectionGraphicsItem* cItem = new DialogConnectionGraphicsItem(outProperty, inProperty, propertyLink);
    addItem(cItem);
    cItem->show();
    connectionGraphicsItems_.push_back(cItem);

    for (size_t i=0; i<connectionGraphicsItems_.size(); i++)
        connectionGraphicsItems_[i]->updateConnectionDrawing();
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
    


    std::vector<PropertyLink*> networkLinks =
        processorNetwork_->getLinksBetweenProcessors(srcProcessor, dstProcessor);

    std::vector<PropertyLink*> pairList;
    PropertyLink* pair = 0;

    std::vector<PropertyLink*> propertyLinks;  // uni directional links only

    for (size_t j = 0; j < networkLinks.size(); j++) {
        if (std::find(pairList.begin(), pairList.end(), networkLinks[j]) == pairList.end()) {
            propertyLinks.push_back(networkLinks[j]);
            if (processorNetwork_->isLinkedBidirectional(
                    networkLinks[j]->getSourceProperty(),
                    networkLinks[j]->getDestinationProperty())) {
                pair = processorNetwork_->getLink(networkLinks[j]->getDestinationProperty(),networkLinks[j]->getSourceProperty());
                pairList.push_back(pair);
            }
        }
    }

    pairList.clear();
    for (size_t j = 0; j < propertyLinks.size(); j++) {
        if (std::find(pairList.begin(), pairList.end(), propertyLinks[j]) != pairList.end()) 
            continue;

        CompositeProperty* compositeSrcProperty =
            IS_COMPOSITE_PROPERTY(propertyLinks[j]->getSourceProperty());
        CompositeProperty* compositeDstProperty =
            IS_COMPOSITE_PROPERTY(propertyLinks[j]->getDestinationProperty());

        if (compositeSrcProperty && compositeDstProperty && expandProperties_) {
            // LogWarn("Removing Composite Property Link. Adding Sub-Property Links")
            bool bidirectional = processorNetwork_->isLinkedBidirectional(
                propertyLinks[j]->getSourceProperty(),
                propertyLinks[j]->getDestinationProperty());

            // remove composite property link and add sub property links
            // TODO: Recursive composite properties yet to be detected here

            std::vector<Property*> srcProperties = compositeSrcProperty->getProperties();
            std::vector<Property*> dstProperties = compositeDstProperty->getProperties();

            Property* s = propertyLinks[j]->getSourceProperty();
            Property* d = propertyLinks[j]->getDestinationProperty();
            processorNetwork_->removeLink(s, d);
            processorNetwork_->removeLink(d, s);

            // Two different sub-properties can be linked which is not allowed now.
            if (srcProperties.size() == dstProperties.size()) {
                for (size_t k = 0; k < srcProperties.size(); k++)
                    addPropertyLink(srcProperties[k], dstProperties[k], bidirectional);
            } else {
                // TODO: Perform auto link?
                LogWarn("Unable to link compsite sub-properties")
            }
        } else {
            bool isSubProperty = IS_SUB_PROPERTY(propertyLinks[j]->getSourceProperty());

            if (isSubProperty && !expandProperties_) {
                bool isBirectional = false;

                Property* compsrc = getParentCompositeProperty(
                    propertyLinks[j]->getSourceProperty(), srcProcessor);
                Property* compdst = getParentCompositeProperty(
                    propertyLinks[j]->getDestinationProperty(), dstProcessor);

                if (compsrc && compdst) {
                    CompositeProperty* compositeSrcProperty =
                        IS_COMPOSITE_PROPERTY(compsrc);
                    CompositeProperty* compositeDstProperty =
                        IS_COMPOSITE_PROPERTY(compdst);

                    std::vector<Property*> srcProperties =
                        compositeSrcProperty->getProperties();
                    std::vector<Property*> dstProperties =
                        compositeDstProperty->getProperties();

                    if (srcProperties.size() == dstProperties.size()) {
                        // LogWarn("Remove Sub-Property Links. Add Composite Property Link")
                        for (size_t k = 0; k < srcProperties.size(); k++) {
                            if (srcProperties[k] == propertyLinks[j]->getSourceProperty() &&
                                dstProperties[k] ==
                                    propertyLinks[j]->getDestinationProperty()) {
                                isBirectional = processorNetwork_->isLinkedBidirectional(
                                    srcProperties[k], dstProperties[k]);

                                processorNetwork_->removeLink(srcProperties[k],
                                                                dstProperties[k]);
                                if (isBirectional)
                                    processorNetwork_->removeLink(dstProperties[k],
                                                                    srcProperties[k]);
                            }
                        }

                        if (!processorNetwork_->getLink(compsrc, compdst)) {
                            addPropertyLink(compositeSrcProperty, compositeDstProperty,
                                            isBirectional);
                            if (processorNetwork_->isLinkedBidirectional(
                                compositeSrcProperty, compositeDstProperty)){
                                pairList.push_back(pair);
                            }
                        }
                    }
                }
            } else {
                // LogWarn("Just adding the sub-properties as it is")
                addPropertyLink(propertyLinks[j]);
                if (processorNetwork_->isLinkedBidirectional(
                    propertyLinks[j]->getSourceProperty(),
                    propertyLinks[j]->getDestinationProperty())) {
                    pairList.push_back(propertyLinks[j]);
                }
            }
        }
    }


    currentConnectionGraphicsItems_.clear();
}


Property* LinkDialogGraphicsScene::getParentCompositeProperty(Property* subProperty, Processor* processor) {
    //uses recursion
    std::vector<Property*> properties = processor->getProperties();
    std::vector<CompositeProperty*> compositeProperties;

    for (size_t i=0; i<properties.size(); i++) {
        if (properties[i] == subProperty) return NULL; //not a composite property
        CompositeProperty* comp = IS_COMPOSITE_PROPERTY(properties[i]);
        if (comp)
            compositeProperties.push_back(comp);
    }

    while(compositeProperties.size()) {
        std::vector<CompositeProperty*> newComposites;
        for (size_t i=0; i<compositeProperties.size(); i++) {
            CompositeProperty *comp = compositeProperties[i];
            std::vector<Property*> p = comp->getProperties();
            for (size_t j=0; j<p.size(); j++) {
                if (p[j]==subProperty) return comp;

                CompositeProperty* c = IS_COMPOSITE_PROPERTY(p[j]);
                if (c) {
                    newComposites.push_back(c);
                }
            }
        }
        compositeProperties = newComposites;
    };

    return NULL;
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
    //procGraphicsItem->setPos(xPosition, yPosition);
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

    for (size_t i=0; i<propItems.size(); i++) {
        addItem(propItems[i]);
        propItems[i]->show();
        propItems[i]->updatePositionBasedOnProcessor(expandProperties_);
    }
}

void LinkDialogGraphicsScene::updatePropertyItemsOfAllProcessors() {
    //make sure link items are handled before calling this function
    LinkDialogProcessorGraphicsItem* procGraphicsItem;
    foreach(procGraphicsItem, processorGraphicsItems_) {
        Processor* processor = procGraphicsItem->getProcessor();
        {
            std::vector<LinkDialogPropertyGraphicsItem*> propItems = procGraphicsItem->getPropertyItemList();
            for (size_t i=0; i<propItems.size(); i++) {
                propItems[i]->hide();
                removeItem(propItems[i]);
            }

            procGraphicsItem->setProcessor(processor, expandProperties_);
            propItems = procGraphicsItem->getPropertyItemList();
            for (size_t i=0; i<propItems.size(); i++) {
                addItem(propItems[i]);
                propItems[i]->show();
                propItems[i]->updatePositionBasedOnProcessor(expandProperties_);
            }
        }
    }
}

} //namespace