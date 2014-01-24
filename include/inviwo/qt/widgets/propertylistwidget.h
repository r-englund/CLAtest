/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#ifndef IVW_PROPERTYLISTWIDGET_H
#define IVW_PROPERTYLISTWIDGET_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/core/processors/processor.h>

#include <inviwo/qt/widgets/inviwodockwidget.h>
#include <inviwo/qt/widgets/properties/collapsiblegroupboxwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <QCheckBox>
#include <QScrollArea>

namespace inviwo {

class IVW_QTWIDGETS_API PropertyListWidget : public InviwoDockWidget, public VoidObservable {
    Q_OBJECT

public:
    static PropertyListWidget* instance();

    PropertyListWidget(QWidget* parent);
    ~PropertyListWidget();

    void addProcessorProperties(Processor* processor);
    void removeProcessorProperties(Processor* processor);
    void removeAndDeleteProcessorProperties(Processor* processor);

    void changeName(std::string oldName, std::string newName);

    void saveState();
    void notify();
    PropertyVisibilityMode getVisibilityMode();

    void cacheProcessorPropertiesItem(Processor* processor);

public slots:
    void setDeveloperViewMode(bool value);
    void setApplicationViewMode(bool value);

protected slots:
    void propertyModified();

private:
    QWidget* getProcessorPropertiesItem(Processor* processor);

    void setViewMode(PropertyVisibilityMode viewMode);
    QWidget* createNewProcessorPropertiesItem(Processor* processor);

    bool developerViewMode_;
    bool applicationViewMode_;

    QVBoxLayout* listWidgetLayout_;
    QWidget* listWidget_;
    QScrollArea* scrollArea_;

    std::vector<Property*> properties_;

protected:
    static PropertyListWidget* propertyListWidget_;
    mutable std::map<std::string, QWidget*> propertyWidgetMap_;
};

} // namespace

#endif // IVW_PROPERTYLISTWIDGET_H