/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
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
 * Main file author: Alexander Johansson
 *
 *********************************************************************************/

#ifndef IVW_COLLAPSIVEGROUPBOXWIDGETQT_H
#define IVW_COLLAPSIVEGROUPBOXWIDGETQT_H

#include <QLineEdit>
#include <QToolButton>
#include <QGroupBox>
#include <QPushButton>

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/qt/widgets/eventpropertymanager.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/propertylistwidget.h>
#include <inviwo/qt/widgets/properties/eventpropertywidgetqt.h>

#include <inviwo/core/properties/property.h>

namespace inviwo {
class IVW_QTWIDGETS_API CollapsibleGroupBoxWidgetQt : public PropertyWidgetQt {
    Q_OBJECT

public:
    CollapsibleGroupBoxWidgetQt(std::string identifier, std::string displayName= "");

    virtual std::string getIdentifier() const;
    virtual void setIdentifier(const std::string& identifier);

    virtual std::string getDisplayName() const;
    virtual void setDisplayName(const std::string& displayName);
   
    virtual PropertyVisibilityMode getVisibilityMode() const;

    void updateFromProperty();
    void addProperty(Property* tmpProperty);
    void generateEventPropertyWidgets(EventPropertyManager* eventPropertyManager);
    
    std::vector<Property*> getProperties();
    
    bool isCollapsed() { return collapsed_; };
    
    virtual void showWidget();
    virtual void hideWidget();

    void addWidget(QWidget* widget);
    void removeWidget(QWidget* widget);
    std::vector<PropertyWidgetQt*> getPropertyWidgets() {return propertyWidgets_; };
    
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    
    virtual void serialize(IvwSerializer& s) const;
    virtual void deserialize(IvwDeserializer& d);

signals:
    void visibilityModified();
    void labelChanged(QString text);

public slots:
    void toggleFold();
    void updateVisibility();
    void setDeveloperViewMode(bool value);
    void setApplicationViewMode(bool value);
    void labelDidChange();
    virtual void resetPropertyToDefaultState();


protected slots:
    void setGroupDisplayName();
    void propertyModified();

protected:
    void generateWidget();
    void updateWidgets();

    std::string identifier_;
    std::string displayName_;
    bool collapsed_;

    std::vector<Property*> properties_;
    std::vector<PropertyWidgetQt*> propertyWidgets_;

private:

    EditableLabelQt* label_;
    QToolButton* btnCollapse_;

    QWidget* propertyWidgetGroup_;
    QVBoxLayout* propertyWidgetGroupLayout_;
    //QFrame* frame_;
    
};

} // namespace

#endif //IVW_COLLAPSIVEGROUPBOXWIDGETQT_H