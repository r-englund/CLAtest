#ifndef IVW_FLOATMAT3ROPERTYWIDGETQT_H
#define IVW_FLOATMAT3ROPERTYWIDGETQT_H

#include <QLineEdit>


#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/editablelabelqt.h>

#include <inviwo/core/properties/matrixproperties.h>

namespace inviwo {

class IVW_QTWIDGETS_API FloatMat3PropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    FloatMat3PropertyWidgetQt(FloatMat3Property* property);

    void updateFromProperty();

private:
    FloatMat3Property* property_;
    QLineEdit* mat0x0_;
    QLineEdit* mat0x1_;
    QLineEdit* mat0x2_;
    QLineEdit* mat1x0_;
    QLineEdit* mat1x1_;
    QLineEdit* mat1x2_;
    QLineEdit* mat2x0_;
    QLineEdit* mat2x1_;
    QLineEdit* mat2x2_;

    EditableLabelQt* label_;

    void generateWidget();

    public slots:
        void set0x0Value();
        void set0x1Value();
        void set0x2Value();
        void set1x0Value();
        void set1x1Value();
        void set1x2Value();
        void set2x0Value();
        void set2x1Value();
        void set2x2Value();

        void setPropertyValue();
        void setPropertyDisplayName();
        void visibilityModified(int mode){property_->setVisibility(static_cast<PropertyVisibility::VisibilityMode>(mode));};
};

} // namespace

#endif // IVW_FLOATMAT3ROPERTYWIDGETQT_H