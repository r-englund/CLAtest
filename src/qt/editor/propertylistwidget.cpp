#include "inviwo/qt/editor/propertylistwidget.h"

#include <QLabel>
#include <QVBoxLayout>

namespace inviwo {

PropertyListWidget* PropertyListWidget::propertyListWidget_ = 0;

PropertyListWidget::PropertyListWidget(QWidget* parent) : InviwoDockWidget(tr("Properties"), parent) {
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    setMinimumWidth(200);
    propertyListWidget_ = this;
    propertyWidgetFactory_ = new PropertyWidgetFactoryQt();
}

PropertyListWidget::~PropertyListWidget() {}

void PropertyListWidget::showProcessorProperties(Processor* processor) {
    // check if processor widget has been already generated
    std::map<std::string, QWidget*>::iterator it = propertyWidgetMap_.find(processor->getIdentifier());
    if (it != propertyWidgetMap_.end()) {
        // property widget has already been created and stored in the map
        setWidget(it->second);
    } else {
        // create property widget and store it in the map
        QWidget* processorPropertyWidget = new QWidget();
        QVBoxLayout* vLayout = new QVBoxLayout(processorPropertyWidget);
        vLayout->setAlignment(Qt::AlignTop);

        QLabel* processorLabel = new QLabel(QString::fromStdString(processor->getIdentifier()));
        processorLabel->setAlignment(Qt::AlignCenter);
        processorLabel->setAutoFillBackground(true);
        processorLabel->setFrameStyle(QFrame::StyledPanel);
        
        vLayout->addWidget(processorLabel);
        std::vector<Property*> properties = processor->getProperties();
        for (size_t i=0; i<properties.size(); i++) {
            Property* curProperty = properties[i];
            PropertyWidgetQt* propertyWidget = propertyWidgetFactory_->create(curProperty);
            vLayout->addWidget(propertyWidget);
            curProperty->registerPropertyWidget(propertyWidget);
        }
        setWidget(processorPropertyWidget);
        propertyWidgetMap_.insert(std::make_pair(processor->getIdentifier(), processorPropertyWidget));
    }
}

PropertyListWidget* PropertyListWidget::instance() {
    return propertyListWidget_;
}

} // namespace