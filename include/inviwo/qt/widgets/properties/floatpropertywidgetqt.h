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
 * Main file authors: Timo Ropinski, Alexander Johansson
 *
 *********************************************************************************/

#ifndef IVW_FLOATPROPERTYWIDGETQT_H
#define IVW_FLOATPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QMenu>
#include <QSignalMapper>
#include <inviwo/qt/widgets/editablelabelqt.h>
#include <inviwo/qt/widgets/sliderwidgetqt.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/properties/ordinalproperty.h>

namespace inviwo {

/**
 *  The Widget should work for FloatProperty and FloatVec(2|3|4)Property
 */
class IVW_QTWIDGETS_API BaseOrdinalPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:
    BaseOrdinalPropertyWidgetQt(Property* property);
    virtual ~BaseOrdinalPropertyWidgetQt();
    virtual void updateFromProperty() = 0;

    typedef std::vector<BaseSliderWidgetQt*> SliderVector;

public slots:
    virtual void setPropertyValue(int sliderId) = 0;
    virtual void setAsMin() = 0;
    virtual void setAsMax() = 0;

    void setPropertyDisplayName();
    void showContextMenuSlider(int sliderId);
    void showSettings();

protected:
    virtual SliderVector makeSliders() = 0;
    void generateWidget();
    SliderVector sliderWidgets_;
    QLabel* readOnlyLabel_;
    int sliderId_;

private:
    EditableLabelQt* label_;
    QMenu* contextMenu_;
    PropertySettingsWidgetQt* settingsWidget_;

    QSignalMapper *signalMapperSetPropertyValue_;
    QSignalMapper *signalMapperContextMenu_;

    QAction* settingsAction_;
    QAction* minAction_;
    QAction* maxAction_;
    void generatesSettingsWidget();

};

template <typename BT, typename T>
class TemplateOrdinalPropertyWidgetQt : public BaseOrdinalPropertyWidgetQt {
public:
    TemplateOrdinalPropertyWidgetQt(OrdinalProperty<T>* property)
        : BaseOrdinalPropertyWidgetQt(property)
        , property_(property) {

    }
    virtual void updateFromProperty() = 0;

protected:
    virtual SliderVector makeSliders() {
        SliderVector sliders;
        for(size_t i = 0; i < property_->getDim(); i++) {
            sliders.push_back(new SliderWidgetQt<BT>());
        }
        return sliders;
    }

    virtual std::string getPropertyText() const {
        return glm::to_string(property_->get());
    }

    virtual std::string getPropertyToolTip() const {
        return "Min: " + glm::to_string(property_->getMinValue()) +
            "  Max: " + glm::to_string(property_->getMaxValue());
    }


    virtual void setPropertyValue(int sliderId) = 0;
    virtual void setAsMin() = 0;
    virtual void setAsMax() = 0;

    OrdinalProperty<T>* property_;
};


// Default case for glm vec properties
template <typename BT, typename T>
class OrdinalPropertyWidgetQt : public TemplateOrdinalPropertyWidgetQt<BT, T> {
public:
    OrdinalPropertyWidgetQt(OrdinalProperty<T>* property)
        : TemplateOrdinalPropertyWidgetQt<BT, T>(property) {
            generateWidget();
            updateFromProperty();
        }


    void updateFromProperty() {
        if(property_->getReadOnly()) {
            readOnlyLabel_->setText(QString(getPropertyText().c_str()));
            readOnlyLabel_->setToolTip(QString(getPropertyToolTip().c_str()));
        } else {
            T min = property_->getMinValue();
            T max = property_->getMaxValue();
            T inc = property_->getIncrement();
            T val = property_->get();

            for(size_t i = 0; i < property_->getDim(); i++) {
                SliderWidgetQt<BT>* widget = static_cast<SliderWidgetQt<BT>*>(sliderWidgets_[i]);
                widget->setRange(min[i], max[i]);
                widget->setIncrement(inc[i]);
                widget->initValue(val[i]);
            }
        }
    }

protected:
    // Connected to sliderwidget valueChanged()
    void setPropertyValue(int sliderId) {
        T propValue = property_->get();

        propValue[sliderId] = static_cast<SliderWidgetQt<BT>*>(sliderWidgets_[sliderId])->getValue();

        property_->setInitiatingWidget(this);
        property_->set(propValue);
        property_->clearInitiatingWidget();

        emit modified();
    }
    void setAsMin() {
        SliderWidgetQt<BT>* slider = static_cast<SliderWidgetQt<BT>*>(sliderWidgets_[sliderId_]);
        T propValue = property_->getMinValue();
        propValue[sliderId_] = slider->getValue();

        property_->setInitiatingWidget(this);
        property_->setMinValue(propValue);
        property_->clearInitiatingWidget();

        slider->setMinValue(propValue[sliderId_]);
    }

    void setAsMax() {
        SliderWidgetQt<BT>* slider = static_cast<SliderWidgetQt<BT>*>(sliderWidgets_[sliderId_]);
        T propValue = property_->getMaxValue();
        propValue[sliderId_] = slider->getValue();

        property_->setInitiatingWidget(this);
        property_->setMaxValue(propValue);
        property_->clearInitiatingWidget();

        slider->setMaxValue(propValue[sliderId_]);
    }
};

// Specialization for primitive types
template <typename T>
class OrdinalPropertyWidgetQt<T, T> : public TemplateOrdinalPropertyWidgetQt<T, T>{
public:
    OrdinalPropertyWidgetQt(OrdinalProperty<T>* property)
        : TemplateOrdinalPropertyWidgetQt<T, T>(property) {
            generateWidget();
            updateFromProperty();
        }


    void updateFromProperty() {

        if(property_->getReadOnly()) {
            readOnlyLabel_->setText(QString(getPropertyText().c_str()));
            readOnlyLabel_->setToolTip(QString(getPropertyToolTip().c_str()));
        } else {
            T min = property_->getMinValue();
            T max = property_->getMaxValue();
            T inc = property_->getIncrement();
            T val = property_->get();

            SliderWidgetQt<T>* widget = static_cast<SliderWidgetQt<T>*>(sliderWidgets_[0]);
            widget->setRange(min, max);
            widget->setIncrement(inc);
            widget->initValue(val);
        }
    }

protected:
    // Connected to sliderwidget valueChanged()
    void setPropertyValue(int sliderId) {
        T propValue = property_->get();

        propValue = static_cast<SliderWidgetQt<T>*>(sliderWidgets_[sliderId])->getValue();

        property_->setInitiatingWidget(this);
        property_->set(propValue);
        property_->clearInitiatingWidget();

        emit modified();
    }
    void setAsMin() {
        SliderWidgetQt<T>* slider = static_cast<SliderWidgetQt<T>*>(sliderWidgets_[sliderId_]);
        T propValue = slider->getValue();

        property_->setInitiatingWidget(this);
        property_->setMinValue(propValue);
        property_->clearInitiatingWidget();

        slider->setMinValue(propValue);
    }

    void setAsMax() {
        SliderWidgetQt<T>* slider = static_cast<SliderWidgetQt<T>*>(sliderWidgets_[sliderId_]);
        T propValue = property_->getMaxValue();
        propValue = slider->getValue();

        property_->setInitiatingWidget(this);
        property_->setMaxValue(propValue);
        property_->clearInitiatingWidget();

        slider->setMaxValue(propValue);
    }
};


/*
//Specialization for floats
template <>
class OrdinalPropertyWidgetQt<float, float> : public TemplateOrdinalPropertyWidgetQt<float, float>{
public:
    OrdinalPropertyWidgetQt(OrdinalProperty<float>* property)
        : TemplateOrdinalPropertyWidgetQt<float, float>(property) {
            generateWidget();
            updateFromProperty();
        }


    void updateFromProperty() {

        if(property_->getReadOnly()) {
            readOnlyLabel_->setText(QString(getPropertyText().c_str()));
            readOnlyLabel_->setToolTip(QString(getPropertyToolTip().c_str()));
        } else {
            float min = property_->getMinValue();
            float max = property_->getMaxValue();
            float inc = property_->getIncrement();
            float val = property_->get();

            SliderWidgetQt<float>* widget = static_cast<SliderWidgetQt<float>*>(sliderWidgets_[0]);
            widget->setRange(min, max);
            widget->setIncrement(inc);
            widget->initValue(val);
        }
    }

protected:
    // Connected to sliderwidget valueChanged()
    void setPropertyValue(int sliderId) {
        float propValue = property_->get();

        propValue = static_cast<SliderWidgetQt<float>*>(sliderWidgets_[sliderId])->getValue();

        property_->setInitiatingWidget(this);
        property_->set(propValue);
        property_->clearInitiatingWidget();

        emit modified();
    }
    void setAsMin() {
        SliderWidgetQt<float>* slider = static_cast<SliderWidgetQt<float>*>(sliderWidgets_[sliderId_]);
        float propValue = slider->getValue();

        property_->setInitiatingWidget(this);
        property_->setMinValue(propValue);
        property_->clearInitiatingWidget();

        slider->setMinValue(propValue);
    }

    void setAsMax() {
        SliderWidgetQt<float>* slider = static_cast<SliderWidgetQt<float>*>(sliderWidgets_[sliderId_]);
        float propValue = property_->getMaxValue();
        propValue = slider->getValue();

        property_->setInitiatingWidget(this);
        property_->setMaxValue(propValue);
        property_->clearInitiatingWidget();

        slider->setMaxValue(propValue);
    }
};

//Specialization for double
template <>
class OrdinalPropertyWidgetQt<double, double> : public TemplateOrdinalPropertyWidgetQt<double, double>{
public:
    OrdinalPropertyWidgetQt(OrdinalProperty<double>* property)
        : TemplateOrdinalPropertyWidgetQt<double, double>(property) {
            generateWidget();
            updateFromProperty();
        }


    void updateFromProperty() {

        if(property_->getReadOnly()) {
            readOnlyLabel_->setText(QString(getPropertyText().c_str()));
            readOnlyLabel_->setToolTip(QString(getPropertyToolTip().c_str()));
        } else {
            double min = property_->getMinValue();
            double max = property_->getMaxValue();
            double inc = property_->getIncrement();
            double val = property_->get();

            SliderWidgetQt<double>* widget = static_cast<SliderWidgetQt<double>*>(sliderWidgets_[0]);
            widget->setRange(min, max);
            widget->setIncrement(inc);
            widget->initValue(val);
        }
    }

protected:
    // Connected to sliderwidget valueChanged()
    void setPropertyValue(int sliderId) {
        double propValue = property_->get();

        propValue = static_cast<SliderWidgetQt<double>*>(sliderWidgets_[sliderId])->getValue();

        property_->setInitiatingWidget(this);
        property_->set(propValue);
        property_->clearInitiatingWidget();

        emit modified();
    }
    void setAsMin() {
        SliderWidgetQt<double>* slider = static_cast<SliderWidgetQt<double>*>(sliderWidgets_[sliderId_]);
        double propValue = slider->getValue();

        property_->setInitiatingWidget(this);
        property_->setMinValue(propValue);
        property_->clearInitiatingWidget();

        slider->setMinValue(propValue);
    }

    void setAsMax() {
        SliderWidgetQt<double>* slider = static_cast<SliderWidgetQt<double>*>(sliderWidgets_[sliderId_]);
        double propValue = property_->getMaxValue();
        propValue = slider->getValue();

        property_->setInitiatingWidget(this);
        property_->setMaxValue(propValue);
        property_->clearInitiatingWidget();

        slider->setMaxValue(propValue);
    }
};

//Specialization for ints
template <>
class OrdinalPropertyWidgetQt<int, int> : public TemplateOrdinalPropertyWidgetQt<int, int>{
public:
    OrdinalPropertyWidgetQt(OrdinalProperty<int>* property)
        : TemplateOrdinalPropertyWidgetQt<int, int>(property) {
            generateWidget();
            updateFromProperty();
        }


    void updateFromProperty() {

        if(property_->getReadOnly()) {
            readOnlyLabel_->setText(QString(getPropertyText().c_str()));
            readOnlyLabel_->setToolTip(QString(getPropertyToolTip().c_str()));
        } else {
            int min = property_->getMinValue();
            int max = property_->getMaxValue();
            int inc = property_->getIncrement();
            int val = property_->get();

            SliderWidgetQt<int>* widget = static_cast<SliderWidgetQt<int>*>(sliderWidgets_[0]);
            widget->setRange(min, max);
            widget->setIncrement(inc);
            widget->initValue(val);
        }
    }

protected:
    // Connected to sliderwidget valueChanged()
    void setPropertyValue(int sliderId) {
        int propValue = property_->get();

        propValue = static_cast<SliderWidgetQt<int>*>(sliderWidgets_[sliderId])->getValue();

        property_->setInitiatingWidget(this);
        property_->set(propValue);
        property_->clearInitiatingWidget();

        emit modified();
    }
    void setAsMin() {
        SliderWidgetQt<int>* slider = static_cast<SliderWidgetQt<int>*>(sliderWidgets_[sliderId_]);
        int propValue = slider->getValue();

        property_->setInitiatingWidget(this);
        property_->setMinValue(propValue);
        property_->clearInitiatingWidget();

        slider->setMinValue(propValue);
    }

    void setAsMax() {
        SliderWidgetQt<int>* slider = static_cast<SliderWidgetQt<int>*>(sliderWidgets_[sliderId_]);
        int propValue = property_->getMaxValue();
        propValue = slider->getValue();

        property_->setInitiatingWidget(this);
        property_->setMaxValue(propValue);
        property_->clearInitiatingWidget();

        slider->setMaxValue(propValue);
    }
};
*/

typedef OrdinalPropertyWidgetQt<float, float> FloatPropertyWidgetQt;
typedef OrdinalPropertyWidgetQt<float, vec2> FloatVec2PropertyWidgetQt;
typedef OrdinalPropertyWidgetQt<float, vec3> FloatVec3PropertyWidgetQt;
typedef OrdinalPropertyWidgetQt<float, vec4> FloatVec4PropertyWidgetQt;

typedef OrdinalPropertyWidgetQt<double, double> DoublePropertyWidgetQt;
typedef OrdinalPropertyWidgetQt<double, dvec2> DoubleVec2PropertyWidgetQt;
typedef OrdinalPropertyWidgetQt<double, dvec3> DoubleVec3PropertyWidgetQt;
typedef OrdinalPropertyWidgetQt<double, dvec4> DoubleVec4PropertyWidgetQt;

typedef OrdinalPropertyWidgetQt<int, int> IntPropertyWidgetQt;
typedef OrdinalPropertyWidgetQt<int, ivec2> IntVec2PropertyWidgetQt;
typedef OrdinalPropertyWidgetQt<int, ivec3> IntVec3PropertyWidgetQt;
typedef OrdinalPropertyWidgetQt<int, ivec4> IntVec4PropertyWidgetQt;

} // namespace

#endif // IVW_FLOATPROPERTYWIDGETQT_H