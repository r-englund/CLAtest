#ifndef IVW_FLOATMAT2PROPERTYWIDGETQT_H
#define IVW_FLOATMAT2PROPERTYWIDGETQT_H

#include <QLineEdit>


#include <inviwo/qt/widgets/properties/propertywidgetqt.h>

#include <inviwo/core/properties/matrixproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API FloatMat2PropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    FloatMat2PropertyWidgetQt(FloatMat2Property* property);

    void updateFromProperty();

private:
    FloatMat2Property* property_;
    QLineEdit* mat0x0_; ///< coordinates 0,0 of the matrix equals upper left corner
    QLineEdit* mat0x1_;
    QLineEdit* mat1x0_;
    QLineEdit* mat1x1_;///< coordinates 1,1 of the matrix equals lower right corner
    

    void generateWidget();

    public slots:
        void set0x0Value();
        void set0x1Value();
        void set1x0Value();
        void set1x1Value();
        void setPropertyValue();
};

} // namespace

#endif // IVW_FLOATMAT2PROPERTYWIDGETQT_H