/** \ Widget for containing the TransferFunctionEditor QGraphicsScene
*       Widget that contains the TransferFunctionEditor and the painted representation
*/

#ifndef IVW_TRANSFERFUNCTIONPROPERTYWIDGETQT_H
#define IVW_TRANSFERFUNCTIONPROPERTYWIDGETQT_H

#include <inviwo/qt/widgets/inviwoqtwidgetsdefine.h>
#include <QCheckBox>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <inviwo/qt/editor/transferfunctioneditor.h>
#include <inviwo/core/properties/scalarproperties.h>
#include <inviwo/qt/widgets/properties/intvec4propertywidgetqt.h>

namespace inviwo {

class IVW_QTWIDGETS_API TransferFunctionPropertyWidgetQt : public PropertyWidgetQt {

    Q_OBJECT;

public:
    TransferFunctionPropertyWidgetQt(TransferFunctionProperty* property);

    /** \Updates and draws the visual transferfunction
    *         
    *      Fetches the float array from the TransferFunction and draws a line for each value
    *      Redraws it fully every time, to be optimized if it is allowed to stay
    */
    void updateFromProperty();

private:
    int zoom_;
    static const std::string logSource_;

    QGraphicsView*	editorview_; ///< View that contains the editor
    QCheckBox* checkBox_; ///< Checkbox for debugging purposes, to disable paintscene
    QGraphicsView* paintview_; ///< View that contains the scene for the painted transferfunction
    QGraphicsScene* paintscene_; ///< Scene where the transferfunction is painted
    TransferFunction* transferFunction_; ///< The TransferFunction property
    TransferFunctionProperty* property_; ///< Pointer to property, for get and invalidation in the widget
    TransferFunctionEditor* editor_; ///< TransferFunctionEditor inherited from QGraphicsScene
    std::vector<TransferFunctionEditorControlPoint*> points_; ///< Control points used for calculating transferfunction-values
    //IntVec4Property intColor_;

    /** \Mousewheel zoom functionality
    *         Enables zooming in and out with mousescroll
    *         Begining of zoom implementation, currently in early stages
    */
    void wheelEvent(QWheelEvent * e);
    void generateWidget();


    public slots:
        void setPropertyValue();
};

} // namespace

#endif // IVW_TRANSFERFUNCTIONPROPERTYWIDGETQT_H