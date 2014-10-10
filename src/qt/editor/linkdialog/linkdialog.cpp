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

#include <inviwo/qt/editor/linkdialog/linkdialog.h>
#include <inviwo/qt/editor/linkdialog/linkdialoggraphicsitems.h>
#include <inviwo/qt/editor/linkdialog/linkdialogscene.h>
#include <inviwo/qt/editor/linkdialog/linkdialogview.h>
#include <inviwo/core/processors/processor.h>
#include <inviwo/core/common/inviwoapplication.h>

namespace inviwo {

LinkDialog::LinkDialog(QWidget* parent) : InviwoDockWidget("Edit Processor Link Dialog", parent) {
    IVW_UNUSED_PARAM(parent);
    //Handle multiple processor links here
}

LinkDialog::~LinkDialog() {}

LinkDialog::LinkDialog(Processor* src, Processor* dest, QWidget* parent) :InviwoDockWidget("Edit Processor Link Dialog", parent) {
    src_ = src;
    dest_ = dest;
    initDialogLayout();
    linkDialogScene_->setNetwork(InviwoApplication::getPtr()->getProcessorNetwork()); //Network is required to add property links created in dialog (or remove )
    linkDialogScene_->setExpandProperties(EXPAND_SUB_PROPERTIES_BY_DEFAULT);
    linkDialogScene_->initScene(src, dest);
}

void LinkDialog::initDialogLayout() {
    setFloating(true);
    setWindowModality (Qt::ApplicationModal);
    /*
    //Custom title bar for consistency with other docked widgets
    QString style("padding-top: 5px; \
                   padding-left: 5px; \
                   border-radius: 3px; \
                   background: qlineargradient(x1:0, y1:0, x2:0, y2:1,stop:0 #888888, stop:0.4 #555555, stop:0.5 #000000, stop:1 #111111);"
                   );
    
     QFrame* titleBar = new QFrame();
     QVBoxLayout* lableLayout = new QVBoxLayout(titleBar);
     QLabel *titleText = new QLabel("Edit Processor Link Dialog");
     titleText->setStyleSheet(style);
     lableLayout->addWidget(titleText);
     setTitleBarWidget(titleBar);
     //titleBarWidget()->setStyleSheet(style);
     setWindowFlags(Qt::Window | Qt::FramelessWindowHint);*/
    
    setObjectName("LinkDialogWidget");
    //setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setAllowedAreas(Qt::NoDockWidgetArea);
    QFrame* frame = new QFrame();

    QSize rSize(linkDialogWidth, linkDialogHeight);
    setFixedSize(rSize);
    QVBoxLayout* mainLayout = new QVBoxLayout(frame);
    linkDialogView_ = new LinkDialogGraphicsView(this);
    linkDialogScene_ = new LinkDialogGraphicsScene(this);
    linkDialogScene_->src_ = src_;
    linkDialogScene_->dest_ = dest_;
    linkDialogView_->setDialogScene(linkDialogScene_);
    linkDialogView_->setSceneRect(0,0,rSize.width(), rSize.height()*5);
    linkDialogView_->fitInView(linkDialogView_->rect());
    linkDialogView_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mainLayout->addWidget(linkDialogView_);
    QHBoxLayout* commonButtonLayout = new QHBoxLayout;
    //auto link button
    QHBoxLayout* autoLinkPushButtonLayout = new QHBoxLayout;
    autoLinkPushButtonLayout->setAlignment(Qt::AlignLeft);
    //qt documentation
    //auto link button
    autoLinkPushButton_ = new QPushButton("AutoLink", this);
    connect(autoLinkPushButton_, SIGNAL(clicked()), this, SLOT(clickedAutoLinkPushButton()));
    autoLinkPushButtonLayout->addWidget(autoLinkPushButton_, 10);
    //checkable combo box
    std::vector<std::string> options;
    options.push_back(SimpleCondition::conditionName());
    options.push_back(PartiallyMatchingIdCondition::conditionName());
    autoLinkOptions_ = new CheckableQComboBox(this,"AutoLink Filter", options);
    autoLinkPushButtonLayout->addWidget(autoLinkOptions_, 20);
    //delete button
    deleteAllLinkPushButton_ = new QPushButton("Delete All", this);
    connect(deleteAllLinkPushButton_, SIGNAL(clicked()), this, SLOT(clickedDeleteAllLinksPushButton()));
    autoLinkPushButtonLayout->addWidget(deleteAllLinkPushButton_, 10);
    //expand composite
    expandCompositeOption_ = new QCheckBox("Expand All Properties", this);
    expandCompositeOption_->setChecked(EXPAND_SUB_PROPERTIES_BY_DEFAULT);
    autoLinkPushButtonLayout->addWidget(expandCompositeOption_, 10);
    connect(expandCompositeOption_, SIGNAL(toggled(bool)), this, SLOT(expandCompositeProperties(bool)));
    commonButtonLayout->addLayout(autoLinkPushButtonLayout);

    //okay cancel button
    QHBoxLayout* okayCancelButtonLayout = new QHBoxLayout;
    okayCancelButtonLayout->setAlignment(Qt::AlignRight);
    //qt documentation
    okayCancelbuttonBox_ = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(okayCancelbuttonBox_, SIGNAL(accepted()), this, SLOT(clickedOkayButton()));
    connect(okayCancelbuttonBox_, SIGNAL(rejected()), this, SLOT(clickedCancelButton()));
    okayCancelButtonLayout->addWidget(okayCancelbuttonBox_);
    commonButtonLayout->addLayout(okayCancelButtonLayout);
    mainLayout->addLayout(commonButtonLayout);
    setWidget(frame);
}

void LinkDialog::clickedOkayButton() {
    hide();
    eventLoop_.quit();
}

void LinkDialog::updateProcessorLinks() {}

void LinkDialog::clickedCancelButton() {
    linkDialogScene_->removeCurrentPropertyLinks();
    //accept();
    updateProcessorLinks();
    hide();
    eventLoop_.quit();
}

void LinkDialog::closeEvent ( QCloseEvent * event ) {
   eventLoop_.quit();
}

void LinkDialog::setValidationLevelOfSubPropertiesOfCompositeProperties(Property* property, PropertyOwner::InvalidationLevel invalidationLevel) {
    CompositeProperty* compositeProperty = IS_COMPOSITE_PROPERTY(property);
    if (!compositeProperty) return;
    std::vector<Property*> subProps = compositeProperty->getProperties();
    for (size_t i=0; i<subProps.size(); i++) {
        subProps[i]->setInvalidationLevel(invalidationLevel);
    }
}

void LinkDialog::clickedAutoLinkPushButton() {
    std::vector<Property*> srcProperties = src_->getProperties();
    std::vector<Property*> dstProperties = dest_->getProperties();
    int selectedTypes = (int) NoLinkCondition;
    std::vector<std::string> selectedConditons = autoLinkOptions_->getCheckedItems();

    for (size_t i=0; i<selectedConditons.size(); i++) {
        if (selectedConditons[i] == SimpleCondition::conditionName())
            selectedTypes|=SimpleCondition::conditionType();

        if (selectedConditons[i] == PartiallyMatchingIdCondition::conditionName())
            selectedTypes|=PartiallyMatchingIdCondition::conditionType();
    }

    for (size_t i=0; i<srcProperties.size(); i++) {
        for (size_t j=0; j<dstProperties.size(); j++) {

            if (expandCompositeOption_->isChecked()) {
                if (AutoLinker::canLink(srcProperties[i], dstProperties[j], (LinkingConditions) selectedTypes)) {
                    CompositeProperty* compSrc = IS_COMPOSITE_PROPERTY(srcProperties[i]);
                    CompositeProperty* compDst = IS_COMPOSITE_PROPERTY(dstProperties[j]);
                    if ( compSrc && compDst) {
                        //If composite property then try to link sub-properties only
                        std::vector<Property*> s = compSrc->getProperties();
                        std::vector<Property*> d = compDst->getProperties();
                        for (size_t ii=0; ii<s.size(); ii++) {
                            for (size_t jj=0; jj<d.size(); jj++) {
                                if (AutoLinker::canLink(s[ii], d[jj], (LinkingConditions) selectedTypes)) {
                                    linkDialogScene_->addPropertyLink(s[ii], d[jj], true);
                                }
                            }
                        }
                    }
                }
            } else {
                if (AutoLinker::canLink(srcProperties[i], dstProperties[j], (LinkingConditions) selectedTypes))
                    linkDialogScene_->addPropertyLink(srcProperties[i], dstProperties[j], true);
            }
        }
    }
}

void LinkDialog::clickedDeleteAllLinksPushButton() {
    linkDialogScene_->removeAllPropertyLinks();
}

void LinkDialog::expandCompositeProperties(bool expand) {
    linkDialogScene_->setExpandProperties(expand);
    initDialog(src_, dest_);
}

void LinkDialog::initDialog(Processor* src, Processor* dest) {
    linkDialogScene_->clearSceneRepresentations();
    QSize rSize(linkDialogWidth, linkDialogHeight);
    linkDialogView_->setSceneRect(0,0,rSize.width(), rSize.height()*5);
    linkDialogView_->fitInView(linkDialogView_->rect());
    linkDialogView_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    src_ = src;
    dest_ = dest;
    linkDialogScene_->initScene(src_, dest_);
}

int LinkDialog::exec() {
    eventLoop_.exit();
    show();
    //connect(this, SIGNAL(destroy()), &eventLoop_, SLOT(quit()));
    return eventLoop_.exec();
}


CheckableQComboBox::CheckableQComboBox(QWidget *parent , std::string widgetName, std::vector<std::string> options) : QComboBox(parent),widgetName_(widgetName) {
    setEditable(true);
    lineEdit()->setReadOnly(true);
    stdandardModel_ = new QStandardItemModel(static_cast<int>(options.size()),1);

    for (size_t i=0; i<options.size(); i++) {
        QStandardItem* item = new QStandardItem(QString(options[i].c_str()));
        item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        item->setData(Qt::Checked, Qt::CheckStateRole);
        stdandardModel_->setItem(static_cast<int>(i), 0, item);
        standardItems_.push_back(item);
    }

    setModel(stdandardModel_);
    lineEdit()->setText(QString(widgetName_.c_str()));
    connect(stdandardModel_, SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(onAutoLinkOptionChecked(const QModelIndex&,
            const QModelIndex&)));
}

CheckableQComboBox::~CheckableQComboBox(){
    delete stdandardModel_;
}

bool CheckableQComboBox::isItemChecked(int i) {
    if (i>(int)standardItems_.size())
        return false;

    QStandardItem* item = standardItems_[i];

    if (item->checkState() == Qt::Checked)
        return true;

    return false;
}

std::vector<std::string> CheckableQComboBox::getCheckedItems() {
    std::vector<std::string> checkedItemString;

    for (size_t i=0; i<standardItems_.size(); i++)
        if (isItemChecked(static_cast<int>(i)))
            checkedItemString.push_back(standardItems_[i]->text().toLocal8Bit().constData());

    return checkedItemString;
}

void CheckableQComboBox::onAutoLinkOptionChecked(const QModelIndex& tl, const QModelIndex& br) {
    if (isItemChecked(tl.row())) {
        //do some maintenance stuff here
    }

    lineEdit()->setText(QString(widgetName_.c_str()));
}

} //namespace