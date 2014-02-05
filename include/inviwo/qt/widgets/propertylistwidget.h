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
 * Main file authors: Timo Ropinski, Alexander Johansson, Erik Sundén
 *
 *********************************************************************************/

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
    PropertyVisibilityMode getViewMode();

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