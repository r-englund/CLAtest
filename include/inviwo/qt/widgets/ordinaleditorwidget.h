#ifndef IVW_ORDINALEDITORWIDGET_H
#define IVW_ORDINALEDITORWIDGET_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <inviwo/core/common/inviwo.h>
#include <inviwo/qt/widgets/sliderwidgetqt.h>
#include <QLineEdit>

namespace inviwo {

class IVW_QTWIDGETS_API BaseOrdinalEditorWidget : public QWidget  {
    Q_OBJECT
public:
    BaseOrdinalEditorWidget();
    virtual ~BaseOrdinalEditorWidget();

protected: 
    virtual QString transformValueToEditor() = 0;
    virtual void newEditorValue(QString) = 0;

    void applyInit();
    void applyValue();

    QLineEdit* editor_;

private slots:
    void updateFromEditor();

signals:
    void valueChanged();

private:
    void generateWidget();
    void updateEditor();


};

template <typename T>
class TemplateOrdinalEditorWidget : public BaseOrdinalEditorWidget, public OrdinalBaseWidget<T> {
public:
    TemplateOrdinalEditorWidget() : BaseOrdinalEditorWidget() {}
    virtual ~TemplateOrdinalEditorWidget() {}

    virtual T getValue();
    virtual void setValue(T value);
    virtual void initValue(T value);
    virtual void setMinValue(T minValue);
    virtual void setMaxValue(T maxValue);
    virtual void setRange(T minValue, T maxValue);
    virtual void setIncrement(T increment);

protected:
    // Define the transforms
    virtual T editorToRepr(QString val) = 0;
    virtual QString reprToEditor(T val) = 0;

    // Has default implementations using above transformations.
    virtual QString transformValueToEditor();
    virtual void newEditorValue(QString);
    
    T value_;
    T minValue_;
    T maxValue_;
    T increment_;
};


// Default case for fractional numbers
template <typename T>
class OrdinalEditorWidget : public TemplateOrdinalEditorWidget<T> {
public:
    OrdinalEditorWidget() : TemplateOrdinalEditorWidget<T>() {
        this->editor_->setValidator(new QDoubleValidator(this));
    }
    virtual ~OrdinalEditorWidget() {}

protected:  
    // Defines the transform
    virtual T editorToRepr(QString val) {
        QLocale locale = BaseOrdinalEditorWidget::editor_->locale();
        return static_cast<T>(locale.toDouble(val));
    }
    virtual QString reprToEditor(T val) {
        QLocale locale = BaseOrdinalEditorWidget::editor_->locale();
        return locale.toString(val);
    }
};

// Specialization for integer types
template <>
class OrdinalEditorWidget<int> : public TemplateOrdinalEditorWidget<int>{
public:
    OrdinalEditorWidget() : TemplateOrdinalEditorWidget<int>() {
        editor_->setValidator(new QIntValidator(this));
    }
    virtual ~OrdinalEditorWidget() {}

protected:
    // Defines the transform
    virtual int editorToRepr(QString val) {
        QLocale locale = BaseOrdinalEditorWidget::editor_->locale();
        return locale.toInt(val);
    }
    virtual QString reprToEditor(int val) {
        QLocale locale = BaseOrdinalEditorWidget::editor_->locale();
        return locale.toString(val);
    }
};


template <typename T>
QString inviwo::TemplateOrdinalEditorWidget<T>::transformValueToEditor() {
    return reprToEditor(value_);
}
template <typename T>
void inviwo::TemplateOrdinalEditorWidget<T>::newEditorValue(QString val) {
    value_ = editorToRepr(val);
}
template <typename T>
T TemplateOrdinalEditorWidget<T>::getValue() {
    return value_;
}
template <typename T>
void TemplateOrdinalEditorWidget<T>::setValue(T value) {
    if(value >= minValue_ && value <= maxValue_ && value != value_) {
        value_ = value;
        applyValue();
    }
}
template <typename T>
void TemplateOrdinalEditorWidget<T>::initValue(T value) {
    value_ = value;
    applyInit();
}
template <typename T>
void TemplateOrdinalEditorWidget<T>::setMinValue(T minValue) {
    if(minValue_ != minValue) {
        minValue_ = minValue;
    }
}
template <typename T>
void TemplateOrdinalEditorWidget<T>::setMaxValue(T maxValue) {
    if(maxValue_ != maxValue) {
        maxValue_ = maxValue;
    }
}
template <typename T>
void TemplateOrdinalEditorWidget<T>::setRange(T minValue, T maxValue) {
    setMinValue(minValue);
    setMaxValue(maxValue);
}
template <typename T>
void TemplateOrdinalEditorWidget<T>::setIncrement(T increment) {
    if(increment_ != increment) {
        increment_ = increment;
    }
}



} // namespace

#endif // IVW_ORDINALEDITORWIDGET_H

