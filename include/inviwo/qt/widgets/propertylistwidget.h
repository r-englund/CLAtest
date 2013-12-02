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
#include <inviwo/qt/widgets/properties/collapsivegroupboxwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetfactoryqt.h>
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

        void showProcessorProperties(Processor* processor);
        void removeProcessorProperties(Processor* processor);
        void showProcessorProperties(std::vector<Processor*> processors);
        void removeAllProcessorProperties();
        void saveState();
        void notify();
        void setMultiSelect(bool multiSelect){multiSelect_ = multiSelect; };
        PropertyVisibilityMode getVisibilityMode();

    public slots:
        void setDeveloperViewMode(bool value);
        void setApplicationViewMode(bool value);

    protected slots:
        void propertyModified();

    private:
        void setViewMode(PropertyVisibilityMode viewMode);
        QWidget* createNewProcessorPropertiesItem(Processor* processor);
        void addProcessorPropertiesToLayout(Processor* processor);
 

        bool developerViewMode_;
        bool applicationViewMode_;
        bool multiSelect_;

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