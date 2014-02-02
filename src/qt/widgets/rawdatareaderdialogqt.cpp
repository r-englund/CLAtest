/**********************************************************************
 * Copyright (C) 2012-2013 Scientific Visualization Group - Linköping University
 * All Rights Reserved.
 * 
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * No part of this software may be reproduced or transmitted in any
 * form or by any means including photocopying or recording without
 * written permission of the copyright owner.
 *
 * Primary author : Timo Ropinski
 *
 **********************************************************************/

#include <inviwo/qt/widgets/rawdatareaderdialogqt.h>
#include <QDialogButtonBox>
#include <QGridLayout>
#include <QGroupBox>

namespace inviwo {

RawDataReaderDialogQt::RawDataReaderDialogQt() {
    setWindowTitle("Importing Raw Data");
    QGridLayout* mainLayout = new QGridLayout(this);

    QLabel* fileNameLabel = new QLabel("Importing file:");
    fileName_= new QLabel();

    QGridLayout* dataTypeLayout = new QGridLayout();
    QLabel* bitDepthLabel = new QLabel("Data format");
    bitDepth_= new QComboBox();
    bitDepth_->addItem("UCHAR");
    bitDepth_->addItem("CHAR");
    bitDepth_->addItem("USHORT");
    bitDepth_->addItem("USHORT_12");
    bitDepth_->addItem("UINT");
    bitDepth_->addItem("INT");
    bitDepth_->addItem("FLOAT");
    bitDepth_->addItem("DOUBLE");
    QLabel* channelLabel = new QLabel("Data channels");
    channels_ = new QSpinBox();
    channels_->setRange(0, 16);
    channels_->setValue(1);
    dataTypeLayout->addWidget(bitDepthLabel, 0, 0);
    dataTypeLayout->addWidget(bitDepth_,     0, 1);
    dataTypeLayout->addWidget(channelLabel,  1, 0);
    dataTypeLayout->addWidget(channels_,     1, 1);
    QGroupBox* dataTypeBox = new QGroupBox("Data type", this);
    dataTypeBox->setLayout(dataTypeLayout);

    QGridLayout* dataSizeLayout = new QGridLayout();
    QLabel* dimensionLabel = new QLabel("Dimensions");
    dimX_= new QSpinBox();
    dimX_->setRange(0, 4096);
    dimX_->setValue(256);
    dimY_= new QSpinBox();
    dimY_->setRange(0, 4096);
    dimY_->setValue(256);
    dimZ_= new QSpinBox();
    dimZ_->setRange(0, 4096);
    dimZ_->setValue(256);
    QWidget* dimensions = new QWidget(this);
    QHBoxLayout* dimensionsLayout = new QHBoxLayout();
    dimensionsLayout->addWidget(dimX_);
    dimensionsLayout->addWidget(dimY_);
    dimensionsLayout->addWidget(dimZ_);
    dimensions->setLayout(dimensionsLayout);
    QLabel* timeStepLabel = new QLabel("Time steps");
    timeSteps_= new QSpinBox();
    dataSizeLayout->addWidget(dimensionLabel, 0, 0);
    dataSizeLayout->addWidget(dimensions,     0, 1);
    dataSizeLayout->addWidget(timeStepLabel,  1, 0);
    dataSizeLayout->addWidget(timeSteps_,     1, 1);
    QGroupBox* dataSizeBox = new QGroupBox("Data size", this);
    dataSizeBox->setLayout(dataSizeLayout);

    QGridLayout* readOptionsLayout = new QGridLayout();
    QLabel* headerOffsetLabel = new QLabel("Header offset");
    headerOffset_ = new QSpinBox();
    headerOffset_->setRange(0, 4096);
    headerOffset_->setValue(0);
    headerOffset_->setSuffix(" Byte");
    QLabel* timeStepOffsetLabel = new QLabel("Time step offset");
    timeStepOffset_ = new QSpinBox();
    timeStepOffset_->setRange(0, 4096);
    timeStepOffset_->setValue(0);
    timeStepOffset_->setSuffix(" Byte");
    QLabel* endianessLabel = new QLabel("Endianess");
    endianess_= new QComboBox();
    endianess_->addItem("Little Endian");
    endianess_->addItem("Big Endian");
    readOptionsLayout->addWidget(headerOffsetLabel,   0, 0);
    readOptionsLayout->addWidget(headerOffset_,       0, 1);
    readOptionsLayout->addWidget(timeStepOffsetLabel, 1, 0);
    readOptionsLayout->addWidget(timeStepOffset_,     1, 1);
    readOptionsLayout->addWidget(endianessLabel,      2, 0);
    readOptionsLayout->addWidget(endianess_,          2, 1);
    QGroupBox* readOptionsBox = new QGroupBox("Read options", this);
    readOptionsBox->setLayout(readOptionsLayout);

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    mainLayout->addWidget(fileNameLabel,  0, 0);
    mainLayout->addWidget(fileName_,      0, 1);
    mainLayout->addWidget(dataTypeBox,    1, 0, 2, 2);
    mainLayout->addWidget(dataSizeBox,    3, 0, 2, 2);
    mainLayout->addWidget(readOptionsBox, 5, 0, 3, 2);
    mainLayout->addWidget(buttonBox,      8, 1);
    setLayout(mainLayout);
}

RawDataReaderDialogQt::~RawDataReaderDialogQt() {
    delete fileName_;
    delete bitDepth_;
    delete channels_;
    delete dimX_;
    delete dimY_;
    delete dimZ_;
    delete timeSteps_;
    delete headerOffset_;
    delete timeStepOffset_;
    delete endianess_;
}

const DataFormatBase* RawDataReaderDialogQt::getFormat(std::string fileName, uvec3* dimensions, bool* littleEndian) {
    fileName_->setText(QString::fromStdString(fileName));
    if (exec() && result() == QDialog::Accepted) {
        const DataFormatBase* format = DataFormatBase::get(bitDepth_->currentText().toStdString());
        dimensions->x = dimX_->value();
        dimensions->y = dimY_->value();
        dimensions->z = dimZ_->value();
        if (endianess_->currentIndex()==0) *littleEndian=true;
        else *littleEndian=false;
        return format;
    }
    else return 0;
}

} // namespace
