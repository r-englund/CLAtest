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

#include <inviwo/core/io/datareaderdialog.h>
#include <QComboBox>
#include <QDialog>
#include <QLabel>
#include <QSpinBox>

namespace inviwo {

class IVW_CORE_API RawDataReaderDialogQt : public DataReaderDialog, public QDialog {
public:
    RawDataReaderDialogQt::RawDataReaderDialogQt();
    virtual ~RawDataReaderDialogQt();

    virtual const DataFormatBase* getFormat(std::string fileName, uvec3* dimensions, bool* littleEndian);

private:
    QLabel* fileName_;
    QComboBox* bitDepth_;
    QSpinBox* channels_;
    QSpinBox* dimX_;
    QSpinBox* dimY_;
    QSpinBox* dimZ_;
    QSpinBox* timeSteps_;
    QSpinBox* headerOffset_;
    QSpinBox* timeStepOffset_;
    QComboBox* endianess_;
};

} // namespace
