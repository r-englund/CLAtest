//#ifndef IVW_OPTIONPROPERTYWIDGETQT_H
//#define IVW_OPTIONPROPERTYWIDGETQT_H
//
//#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
//
//#include <QComboBox>
//
//#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
//
//#include <inviwo/core/properties/stringoptionproperty.h>
//
//namespace inviwo {
//
////    class Base :public PropertyWidgetQt
////{
////    Q_OBJECT
////public:
////    Base( QWidget* parent = 0 );
////    ~Base();
////
////public slots:
////    void setPropertyValue();
////};
//
//
//class IVW_QTWIDGETS_API OptionPropertyWidgetQt : public PropertyWidgetQt {
//
//    Q_OBJECT;
//
//public:
//
//    OptionPropertyWidgetQt(StringOptionProperty* property);
//
//    void updateFromProperty();
//
//private:
//
//    StringOptionProperty* property_;
//    QComboBox* comboBox_;
//
//    void generateWidget();
//    void fillComboBox();
//
////public slots:
////    void setPropertyValue();
//};
//
//} // namespace
//
//#endif // IVW_OPTIONPROPERTYWIDGETQT_H