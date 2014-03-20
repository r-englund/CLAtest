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
#include <inviwo/qt/widgets/ordinaleditorwidget.h>
#include <inviwo/qt/widgets/properties/propertysettingswidgetqt.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/properties/ordinalproperty.h>
#include <math.h>

namespace inviwo {

class Property;

/**
 *  The Widget should work for FloatProperty and FloatVec(2|3|4)Property
 */
class IVW_QTWIDGETS_API BaseOrdinalPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT

public:
    BaseOrdinalPropertyWidgetQt(Property* property);
    virtual ~BaseOrdinalPropertyWidgetQt();
    virtual void updateFromProperty() = 0;

    typedef std::vector<QWidget*> SliderVector;

public slots:
    virtual void setPropertyValue(int sliderId) = 0;
    virtual void setAsMin() = 0;
    virtual void setAsMax() = 0;

    void setPropertyDisplayName();
    void showContextMenuSlider(int sliderId);
    void showSettings();

protected:
    virtual SliderVector makeSliders(QWidget* widget) = 0;
    void generateWidget();
    SliderVector sliderWidgets_;
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
        , ordinalproperty_(property) {

    }
    virtual void updateFromProperty() = 0;

protected:
    virtual SliderVector makeSliders(QWidget* widget) {
        QSizePolicy sliderPol = widget->sizePolicy();
        sliderPol.setHorizontalStretch(3);
        widget->setSizePolicy(sliderPol);

        QGridLayout* vLayout = new QGridLayout();
        widget->setLayout(vLayout);
        vLayout->setContentsMargins(0, 0, 0, 0);
        vLayout->setSpacing(0);
        
        SliderVector sliders;
        QWidget* controlWidget;
        for (size_t i = 0; i < ordinalproperty_->getDim().x; i++) {
             for (size_t j = 0; j < ordinalproperty_->getDim().y; j++) {
                if (ordinalproperty_->getDim().y > 1) {
                    controlWidget = new OrdinalEditorWidget<BT>();
                } else {
                    controlWidget = new SliderWidgetQt<BT>();
                }
                sliders.push_back(controlWidget);
                vLayout->addWidget(controlWidget, i, j);
            }
        } 
        return sliders;
    }

    virtual std::string getPropertyText() const {
        return glm::to_string(ordinalproperty_->get());
    }

    virtual std::string getPropertyToolTip() const {
        return "Min: " + glm::to_string(ordinalproperty_->getMinValue()) +
             "  Max: " + glm::to_string(ordinalproperty_->getMaxValue());
    }


    virtual void setPropertyValue(int sliderId) = 0;
    virtual void setAsMin() = 0;
    virtual void setAsMax() = 0;

    OrdinalProperty<T>* ordinalproperty_;
};

template <typename BT, typename T>
class glmwrapper {};

template <typename T>
class glmwrapper<T, glm::detail::tmat4x4<T, glm::defaultp> > {
public:
    static T getval(glm::detail::tmat4x4<T, glm::defaultp> mat, size_t const ind) {
        return mat[ind/4][ind % 4];
    }
    static glm::detail::tmat4x4<T, glm::defaultp> setval(glm::detail::tmat4x4<T, glm::defaultp> mat,
                                                          size_t const ind, T val) {
        mat[ind / 4][ind % 4] = val;
        return mat;
    }
};
template <typename T>
class glmwrapper<T, glm::detail::tmat3x3<T, glm::defaultp> > {
public:
    static T getval(glm::detail::tmat3x3<T, glm::defaultp> mat, size_t const ind) {
        return mat[ind / 3][ind % 3];
    }
    static glm::detail::tmat3x3<T, glm::defaultp> setval(glm::detail::tmat3x3<T, glm::defaultp> mat,
                                                          size_t const ind, T val) {
        mat[ind / 3][ind % 3] = val;
        return mat;
    }
};
template <typename T>
class glmwrapper<T, glm::detail::tmat2x2<T, glm::defaultp> > {
public:
    static T getval(glm::detail::tmat2x2<T, glm::defaultp> mat, size_t const ind) {
        return mat[ind / 2][ind % 2];
    }
    static glm::detail::tmat2x2<T, glm::defaultp> setval(glm::detail::tmat2x2<T, glm::defaultp> mat,
                                                          size_t const ind, T val) {
        mat[ind / 2][ind % 2] = val;
        return mat;
    }
};

template <typename T>
class glmwrapper<T, glm::detail::tvec4<T, glm::defaultp> > {
public:
    static T getval(glm::detail::tvec4<T, glm::defaultp> vec, size_t const ind) {
        return vec[ind];
    }
    static glm::detail::tvec4<T, glm::defaultp> setval(glm::detail::tvec4<T, glm::defaultp> vec,
                                                       size_t const ind, T val) {
        vec[ind] = val;
        return vec;
    }
};
template <typename T>
class glmwrapper<T, glm::detail::tvec3<T, glm::defaultp> > {
public:
    static T getval(glm::detail::tvec3<T, glm::defaultp> vec, size_t const ind) {
        return vec[ind];
    }
    static glm::detail::tvec3<T, glm::defaultp> setval(glm::detail::tvec3<T, glm::defaultp> vec,
                                                       size_t const ind, T val) {
        vec[ind] = val;
        return vec;
    }
};
template <typename T>
class glmwrapper<T, glm::detail::tvec2<T, glm::defaultp> > {
public:
    static T getval(glm::detail::tvec2<T, glm::defaultp> vec, size_t const ind) {
        return vec[ind];
    }
    static glm::detail::tvec2<T, glm::defaultp> setval(glm::detail::tvec2<T, glm::defaultp> vec,
                                                       size_t const ind, T val) {
        vec[ind] = val;
        return vec;
    }
};
template <typename T>
class glmwrapper<T, T> {
public:
    static T getval(T val, size_t const ind) {
        return val;
    }
    static T setval(T org, size_t const ind, T val) {
        return val;
    }
};

template <typename T>
class PropertyTransformer {
public:
    PropertyTransformer(OrdinalProperty<T>* prop) : property_(prop) {
    }
    virtual T value(T val) = 0;
    virtual T min(T val) = 0;
    virtual T max(T val) = 0;
    virtual T inc(T val) = 0;

    virtual T invValue(T val) = 0;
    virtual T invMin(T val) = 0;
    virtual T invMax(T val) = 0;
    virtual T invInc(T val) = 0;
protected:
    OrdinalProperty<T>* property_;
};

template <typename T>
class IdentityPropertyTransformer : public PropertyTransformer<T> {
public:
    IdentityPropertyTransformer(OrdinalProperty<T>* prop)
        : PropertyTransformer<T>(prop) {
    }
    virtual T value(T val) { return val; }
    virtual T min(T val) { return val; }
    virtual T max(T val) { return val; }
    virtual T inc(T val) { return val; }
    
    virtual T invValue(T val) { return val; }
    virtual T invMin(T val) { return val; }
    virtual T invMax(T val) { return val; }
    virtual T invInc(T val) { return val; }
};

template <typename T>
class SphericalPropertyTransformer : public IdentityPropertyTransformer<T> {
public:
    SphericalPropertyTransformer(OrdinalProperty<T>* prop)
        : IdentityPropertyTransformer<T>(prop) {
    }
};

template <typename T>
class SphericalPropertyTransformer<glm::detail::tvec3<T, glm::defaultp> > 
    : public PropertyTransformer<glm::detail::tvec3<T, glm::defaultp> > {
public:
    typedef glm::detail::tvec3<T, glm::defaultp> V;

    SphericalPropertyTransformer(OrdinalProperty<V>* prop)
        : PropertyTransformer<V>(prop) {
    }
    
    virtual V value(V val) {
        return V(std::sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2]),
                 arctan(val[2], std::sqrt(val[0] * val[0] + val[1] * val[1])),
                 arctan(val[0], val[1]));
    }
    virtual V min(V val) { return V(0, 0, -M_PI); }
    virtual V max(V val) { return V(3*std::sqrt(val[0] * val[0] + val[1] * val[1] + val[2] * val[2]), M_PI, M_PI); }
    virtual V inc(V val) { return V(0.01, 0.01, 0.01); }
            
    virtual V invValue(V val) { 
        return V(val[0] * std::sin(val[1]) * std::cos(val[2]),
                 val[0] * std::sin(val[1]) * std::sin(val[2]),
                 val[0] * std::cos(val[1]));
    }
    virtual V invMin(V val) { return this->property_->getMinValue(); }
    virtual V invMax(V val) { return this->property_->getMaxValue(); }
    virtual V invInc(V val) { return this->property_->getIncrement(); }

private:
    inline T arctan(T x, T y) {
        if (x == 0) {
            return 0;
        } else if (x < 0 && y > 0) {
            return std::atan(y / x) + M_PI;
        } else if (x < 0 && y < 0) {
            return std::atan(y / x) - M_PI;
        } else {
            return std::atan(y / x);
        }
    }
};

template <typename BT, typename T>
class OrdinalPropertyWidgetQt : public TemplateOrdinalPropertyWidgetQt<BT, T> {
public:
    OrdinalPropertyWidgetQt(OrdinalProperty<T>* property) 
        : TemplateOrdinalPropertyWidgetQt<BT, T>(property) {
        
        if (property->getSemantics() == PropertySemantics("Spherical")) {
            transformer_ = new SphericalPropertyTransformer<T>(property);
        } else {
            transformer_ = new IdentityPropertyTransformer<T>(property);
        }
        BaseOrdinalPropertyWidgetQt::generateWidget();
        updateFromProperty();
    }
    virtual ~OrdinalPropertyWidgetQt() {
        delete transformer_;
    }
    void updateFromProperty();

protected:
    // Connected to sliderwidget valueChanged()
    void setPropertyValue(int);
    void setAsMin();
    void setAsMax();

    PropertyTransformer<T>* transformer_;
};

template <typename BT, typename T>
void OrdinalPropertyWidgetQt<BT, T>::updateFromProperty() {
    this->setEnabled(!this->ordinalproperty_->getReadOnly());

    T min = transformer_->min(this->ordinalproperty_->getMinValue());
    T max = transformer_->max(this->ordinalproperty_->getMaxValue());
    T inc = transformer_->inc(this->ordinalproperty_->getIncrement());
    T val = transformer_->value(this->ordinalproperty_->get());

    for (size_t i = 0; i < this->ordinalproperty_->getDim().x*this->ordinalproperty_->getDim().y; i++) {
        OrdinalBaseWidget<BT>* widget = dynamic_cast<OrdinalBaseWidget<BT>*>(this->sliderWidgets_[i]);
        widget->setRange(glmwrapper<BT,T>::getval(min, i), glmwrapper<BT,T>::getval(max,i));
        widget->setIncrement(glmwrapper<BT,T>::getval(inc, i));
        widget->initValue(glmwrapper<BT,T>::getval(val, i));
    }
}

template <typename BT, typename T>
void OrdinalPropertyWidgetQt<BT, T>::setPropertyValue(int sliderId) {
    T propValue = transformer_->value(this->ordinalproperty_->get());

    propValue = glmwrapper<BT, T>::setval(propValue, sliderId, dynamic_cast<OrdinalBaseWidget<BT>*>(this->sliderWidgets_[sliderId])->getValue());

    this->ordinalproperty_->setInitiatingWidget(this);
    this->ordinalproperty_->set(transformer_->invValue(propValue));
    this->ordinalproperty_->clearInitiatingWidget();
}

template <typename BT, typename T>
void OrdinalPropertyWidgetQt<BT, T>::setAsMin() {
    OrdinalBaseWidget<BT>* slider = dynamic_cast<OrdinalBaseWidget<BT>*>(this->sliderWidgets_[this->sliderId_]);
    T propValue = transformer_->min(this->ordinalproperty_->getMinValue());  
    propValue = glmwrapper<BT, T>::setval(propValue, this->sliderId_, slider->getValue());

    this->ordinalproperty_->setInitiatingWidget(this);
    this->ordinalproperty_->setMinValue(transformer_->invMin(propValue));
    this->ordinalproperty_->clearInitiatingWidget();

    slider->setMinValue(glmwrapper<BT,T>::getval(propValue, this->sliderId_));
}

template <typename BT, typename T>
void OrdinalPropertyWidgetQt<BT, T>::setAsMax() {
    OrdinalBaseWidget<BT>* slider = dynamic_cast<OrdinalBaseWidget<BT>*>(this->sliderWidgets_[this->sliderId_]);
    T propValue = transformer_->max(this->ordinalproperty_->getMaxValue());
    propValue = glmwrapper<BT, T>::setval(propValue, this->sliderId_, slider->getValue());

    this->ordinalproperty_->setInitiatingWidget(this);
    this->ordinalproperty_->setMaxValue(transformer_->invMax(propValue));
    this->ordinalproperty_->clearInitiatingWidget();

    slider->setMaxValue(glmwrapper<BT,T>::getval(propValue, this->sliderId_));
}


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

typedef OrdinalPropertyWidgetQt<float, mat2> FloatMat2PropertyWidgetQt;
typedef OrdinalPropertyWidgetQt<float, mat3> FloatMat3PropertyWidgetQt;
typedef OrdinalPropertyWidgetQt<float, mat4> FloatMat4PropertyWidgetQt;

typedef OrdinalPropertyWidgetQt<double, dmat2> DoubleMat2PropertyWidgetQt;
typedef OrdinalPropertyWidgetQt<double, dmat3> DoubleMat3PropertyWidgetQt;
typedef OrdinalPropertyWidgetQt<double, dmat4> DoubleMat4PropertyWidgetQt;

} // namespace

#endif // IVW_FLOATPROPERTYWIDGETQT_H