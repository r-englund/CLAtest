/** \ Widget for containing the TransferFunctionEditor QGraphicsScene
*       Widget that contains the TransferFunctionEditor and the painted representation
*/

#ifndef IVW_TRANSFERFUNCTIONPROPERTYDIALOG_H
#define IVW_TRANSFERFUNCTIONPROPERTYDIALOG_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/editor/transferfunctioneditor.h>

#include <QCheckBox>
#include <qcolordialog>
#include <QGradientStops>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QWheelEvent>

namespace inviwo {
 
class IVW_QTWIDGETS_API TransferFunctionPropertyDialog : public PropertyWidgetQt {

    Q_OBJECT;

public:
	TransferFunctionPropertyDialog(TransferFunctionProperty* property);

    /** \Updates and draws the visual transferfunction
    *         
    *      Fetches the float array from the TransferFunction and draws a line for each value
    *      Redraws it fully every time, to be optimized if it is allowed to stay
    */
    void updateFromProperty();
    void logStuff();

private:
    int zoom_;
	vec4* data_;
    static const std::string logSource_;
    QGraphicsView*	editorview_; ///< View that contains the editor
    QGraphicsView* paintview_; ///< View that contains the scene for the painted transferfunction
    QGraphicsScene* paintscene_; ///< Scene where the transferfunction is painted
    TransferFunction* transferFunction_; ///< The TransferFunction property
    TransferFunctionProperty* property_; ///< Pointer to property, for get and invalidation in the widget
    TransferFunctionEditor* editor_; ///< TransferFunctionEditor inherited from QGraphicsScene
    std::vector<TransferFunctionEditorControlPoint*> points_; ///< Control points used for calculating transferfunction-values

	QLinearGradient* gradient_;
	QVector<QGradientStop>* stops_;
	QGradientStop* temp_;
	const vec4* color_;
    

    QColorDialog* colorDialog_;

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

#endif // IVW_TRANSFERFUNCTIONPROPERTYDIALOG_H