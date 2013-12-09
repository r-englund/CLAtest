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
 * Primary author : Alexander Johansson
 *
 **********************************************************************/

#include <inviwo/qt/widgets/properties/buttonpropertywidgetqt.h>
#include <QHBoxLayout>
#include <QLabel>


namespace inviwo {

ButtonPropertyWidgetQt::ButtonPropertyWidgetQt(ButtonProperty *property) : property_(property){
    PropertyWidgetQt::setProperty(property_);
    PropertyWidgetQt::generateContextMenu();
	generateWidget();
    updateFromProperty();

	}

void ButtonPropertyWidgetQt::generateWidget() {
	QHBoxLayout* hLayout = new QHBoxLayout();
	button_ = new QPushButton();
    button_->setText(QString::fromStdString(property_->getDisplayName()));
	connect(button_, SIGNAL(released()), this, SLOT(handleButton()));
	hLayout->addWidget(button_);
	setLayout(hLayout);
}

void ButtonPropertyWidgetQt::handleButton(){
    property_->propertyModified();
}
void ButtonPropertyWidgetQt::updateFromProperty() {}

QPushButton* ButtonPropertyWidgetQt::getButton() {
    return button_;
}


} //namespace