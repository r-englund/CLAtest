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
#include <inviwo/qt/widgets/properties/transferfunctioneditor.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorview.h>
#include <inviwo/qt/widgets/colorwheel.h>
#include <inviwo/qt/widgets/rangesliderqt.h>
#include <inviwo/qt/widgets/properties/intminmaxpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/optionpropertywidgetqt.h>
#include <inviwo/core/properties/propertywidget.h>

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

class TransferFunctionPropertyWidgetQt;

class IVW_QTWIDGETS_API TransferFunctionPropertyDialog : public InviwoDockWidget, public VoidObserver, public PropertyEditorWidget {

    Q_OBJECT

public:
    TransferFunctionPropertyDialog(TransferFunctionProperty* property, QWidget* parent);
    ~TransferFunctionPropertyDialog();

    /** \Updates and draws the visual transfer function
    *
    *      Fetches the float array from the TransferFunction and draws a line for each value
    *      Redraws it fully every time, to be optimized if it is allowed to stay
    */
    void updateFromProperty();

    QLinearGradient* getTFGradient() { return gradient_; }

    virtual void notify();

public slots:
    void setPointColor();
    void setPointColorDialog();
    void updateColorWheel();
    void showColorDialog();
    void switchInterpolationType(int interpolationType);
    void changeMask(int maskMin, int maskMax);

    void importTransferFunction();
    void exportTransferFunction();
    void showHistogram(bool);
    void dockLocationChanged(Qt::DockWidgetArea dockArea);
private:
    int arrayWidth_;
    int arrayHeight_;

    TransferFunctionProperty* tfProperty_;     ///< Pointer to property, for get and invalidation in the widget
    TransferFunctionEditor* tfEditor_;         ///< TransferFunctionEditor inherited from QGraphicsScene
    TransferFunctionEditorView* tfEditorView_; ///< View that contains the editor
    QPushButton* btnClearTF_;
    QPushButton* btnImportTF_;
    QPushButton* btnExportTF_;
    QComboBox* cmbInterpolation_;
    QCheckBox* chkShowHistogram_;

    QLabel* tfPreview_; ///< View that contains the scene for the painted transfer function
    QPixmap* tfPixmap_;

    QLinearGradient* gradient_;

    QColorDialog* colorDialog_;
    ColorWheel* colorWheel_;

    RangeSliderQt* zoomVSlider_;
    RangeSliderQt* zoomHSlider_;
    RangeSliderQt* maskSlider_;

    bool colorChange_;
    void generateWidget();
    void setPointColor(QColor color);
    void updateTFPreview();
protected:
    virtual void resizeEvent(QResizeEvent*);
    virtual void closeEvent(QCloseEvent*);
    virtual void showEvent(QShowEvent*);
    virtual void moveEvent(QMoveEvent*);
};

} // namespace

#endif // IVW_TRANSFERFUNCTIONPROPERTYDIALOG_H