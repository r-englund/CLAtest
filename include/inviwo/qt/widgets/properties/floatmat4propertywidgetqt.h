#ifndef IVW_FLOATMAT4PROPERTYWIDGETQT_H
#define IVW_FLOATMAT4PROPERTYWIDGETQT_H

#include <QLineEdit>


#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/matrixproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API FloatMat4PropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    FloatMat4PropertyWidgetQt(FloatMat4Property* property);

    void updateFromProperty();

private:
    FloatMat4Property* property_;
    QLineEdit* mat0x0_;
    QLineEdit* mat0x1_;
    QLineEdit* mat0x2_;
    QLineEdit* mat0x3_;
    QLineEdit* mat1x0_;
    QLineEdit* mat1x1_;
    QLineEdit* mat1x2_;
    QLineEdit* mat1x3_;
    QLineEdit* mat2x0_;
    QLineEdit* mat2x1_;
    QLineEdit* mat2x2_;
    QLineEdit* mat2x3_;
    QLineEdit* mat3x0_;
    QLineEdit* mat3x1_;
    QLineEdit* mat3x2_;
    QLineEdit* mat3x3_;

    void generateWidget();

    public slots:
        void set0x0Value();
        void set0x1Value();
        void set0x2Value();
        void set0x3Value();
        void set1x0Value();
        void set1x1Value();
        void set1x2Value();
        void set1x3Value();
        void set2x0Value();
        void set2x1Value();
        void set2x2Value();
        void set2x3Value();
        void set3x0Value();
        void set3x1Value();
        void set3x2Value();
        void set3x3Value();

        void setPropertyValue();
};

} // namespace

#endif // IVW_INTMAT4PROPERTYWIDGETQT_H