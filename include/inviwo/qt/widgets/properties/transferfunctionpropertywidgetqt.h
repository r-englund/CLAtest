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
 * Primary author : Viktor Axelsson
 *
 **********************************************************************/

#ifndef IVW_TRANSFERFUNCTIONPROPERTYWIDGET_H
#define IVW_TRANSFERFUNCTIONPROPERTYWIDGET_H

#include <inviwo/core/properties/properties.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/transferfunctionpropertydialog.h>
#include <inviwo/qt/widgets/inviwoapplicationqt.h>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <QHBoxLayout>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGradient>

namespace inviwo {

class IVW_QTWIDGETS_API TransferFunctionPropertyWidgetQt : public PropertyWidgetQt {
	Q_OBJECT

public:
	TransferFunctionPropertyWidgetQt(TransferFunctionProperty* property);
	~TransferFunctionPropertyWidgetQt();

	void updateFromProperty();

private:
    QToolButton* btnOpenTF_;
    EditableLabelQt* label_;
    QLinearGradient* gradient_;
    QGraphicsView* gradientView_; 
    QGraphicsScene* gradientScene_;
    TransferFunctionProperty* property_;

	TransferFunctionPropertyDialog* transferFunctionDialog_;

	void generateWidget();

	public slots:
		void setPropertyValue();
		void openTransferFunctionDialog();
        void setPropertyDisplayName();
};

}//namespace

#endif //IVW_TRANSFERFUNCTIONPROPERTYWIDGET_H