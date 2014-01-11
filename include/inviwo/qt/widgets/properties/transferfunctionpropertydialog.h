/**********************************************************************
 * Copyright (C) 2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Viktor Axelsson
 *
 **********************************************************************/

/** \ Widget for containing the TransferFunctionEditor QGraphicsScene
*       Widget that contains the TransferFunctionEditor and the painted representation
*/

#ifndef IVW_TRANSFERFUNCTIONPROPERTYDIALOG_H
#define IVW_TRANSFERFUNCTIONPROPERTYDIALOG_H

#include <inviwo/core/common/inviwocoredefine.h>
#include <inviwo/core/properties/transferfunctionproperty.h>
#include <inviwo/qt/widgets/properties/propertywidgetqt.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditor.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorview.h>
#include <inviwo/qt/widgets/colorwheel.h>
#include <inviwo/qt/widgets/rangesliderqt.h>
#include <inviwo/qt/widgets/properties/intminmaxpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>

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
	TransferFunctionPropertyDialog(TransferFunctionProperty* property, QWidget* parent);
	~TransferFunctionPropertyDialog();

    /** \Updates and draws the visual transfer function
    *         
    *      Fetches the float array from the TransferFunction and draws a line for each value
    *      Redraws it fully every time, to be optimized if it is allowed to stay
    */
    void updateFromProperty();
	void notify();
    QVector<QGradientStop>* getGradientStops();
    QPixmap* getTFPreview() { return tfPixmap_; }

public slots:
    void setPointColor();
    void setPointColorDialog();
    void updateColorWheel();
    void showColorDialog();
    void editorViewResized();
    void updateTransferFunction();

    void zoomHorizontally(int zoomHMin, int zoomHMax);
    void zoomVertically(int zoomVMin, int zoomVMax);
    void changeMask(int maskMin, int maskMax);

private:
	int arrayWidth_;
    int arrayHeight_;

    TransferFunctionProperty* tfProperty_;     ///< Pointer to property, for get and invalidation in the widget
    TransferFunctionEditor* tfEditor_;         ///< TransferFunctionEditor inherited from QGraphicsScene
    TransferFunctionEditorView*	tfEditorView_; ///< View that contains the editor

    QGraphicsView* tfPreview_; ///< View that contains the scene for the painted transfer function
    QGraphicsScene* paintscene_; ///< Scene where the transfer function is painted
    QPixmap* tfPixmap_;

	QLinearGradient* gradient_;
	QVector<QGradientStop>* gradientStops_;

    QColorDialog* colorDialog_;
    ColorWheel* colorWheel_;
    
    RangeSliderQt* zoomVSlider_;
    RangeSliderQt* zoomHSlider_;
    RangeSliderQt* maskSlider_;

    bool colorChange_;
    int zoom_;
    bool eventFilter(QObject *object, QEvent *event);
    void generateWidget();
    void setPointColor(QColor color);
    void updateTFPreview();
};

} // namespace

#endif // IVW_TRANSFERFUNCTIONPROPERTYDIALOG_H