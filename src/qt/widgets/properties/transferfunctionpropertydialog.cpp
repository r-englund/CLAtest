/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 * Version 0.6b
 *
 * Copyright (c) 2013-2014 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Main file author: Timo Ropinski
 *
 *********************************************************************************/

#include <inviwo/qt/widgets/properties/transferfunctionpropertydialog.h>
#include <inviwo/qt/widgets/properties/transferfunctionpropertywidgetqt.h>
#include <inviwo/qt/widgets/properties/collapsiblegroupboxwidgetqt.h>
#include <inviwo/qt/widgets/properties/transferfunctioneditorcontrolpoint.h>
#include <inviwo/core/util/urlparser.h>
#include <QFileDialog>
#include <QDockWidget>
#include <QGraphicsItem>


namespace inviwo {

TransferFunctionPropertyDialog::TransferFunctionPropertyDialog(TransferFunctionProperty* tfProperty,
                                                               QWidget* parent)
    : PropertyEditorWidgetQt("Transfer Function", parent)
    , TransferFunctionObserver()
    , sliderRange_(1000)
    , tfProperty_(tfProperty)
    , tfPixmap_(NULL) {

    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    generateWidget();
    tfProperty_->get().addObserver(this);

    if (!tfProperty_->getVolumeInport()) chkShowHistogram_->setVisible(false);

    gradient_ = new QLinearGradient(0, 0, 100, 20);
    updateTFPreview();
    updateFromProperty();
}

TransferFunctionPropertyDialog::~TransferFunctionPropertyDialog() {
    hide();
    delete tfPixmap_;
    delete tfEditor_;
    delete colorWheel_;
    delete gradient_;
    delete colorDialog_;
}

void TransferFunctionPropertyDialog::generateWidget() {
    vec2 minEditorDims = vec2(255.0f, 100.0f);

    tfEditorView_ = new TransferFunctionEditorView(tfProperty_);
    tfProperty_->get().addObserver(tfEditorView_);
    // put origin to bottom left corner
    tfEditorView_->scale(1.0, -1.0);
    tfEditorView_->setAlignment(Qt::AlignLeft | Qt::AlignBottom);
    tfEditorView_->setMinimumSize(minEditorDims.x, minEditorDims.y);
    tfEditorView_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    tfEditorView_->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    
    tfEditor_ = new TransferFunctionEditor(&tfProperty_->get(), tfEditorView_);
    connect(tfEditor_, SIGNAL(doubleClick()), this, SLOT(showColorDialog()));
    connect(tfEditor_, SIGNAL(selectionChanged()), this, SLOT(updateColorWheel()));
    tfEditorView_->setScene(tfEditor_);

    zoomVSlider_ = new RangeSliderQt(Qt::Vertical, this);
    zoomVSlider_->setRange(0, sliderRange_);
    zoomVSlider_->setMinSeparation(5);
    // flip slider values to compensate for vertical slider layout
    zoomVSlider_->setValue(sliderRange_ - static_cast<int>(tfProperty_->getZoomV().y*sliderRange_),
                           sliderRange_ - static_cast<int>(tfProperty_->getZoomV().x*sliderRange_));
    connect(zoomVSlider_, SIGNAL(valuesChanged(int, int)),
            this, SLOT(changeVerticalZoom(int, int)));
    
    zoomHSlider_ = new RangeSliderQt(Qt::Horizontal, this);
    zoomHSlider_->setRange(0, sliderRange_);
    zoomHSlider_->setMinSeparation(5);
    zoomHSlider_->setValue(static_cast<int>(tfProperty_->getZoomH().x*sliderRange_),
                           static_cast<int>(tfProperty_->getZoomH().y*sliderRange_));
    connect(zoomHSlider_, SIGNAL(valuesChanged(int, int)),
            this, SLOT(changeHorizontalZoom(int, int)));

    maskSlider_ = new RangeSliderQt(Qt::Horizontal, this);
    maskSlider_->setRange(0, sliderRange_);
    maskSlider_->setValue(static_cast<int>(tfProperty_->getMask().x*sliderRange_),
                          static_cast<int>(tfProperty_->getMask().y*sliderRange_));
    connect(maskSlider_, SIGNAL(valuesChanged(int, int)),
            this, SLOT(changeMask(int, int)));
    
    colorChange_ = false;
    colorWheel_ = new ColorWheel();
    connect(colorWheel_, SIGNAL(colorChange(QColor)), this, SLOT(setPointColor(QColor)));
    
    btnClearTF_ = new QPushButton("Reset transfer function");
    connect(btnClearTF_, SIGNAL(clicked()), tfEditor_, SLOT(resetTransferFunction()));
    
    btnImportTF_ = new QPushButton("Import transfer function");
    connect(btnImportTF_, SIGNAL(clicked()), this, SLOT(importTransferFunction()));

    btnExportTF_ = new QPushButton("Export transfer function");
    connect(btnExportTF_, SIGNAL(clicked()), this, SLOT(exportTransferFunction()));
    
    tfPreview_ = new QLabel();
    tfPreview_->setMinimumSize(1,20);
    QSizePolicy sliderPol = tfPreview_->sizePolicy();
    sliderPol.setHorizontalStretch(3);
    tfPreview_->setSizePolicy(sliderPol);
    
    cmbInterpolation_ = new QComboBox();
    cmbInterpolation_->addItem("linear interpolation");
    cmbInterpolation_->addItem("cubic interpolation");
    cmbInterpolation_->setCurrentIndex(tfProperty_->get().getInterpolationType());
    connect(cmbInterpolation_, SIGNAL(currentIndexChanged(int)),
            this, SLOT(switchInterpolationType(int)));
    
    chkShowHistogram_ = new QComboBox();
    chkShowHistogram_->addItem("No Histogram");
    chkShowHistogram_->addItem("Show Histogram");
    chkShowHistogram_->addItem("Histogram 99%");
    chkShowHistogram_->addItem("Histogram 95%");
    chkShowHistogram_->addItem("Histogram 90%");
    chkShowHistogram_->addItem("Histogram Log");
    chkShowHistogram_->setCurrentIndex(tfProperty_->getShowHistogram());
    connect(chkShowHistogram_, SIGNAL(currentIndexChanged(int)), this, SLOT(showHistogram(int)));
    
    colorDialog_ = new QColorDialog();
    colorDialog_->setWindowFlags(Qt::WindowStaysOnTopHint);
    connect(colorDialog_, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(setPointColorDialog()));
    
    QFrame* leftPanel = new QFrame(this);
    QGridLayout* leftLayout = new QGridLayout();
    leftLayout->addWidget(zoomVSlider_,  0, 0);
    leftLayout->addWidget(tfEditorView_, 0, 1);
    leftLayout->addWidget(zoomHSlider_,  1, 1);
    leftLayout->addWidget(tfPreview_,    2, 1);
    leftLayout->addWidget(maskSlider_,   3, 1);
    leftPanel->setLayout(leftLayout);
    
    QFrame* rightPanel = new QFrame(this);
    QVBoxLayout* rightLayout = new QVBoxLayout();
    rightLayout->setAlignment(Qt::AlignTop);
    rightLayout->addWidget(colorWheel_);
    rightLayout->addWidget(btnClearTF_);
    rightLayout->addWidget(btnImportTF_);
    rightLayout->addWidget(btnExportTF_);
    rightLayout->addWidget(cmbInterpolation_);
    rightLayout->addWidget(chkShowHistogram_);
    rightPanel->setLayout(rightLayout);
    
    QFrame* mainPanel = new QFrame(this);
    QHBoxLayout* mainLayout = new QHBoxLayout();
    mainLayout->addWidget(leftPanel);
    mainLayout->addWidget(rightPanel);
    mainPanel->setLayout(mainLayout);
    
    setWidget(mainPanel);
    connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)),
            this, SLOT(dockLocationChanged(Qt::DockWidgetArea)));
    
    initialize(tfProperty_);
    setFloating(true);
    setVisible(false);
}

// Connected to the cmbInterpolation_ button
void TransferFunctionPropertyDialog::switchInterpolationType(int interpolationType) {
    if (interpolationType == 0) {
        tfProperty_->get().setInterpolationType(TransferFunction::InterpolationLinear);
    } else {
        tfProperty_->get().setInterpolationType(TransferFunction::InterpolationCubic);
    }
}

void TransferFunctionPropertyDialog::updateTFPreview() {
    int gradientWidth = tfPreview_->width();
    gradient_->setFinalStop(gradientWidth, 0);

    if (!tfPixmap_ || gradientWidth != tfPixmap_->width()) {
        if (tfPixmap_) {
            delete tfPixmap_;
        }
        tfPixmap_ = new QPixmap(gradientWidth, 20);
    }

    QPainter tfPainter(tfPixmap_);
    QPixmap checkerBoard(10, 10);
    QPainter checkerBoardPainter(&checkerBoard);
    checkerBoardPainter.fillRect(0, 0, 5, 5, Qt::lightGray);
    checkerBoardPainter.fillRect(5, 0, 5, 5, Qt::darkGray);
    checkerBoardPainter.fillRect(0, 5, 5, 5, Qt::darkGray);
    checkerBoardPainter.fillRect(5, 5, 5, 5, Qt::lightGray);
    checkerBoardPainter.end();
    tfPainter.fillRect(0, 0, gradientWidth, 20, QBrush(checkerBoard));
    tfPainter.fillRect(0, 0, gradientWidth, 20, *gradient_);

    // draw masking indicators
    if (tfProperty_->getMask().x > 0.0f) {
        tfPainter.fillRect(0, 0,
                           static_cast<int>(tfProperty_->getMask().x*gradientWidth), 20,
                           QColor(25,25,25,100));

        tfPainter.drawLine(static_cast<int>(tfProperty_->getMask().x*gradientWidth), 0,
                           static_cast<int>(tfProperty_->getMask().x*gradientWidth), 20);
    }

    if (tfProperty_->getMask().y < 1.0f) {
        tfPainter.fillRect(static_cast<int>(tfProperty_->getMask().y*gradientWidth), 0,
                           static_cast<int>((1.0f - tfProperty_->getMask().y)*gradientWidth)+1, 20,
                           QColor(25,25,25,150));

        tfPainter.drawLine(static_cast<int>(tfProperty_->getMask().y*gradientWidth), 0,
                           static_cast<int>(tfProperty_->getMask().y*gradientWidth), 20);
    }

    tfPreview_->setPixmap(*tfPixmap_);
}

void TransferFunctionPropertyDialog::updateFromProperty() {
    std::string processorName = tfProperty_->getOwner()->getProcessor()->getIdentifier();
    QString windowTitle = QString::fromStdString(tfProperty_->getDisplayName() + " (") +
                          QString::fromStdString(processorName) + QString::fromStdString(")");
    setWindowTitle(windowTitle);

    TransferFunction& transFunc = tfProperty_->get();
    QVector<QGradientStop> gradientStops;

    for (int i = 0; i < transFunc.getNumPoints(); i++) {
        TransferFunctionDataPoint* curPoint = transFunc.getPoint(i);
        vec4 curColor = curPoint->getRGBA();

        // increase alpha to allow better visibility by 1 - (a - 1)^4
        float factor = (1.0f - curColor.a) * (1.0f - curColor.a);
        curColor.a = 1.0f - factor * factor;

        gradientStops.append(
            QGradientStop(curPoint->getPos().x,
                          QColor::fromRgbF(curColor.r, curColor.g, curColor.b, curColor.a)));
    }

    gradient_->setStops(gradientStops);
    updateTFPreview();
}

// Connected to selectionChanged() on the tfEditor
void TransferFunctionPropertyDialog::updateColorWheel() {
    QList<QGraphicsItem*> selection = tfEditor_->selectedItems();
    
    if (selection.size() > 0) {
        TransferFunctionEditorControlPoint* tfPoint =
            qgraphicsitem_cast<TransferFunctionEditorControlPoint*>(selection.at(0));

        if (selection.size() == 1 && tfPoint) {
            vec4 color = tfPoint->getPoint()->getRGBA() * 255.0f;
            colorWheel_->blockSignals(true);
            colorWheel_->setColor(QColor(color.r, color.g, color.b, color.a));
            colorWheel_->blockSignals(false);

            if (!colorChange_) {
                colorDialog_->setCurrentColor(QColor(color.r, color.g, color.b, color.a));
            }
        }
    }
}

// Connected to doubleClick on the tfEditor
void TransferFunctionPropertyDialog::showColorDialog() {
    QList<QGraphicsItem*> selection = tfEditor_->selectedItems();

    if (selection.size() == 1 &&
        qgraphicsitem_cast<TransferFunctionEditorControlPoint*>(selection.at(0))) {
        colorDialog_->show();
    }
}

// Connected to currentColorChanged on the colorDialog_
void TransferFunctionPropertyDialog::setPointColorDialog() {
    QColor color = colorDialog_->currentColor();
    setPointColor(color);
    colorChange_ = true;
    updateColorWheel();
    colorChange_ = false;
}

// Connected to colorChange on the colorWheel_
void TransferFunctionPropertyDialog::setPointColor(QColor color) {
    QList<QGraphicsItem*> selection = tfEditor_->selectedItems();
    vec3 newRgb = vec3(color.redF(), color.greenF(), color.blueF());

    for (int i=0; i<selection.size(); i++) {
        TransferFunctionEditorControlPoint* tfcp =
            qgraphicsitem_cast<TransferFunctionEditorControlPoint*>(selection.at(i));

        if (tfcp) {
            tfcp->getPoint()->setRGB(newRgb);
        }
    }
}

void TransferFunctionPropertyDialog::changeVerticalZoom(int zoomMin, int zoomMax) {
    // normalize zoom values, as sliders in TransferFunctionPropertyDialog
    // have the range [0...100]
    // and flip/rescale values to compensate slider layout
    float zoomMaxF = static_cast<float>(sliderRange_ - zoomMin)/sliderRange_;
    float zoomMinF = static_cast<float>(sliderRange_ - zoomMax)/sliderRange_;

    tfProperty_->setZoomV(zoomMinF, zoomMaxF);
    tfEditorView_->updateZoom();
}

void TransferFunctionPropertyDialog::changeHorizontalZoom(int zoomMin, int zoomMax) {
    float zoomMinF = static_cast<float>(zoomMin)/sliderRange_;
    float zoomMaxF = static_cast<float>(zoomMax)/sliderRange_;

    tfProperty_->setZoomH(zoomMinF, zoomMaxF);
    tfEditorView_->updateZoom();
}

// Connected to valuesChanged on the maskSlider
void TransferFunctionPropertyDialog::changeMask(int maskMin, int maskMax) {
    float maskMinF = static_cast<float>(maskMin)/sliderRange_;
    float maskMaxF = static_cast<float>(maskMax)/sliderRange_;
    tfProperty_->setMask(maskMinF, maskMaxF);
    tfEditorView_->setMask(maskMinF, maskMaxF);

    updateTFPreview();
}

void TransferFunctionPropertyDialog::importTransferFunction() {
    QFileDialog importFileDialog(this, QString::fromStdString("Import transfer function"));
    importFileDialog.setDirectory(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_TRANSFERFUNCTIONS).c_str());
    importFileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    importFileDialog.setFileMode(QFileDialog::ExistingFile);
    importFileDialog.setNameFilter("*.itf");

    if (importFileDialog.exec()) {
        QString file = importFileDialog.selectedFiles().at(0);
        IvwDeserializer deserializer(file.toLocal8Bit().constData());
        TransferFunction tf;
        tf.deserialize(deserializer);
        tfProperty_->set(tf);
        updateFromProperty();
    }
}

void TransferFunctionPropertyDialog::exportTransferFunction() {
    QFileDialog exportFileDialog(this, QString::fromStdString("Export transfer function"));
    exportFileDialog.setDirectory(InviwoApplication::getPtr()->getPath(InviwoApplication::PATH_TRANSFERFUNCTIONS).c_str());
    exportFileDialog.setAcceptMode(QFileDialog::AcceptSave);
    exportFileDialog.setFileMode(QFileDialog::AnyFile);
    exportFileDialog.setNameFilter("*.itf");

    if (exportFileDialog.exec()) {
        std::string file = exportFileDialog.selectedFiles().at(0).toLocal8Bit().constData();
        std::string extension = URLParser::getFileExtension(file);

        if (extension == "") {
            file.append(".itf");
        } else if (extension != "itf") {
            URLParser::replaceFileExtension(file, "itf");
        }

        IvwSerializer serializer(file);
        tfProperty_->get().serialize(serializer);
        serializer.writeFile();
    }
}

void TransferFunctionPropertyDialog::showHistogram(int type) {
    tfProperty_->setShowHistogram(type);
    tfEditorView_->setShowHistogram(type);
}

void TransferFunctionPropertyDialog::resizeEvent(QResizeEvent* event) {
    setEditorDimension(ivec2(event->size().width(), event->size().height()));
    QWidget::resizeEvent(event);
    updateTFPreview();
}

void TransferFunctionPropertyDialog::showEvent(QShowEvent* event) {
    updateTFPreview();
    tfEditorView_->update();
    showEditor();
}

void TransferFunctionPropertyDialog::closeEvent(QCloseEvent* event) {
    hideEditor();
}

void TransferFunctionPropertyDialog::moveEvent(QMoveEvent* event) {
    ivec2 pos = ivec2(event->pos().x(), event->pos().y());
    moveEditor(pos);

    if (isFloating() && !(getEditorDockStatus()==PropertyEditorWidgetDockStatus::Floating))
        setDockStatus(PropertyEditorWidgetDockStatus::Floating);

    QWidget::moveEvent(event);
}

void TransferFunctionPropertyDialog::dockLocationChanged(Qt::DockWidgetArea dockArea) {
    if (dockArea == Qt::LeftDockWidgetArea)
        setDockStatus(PropertyEditorWidgetDockStatus::DockedLeft);
    else if (dockArea == Qt::RightDockWidgetArea)
        setDockStatus(PropertyEditorWidgetDockStatus::DockedRight);
    else
        setDockStatus(PropertyEditorWidgetDockStatus::Floating);
}

void TransferFunctionPropertyDialog::onControlPointAdded(TransferFunctionDataPoint* p) {
    tfEditor_->onControlPointAdded(p);
    updateFromProperty();
}

void TransferFunctionPropertyDialog::onControlPointRemoved(TransferFunctionDataPoint* p) {
    tfEditor_->onControlPointRemoved(p);
    updateFromProperty();
}

void TransferFunctionPropertyDialog::onControlPointChanged(const TransferFunctionDataPoint* p) {
    tfEditor_->onControlPointChanged(p);
    updateFromProperty();
}

QLinearGradient* TransferFunctionPropertyDialog::getTFGradient() {
    return gradient_;
}

TransferFunctionEditorView* TransferFunctionPropertyDialog::getEditorView() {
    return tfEditorView_;
}

} // namespace
