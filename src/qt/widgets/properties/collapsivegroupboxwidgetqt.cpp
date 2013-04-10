#include <inviwo/qt/widgets/properties/collapsivegroupboxwidgetqt.h>

#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>

namespace inviwo {

    CollapsiveGroupBoxWidgetQt::CollapsiveGroupBoxWidgetQt(CompositeProperty* property) : property_(property), btnProperty_("Edit","-") {

        generateWidget();
        updateFromProperty();
    }

    void CollapsiveGroupBoxWidgetQt::generateWidget() {
        btnWidget_ = new ButtonPropertyWidgetQt(&btnProperty_);
        btnWidget_->getButton()->setFixedSize(15,15);

        QHBoxLayout* hLayout = new QHBoxLayout();
        QHBoxLayout* boxLayout = new QHBoxLayout();
        QGridLayout* gridLayout = new QGridLayout();

        groupBox_ = new QGroupBox();
        groupBox_->setFlat(true);

        QFrame* frame = new QFrame();
        frame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
        frame->setLineWidth(2);
        hLayout->addWidget(frame);


        QVBoxLayout* vLayout = new QVBoxLayout();
        vLayout->setAlignment(Qt::AlignTop);
        std::vector<Property*> subProperties = property_->getSubProperties();
        PropertyWidgetFactoryQt* propertyWidgetFactory = new PropertyWidgetFactoryQt();
        for (size_t i=0; i<subProperties.size(); i++) {
            Property* curProperty = subProperties[i];
            PropertyWidgetQt* propertyWidget = propertyWidgetFactory->create(curProperty);
            subPropertyWidgets_.push_back(propertyWidget);
            vLayout->addWidget(propertyWidget);
            curProperty->registerPropertyWidget(propertyWidget);
        } 
        
        groupBox_->setLayout(vLayout);
        groupBox_->layout()->setContentsMargins(0,0,0,0);
        groupBox_->setStyleSheet("border:0;");
        
        gridLayout->setContentsMargins(0,0,0,0);
        gridLayout->setSpacing(0);
        gridLayout->addWidget(btnWidget_,1,1,1,1,Qt::AlignLeft);
        gridLayout->addWidget(groupBox_,1,2,4,4,Qt::AlignTop);

        frame->setLayout(gridLayout);

        setLayout(hLayout);
       
        show();
    }

    void CollapsiveGroupBoxWidgetQt::updateFromProperty() {
        for (size_t i=0; i<subPropertyWidgets_.size(); i++)
            subPropertyWidgets_[i]->updateFromProperty();
    }

    void CollapsiveGroupBoxWidgetQt::show() {
        groupBox_->show();
        btnWidget_->getButton()->setText("-");
        //visible_=true;
        btnProperty_.registerClassMemberFunction(this,&CollapsiveGroupBoxWidgetQt::hide);
    }

    void CollapsiveGroupBoxWidgetQt::hide() {
        groupBox_->hide();
        btnWidget_->getButton()->setText("+");
        //visible_ = false;
        btnProperty_.registerClassMemberFunction(this,&CollapsiveGroupBoxWidgetQt::show);

    }

} // namespace
