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
#include <inviwo/core/processors/processorobserver.h>
#include <inviwo/qt/widgets/inviwodockwidget.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <QCheckBox>
#include <QScrollArea>
#include <QEvent>

namespace inviwo {

class CollapsibleGroupBoxWidgetQt;

class IVW_QTWIDGETS_API PropertyListWidgetObserver : public Observer {
public:
    PropertyListWidgetObserver() : Observer() {};

    /**
    * This method will be called when observed object changes.
    * Override it to add behavior.
    */
    virtual void onPropertyListWidgetChange() {};
};

class IVW_QTWIDGETS_API PropertyListWidgetObservable
    : public Observable<PropertyListWidgetObserver> {
public:
    PropertyListWidgetObservable() : Observable<PropertyListWidgetObserver>() {};

    void notifyPropertyListWidgetObservers() const {
        // Notify observers
        for (ObserverSet::reverse_iterator it = observers_->rbegin(); it != observers_->rend();
             ++it) {
            // static_cast can be used since only template class objects can be added
            static_cast<PropertyListWidgetObserver*>(*it)->onPropertyListWidgetChange();
        }
    }
};

class IVW_QTWIDGETS_API PropertyListFrame : public QWidget {
public:
    PropertyListFrame(QWidget* parent);
    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;
    void paintEvent(QPaintEvent*);
};

class IVW_QTWIDGETS_API PropertyListEvent : public QEvent {
    Q_GADGET
public:

    enum Action {
        ADD = 0,
        REMOVE = 1,
        CACHE = 2
    };

    PropertyListEvent(Action action, const std::string identifier)
        : QEvent(PROPERY_LIST_EVENT)
        , action_(action)
        , identifier_(identifier) {}
    
    static QEvent::Type type() {
        if (PROPERY_LIST_EVENT == QEvent::None) {
            PROPERY_LIST_EVENT = static_cast<QEvent::Type>(QEvent::registerEventType());
        }
        return PROPERY_LIST_EVENT;
    }
    
    Action action_;
    const std::string identifier_;
    
private:
    static QEvent::Type PROPERY_LIST_EVENT;
};


class IVW_QTWIDGETS_API PropertyListWidget : public InviwoDockWidget,
                                             public PropertyListWidgetObservable,
                                             public ProcessorObserver {
    Q_OBJECT

public:
    static PropertyListWidget* instance();

    PropertyListWidget(QWidget* parent);
    ~PropertyListWidget();

    void addProcessorProperties(Processor* processor);
    void removeProcessorProperties(Processor* processor);
    void removeAndDeleteProcessorProperties(Processor* processor);
    
    //override 
    void onProcessorIdentifierChange(Processor*);
    UsageMode getUsageMode();

    void cacheProcessorPropertiesItem(Processor* processor);
    

    void updateProcessorIdentifier(std::string oldName, std::string newName);

    virtual bool event(QEvent* e);

    typedef std::map<Processor*, CollapsibleGroupBoxWidgetQt*> WidgetMap;
    typedef std::vector<CollapsibleGroupBoxWidgetQt*> WidgetVector;

public slots:
    void setUsageMode(bool value);  // True = Application, False = Developer
    void setUsageMode(UsageMode viewMode);

protected slots:
    void propertyModified();

protected:
    static PropertyListWidget* propertyListWidget_;
    mutable WidgetMap widgetMap_;

private:
    CollapsibleGroupBoxWidgetQt* getProcessorPropertiesItem(Processor* processor);
    CollapsibleGroupBoxWidgetQt* createNewProcessorPropertiesItem(Processor* processor);

    UsageMode usageMode_;

    QVBoxLayout* listLayout_;
    WidgetVector devWidgets_;
    QWidget* listWidget_;
    QScrollArea* scrollArea_;


};

}  // namespace

#endif  // IVW_PROPERTYLISTWIDGET_H