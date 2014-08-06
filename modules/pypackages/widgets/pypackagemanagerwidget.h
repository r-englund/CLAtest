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
 * Main file author: Sathish Kottravel
 *
 *********************************************************************************/

#ifndef IVW_PYPACKAGE_MANAGER_WIDGET_H
#define IVW_PYPACKAGE_MANAGER_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QApplication>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>

#include <modules/pypackages/pypackagesmoduledefine.h>
#include <inviwo/qt/widgets/inviwodockwidget.h>

namespace inviwo {

class IVW_MODULE_PYPACKAGES_API PyPackageManagerWidget : public InviwoDockWidget, public Singleton<PyPackageManagerWidget> {
    Q_OBJECT
public:
    PyPackageManagerWidget(QWidget* parent=0);
    ~PyPackageManagerWidget();
    void show();
    void clear();
    void addPackages();
    void updatePackages();
public slots: 
    void mappedButtonClick(QString str);
    void refresh();
private:
    QLineEdit* lineEdit_;
    QVBoxLayout* vLayout_;
    QPushButton* refreshButton_;
    QTableWidget* packageTableWidget_;
    QLabel* satusLabel_;
};

} // namespace

#endif // IVW_PYPACKAGE_MANAGER_WIDGET_H