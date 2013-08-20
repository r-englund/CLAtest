/** \ Widget for containing the TransferFunctionEditor QGraphicsScene
*       Widget that contains the TransferFunctionEditor and the painted representation
*/

#ifndef IVW_TRANSFERFUNCTIONPROPERTYDIALOG_H
#define IVW_TRANSFERFUNCTIONPROPERTYDIALOG_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/editor/transferfunctioneditor.h>
#include <inviwo/qt/editor/transferfunctioneditorview.h>
#include <inviwo/qt/widgets/colorwheel.h>
#include <inviwo/qt/widgets/rangesliderqt.h>

#include <QCheckBox>
#include <QColorDialog>
#include <QGradientStops>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainter>
#include <QPixmap>
#include <QSpinBox>
#include <QWheelEvent>
#include <inviwo/core/util/observer.h>



namespace inviwo {
 
class IVW_QTWIDGETS_API TransferFunctionPropertyDialog : public InviwoDockWidget, public VoidObserver {

    Q_OBJECT;

public:
	//TransferFunctionPropertyDialog();
	TransferFunctionPropertyDialog(TransferFunctionProperty* property, QWidget* parent);
	~TransferFunctionPropertyDialog();

    /** \Updates and draws the visual transferfunction
    *         
    *      Fetches the float array from the TransferFunction and draws a line for each value
    *      Redraws it fully every time, to be optimized if it is allowed to stay
    */
    void updateFromProperty();
    void logStuff();
	void notify();
    QGraphicsView* getGradient();


private:
    int zoom_;
	float width;
	float height;
    static const std::string logSource_;
    TransferFunctionEditorView*	editorview_; ///< View that contains the editor
    QGraphicsView* paintview_; ///< View that contains the scene for the painted transferfunction
    QGraphicsScene* paintscene_; ///< Scene where the transferfunction is painted
    TransferFunctionProperty* property_; ///< Pointer to property, for get and invalidation in the widget
    TransferFunctionEditor* editor_; ///< TransferFunctionEditor inherited from QGraphicsScene

	QLinearGradient* gradient_;
	QVector<QGradientStop>* stops_;

    QColorDialog* colorDialog_;
    ColorWheel* colorWheel_;

	RangeSliderQt* zoomSlider_;
	QSpinBox* spinBoxMin_;
	QSpinBox* spinBoxMax_;

    bool colorChange_;
	bool eventFilter(QObject *object, QEvent *event);

    /** \Mousewheel zoom functionality
    *         Enables zooming in and out with mousescroll
    *         Begining of zoom implementation, currently in early stages
    */
    //void wheelEvent(QWheelEvent * e);
    void generateWidget();

    void setPointColor(QColor color);

	int zoomMin; 
	int zoomMax;

    public slots:
        void setPropertyValue();
        void setPropertyValueColorDialog();
        void updateColorWheel();
        void showColorDialog();
		void updateFromSlider(int valMin, int valMax);
		void updateFromSpinBoxMin(int val);
		void updateFromSpinBoxMax(int val);
};

} // namespace

#endif // IVW_TRANSFERFUNCTIONPROPERTYDIALOG_H